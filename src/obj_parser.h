#pragma once

#include <fstream>
#include <string>
#include <vector>
#include "linalg.h"

namespace renderer {
class OBJParser {
public:
    OBJParser(const std::string& path);

    const std::vector<std::vector<Vec3>>& GetFaces();

private:
    std::fstream file_;
    std::vector<Vec3> vertices_;
    std::vector<std::vector<Vec3>> faces_;

    void Parse();
    void ReadVertex(std::istream& stream);
    void ReadFace(std::istream& stream);
};
}  // namespace renderer
