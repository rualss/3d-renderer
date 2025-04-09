#pragma once

#include "mesh.h"

#include <vector>

namespace renderer {

class World {
public:
    World();
    World(const std::vector<Mesh>& meshes);

    const std::vector<Mesh>& GetMeshes() const;
    void AddMesh(const Mesh& mesh);
    void AddMesh(Mesh&& mesh);

private:
    std::vector<Mesh> meshes_;
};

}  // namespace renderer
