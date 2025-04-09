#include "world.h"

namespace renderer {

World::World() = default;
World::World(const std::vector<Mesh>& meshes) : meshes_(meshes) {
}

const std::vector<Mesh>& World::GetMeshes() const {
    return meshes_;
}

void World::AddMesh(const Mesh& mesh) {
    meshes_.push_back(mesh);
}

void World::AddMesh(Mesh&& mesh) {
    meshes_.emplace_back(std::move(mesh));
}

}  // namespace renderer
