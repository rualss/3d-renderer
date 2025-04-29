#include "model_loader.h"
#include "assimp/material.h"
#include "assimp/scene.h"
#include "linalg.h"
#include "object_3d.h"
#include "polygon.h"
#include "texture_loader.h"

#include <assimp/Importer.hpp>
#include <filesystem>
#include <iostream>
#include <vector>
#include <assimp/postprocess.h>

namespace renderer {

void ModelLoader::Open(std::filesystem::path path) {
    const aiScene* model = importer_.ReadFile(
        path.c_str(), aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_FlipUVs);
    if (IsInvalid(model)) {
        loaded_object_ = Object3D{};
    }
    std::vector<Material> materials = ParseMaterials(model, path);
    std::vector<Mesh> meshes = ParseMeshes(model, materials);
    loaded_object_ = Object3D{meshes.begin(), meshes.end()};
}

Object3D ModelLoader::GetObject() {
    return loaded_object_;
}

bool ModelLoader::IsInvalid(const aiScene* model) {
    return (model == nullptr) || (model->mFlags & AI_SCENE_FLAGS_INCOMPLETE) ||
           (model->mRootNode == nullptr);
}

std::vector<Material> ModelLoader::ParseMaterials(const aiScene* model, Path path) {
    std::vector<Material> materials;
    materials.reserve(model->mNumMaterials);
    for (Index i = 0; i < model->mNumMaterials; ++i) {
        const aiMaterial* assimp_material = model->mMaterials[i];
        materials.push_back(std::move(ParseMaterial(model->mMaterials[i], path)));
    }
    return materials;
}

Material ModelLoader::ParseMaterial(const aiMaterial* assimp_material, Path path) {
    assert(assimp_material && "Material must not be nullptr");
    Material material;
    aiString name;
    if (assimp_material->Get(AI_MATKEY_NAME, name) == AI_SUCCESS) {
        if (strcmp(name.C_Str(), "DefaultMaterial") == 0) {
            return material;
        }
    }
    aiColor3D color{material.ambient.r, material.ambient.g, material.ambient.b};
    assimp_material->Get(AI_MATKEY_COLOR_AMBIENT, color);
    material.ambient = {color.r, color.g, color.b};

    color = {material.diffuse.r, material.diffuse.g, material.diffuse.b};
    assimp_material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
    material.diffuse = {color.r, color.g, color.b};

    color = {material.specular.r, material.specular.g, material.specular.b};
    assimp_material->Get(AI_MATKEY_COLOR_SPECULAR, color);
    material.specular = {color.r, color.g, color.b};

    assimp_material->Get(AI_MATKEY_SHININESS, material.shininess);

    int two_sided = 0;
    assimp_material->Get(AI_MATKEY_TWOSIDED, two_sided);
    material.two_sided = (two_sided != 0);

    SetTextures(assimp_material, &material, path);
    return material;
}

void ModelLoader::SetTextures(const aiMaterial* assimp_material, Material* material, Path path) {
    SetTexture(assimp_material, material, aiTextureType_AMBIENT, path);
    SetTexture(assimp_material, material, aiTextureType_DIFFUSE, path);
    SetTexture(assimp_material, material, aiTextureType_SPECULAR, path);
    if (material->diffuse_texture.has_value() && !material->ambient_texture.has_value()) {
        material->ambient_texture = material->diffuse_texture;
    }
}

void ModelLoader::SetTexture(const aiMaterial* assimp_material, Material* material,
                             aiTextureType type, Path path) {
    if (assimp_material->GetTextureCount(type) > 0) {
        std::filesystem::path parent_path = path.parent_path();
        aiString path;
        assimp_material->GetTexture(type, 0, &path);
        std::filesystem::path path_to_texture = parent_path;
        path_to_texture /= std::filesystem::path(path.C_Str());
        switch (type) {
            case aiTextureType_AMBIENT:
                material->ambient_texture = texture_loader_.LoadTexture(path_to_texture);
                break;
            case aiTextureType_DIFFUSE:
                material->diffuse_texture = texture_loader_.LoadTexture(path_to_texture);
                break;
            case aiTextureType_SPECULAR:
                material->specular_texture = texture_loader_.LoadTexture(path_to_texture);
                break;
            default:
                assert(false);
                break;
        }
    }
}

std::vector<Mesh> ModelLoader::ParseMeshes(const aiScene* model,
                                           const std::vector<Material>& materials) {
    std::vector<Mesh> meshes;
    meshes.reserve(model->mNumMeshes);
    for (Index i = 0; i < model->mNumMeshes; ++i) {
        meshes.push_back(std::move(ParseMesh(model->mMeshes[i], materials)));
    }
    return meshes;
}

Mesh ModelLoader::ParseMesh(const aiMesh* assimp_mesh, const std::vector<Material>& materials) {
    assert(assimp_mesh && "Mesh must not be nullptr");
    Mesh mesh;
    mesh.SetMaterial(materials[assimp_mesh->mMaterialIndex]);
    std::vector<Vec3> vertices;
    std::vector<Vec3> normals;
    std::vector<Vec2> texture_coordinates;
    vertices.reserve(assimp_mesh->mNumVertices);
    normals.reserve(assimp_mesh->mNumVertices);
    bool has_texture = false;
    if (assimp_mesh->mTextureCoords[0]) {
        texture_coordinates.reserve(assimp_mesh->mNumVertices);
        has_texture = true;
    }
    for (Index i = 0; i < assimp_mesh->mNumVertices; ++i) {
        vertices.emplace_back(assimp_mesh->mVertices[i].x, assimp_mesh->mVertices[i].y,
                              assimp_mesh->mVertices[i].z);
        normals.emplace_back(assimp_mesh->mNormals[i].x, assimp_mesh->mNormals[i].y,
                             assimp_mesh->mNormals[i].z);
        if (has_texture) {
            texture_coordinates.emplace_back(assimp_mesh->mTextureCoords[0][i].x,
                                             assimp_mesh->mTextureCoords[0][i].y);
        }
    }
    for (Index i = 0; i < assimp_mesh->mNumFaces; ++i) {
        const aiFace assimp_face = assimp_mesh->mFaces[i];
        assert(assimp_face.mNumIndices == 3 && "Face must be a triangle");
        Polygon polygon;
        if (has_texture) {
            polygon.texture_vertices.emplace();
        }
        for (int i = 0; i < 3; ++i) {
            polygon.vertices[i] = vertices[assimp_face.mIndices[i]];
            polygon.normals[i] = normals[assimp_face.mIndices[i]];
            if (has_texture) {
                polygon.texture_vertices.value()[i] = texture_coordinates[assimp_face.mIndices[i]];
            }
        }
        mesh.AddPolygon(std::move(polygon));
    }
    return mesh;
}

}  // namespace renderer
