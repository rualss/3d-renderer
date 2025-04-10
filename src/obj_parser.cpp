#include "obj_parser.h"

#include <array>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// TODO Validate obj file

namespace renderer {
OBJParser::OBJParser(const std::string& path) : file_(path) {
    Parse();
}

const std::vector<std::vector<Vec3>>& OBJParser::GetFaces() {
    return faces_;
}

void OBJParser::Parse() {
    for (std::string line; std::getline(file_, line);) {
        std::istringstream line_stream(line);
        std::string entry_type;
        line_stream >> entry_type;
        if (entry_type == "v") {
            ReadVertex(line_stream);
        } else if (entry_type == "f") {
            ReadFace(line_stream);
        }
    }
}

void OBJParser::ReadVertex(std::istream& stream) {
    Vec3 vertex;
    for (Index i = 0; i < 3 && (stream >> vertex[i]); ++i) {
    }
    vertices_.emplace_back(std::move(vertex));
}

void OBJParser::ReadFace(std::istream& stream) {
    std::vector<Vec3> face;
    for (std::string current_token; stream >> current_token;) {
        std::string num;
        std::getline(std::stringstream{current_token}, num, '/');
        Index id = std::atoi(num.data());
        if (id < 0) {
            id = vertices_.size() - id;
        } else {
            --id;
        }
        face.push_back(vertices_[id]);
    }
    faces_.emplace_back(std::move(face));
}

}  // namespace renderer
