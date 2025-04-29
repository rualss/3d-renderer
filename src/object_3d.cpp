#include "object_3d.h"

#include <assimp/Importer.hpp>
#include "model_loader.h"
#include <assimp/postprocess.h>
#include <assimp/scene.h>

namespace renderer {

Mesh& Object3D::operator[](Index i) {
    return meshes_[i];
}

const Mesh& Object3D::operator[](Index i) const {
    return meshes_[i];
}

void Object3D::AddMesh(const Mesh& mesh) {
    meshes_.push_back(mesh);
}

void Object3D::AddMesh(Mesh&& mesh) {
    meshes_.push_back(std::move(mesh));
}

void Object3D::ApplyMatrix(const Mat4& mat) {
    for (Mesh& mesh : meshes_) {
        mesh.ApplyMatrix(mat);
    }
}

const Vec3& Object3D::GetLocalOrigin() const {
    return local_origin_;
}

void Object3D::SetLocalOrigin(const Vec3& new_origin) {
    local_origin_ = new_origin;
}

const std::vector<Mesh>& Object3D::GetMeshes() const {
    return meshes_;
}

}  // namespace renderer
