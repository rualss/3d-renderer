#pragma once

#include "assimp/Importer.hpp"
#include "assimp/material.h"
#include "material.h"
#include "mesh.h"
#include "object_3d.h"
#include "texture_loader.h"

#include <filesystem>
#include <vector>
#include <assimp/scene.h>

namespace renderer {

class ModelLoader {
public:
    using Path = std::filesystem::path;

    void Open(Path path);
    Object3D GetObject();

private:
    bool IsInvalid(const aiScene* model);
    std::vector<Material> ParseMaterials(const aiScene* model, Path path);
    Material ParseMaterial(const aiMaterial* assimp_material, Path path);
    void SetTextures(const aiMaterial* assimp_material, Material* material, Path path);
    void SetTexture(const aiMaterial* assimp_material, Material* material, aiTextureType type,
                    Path path);
    std::vector<Mesh> ParseMeshes(const aiScene* model, const std::vector<Material>& materials);
    Mesh ParseMesh(const aiMesh* assimp_mesh, const std::vector<Material>& materials);

    Assimp::Importer importer_;
    TextureLoader texture_loader_;
    Object3D loaded_object_;
};

}  // namespace renderer
