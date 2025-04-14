#pragma once

#include "assimp/Importer.hpp"
#include "assimp/material.h"
#include "material.h"
#include "mesh.h"
#include "texture_loader.h"

#include <filesystem>
#include <vector>
#include <assimp/scene.h>

namespace renderer {

class Object3D;

class ModelLoader {
public:
    ModelLoader(std::filesystem::path path);
    Object3D GetObject();

private:
    bool IsInvalid();
    void GetMaterials();
    void GetMeshes();
    void SetTexture(const aiMaterial* assimp_material, Material* material, aiTextureType type);
    void SetTextures(const aiMaterial* assimp_material, Material* material);
    Mesh GetMesh(Index i);

    Assimp::Importer importer_;
    std::filesystem::path path_;
    const aiScene* scene_;
    std::vector<Material> materials_;
    std::vector<Mesh> meshes_;
    TextureLoader texture_loader_;
};
}  // namespace renderer
