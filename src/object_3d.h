#pragma once

#include <filesystem>
#include <iostream>
#include <vector>
#include "mesh.h"

namespace renderer {

class Object3D {
public:
    Object3D() = default;

    template <std::input_iterator InputIt>
    Object3D(InputIt first, InputIt last) : meshes_(first, last) {
    }

    Mesh& operator[](Index i);
    const Mesh& operator[](Index i) const;
    void AddMesh(const Mesh& polygon);
    void AddMesh(Mesh&& polygon);
    void ApplyMatrix(const Mat4& mat);
    const Vec3& GetLocalOrigin() const;
    void SetLocalOrigin(const Vec3& new_origin);
    const std::vector<Mesh>& GetMeshes() const;

private:
    static constexpr Vec3 kDefaultLocalOrigin = {0, 0, 0};

    std::vector<Mesh> meshes_;
    Vec3 local_origin_ = kDefaultLocalOrigin;
};

}  // namespace renderer
