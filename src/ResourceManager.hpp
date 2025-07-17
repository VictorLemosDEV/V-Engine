#pragma once
#include <map>
#include <memory>
#include <string>
#include "Shader.hpp"
#include "Texture.hpp"
#include "Mesh.hpp"

class ResourceManager {
public:
    std::map<std::string, std::shared_ptr<Shader>> shaders;
    std::map<std::string, std::shared_ptr<Texture>> textures;
    std::map<std::string, std::shared_ptr<Mesh>> meshes;

    static std::shared_ptr<Shader> loadShader(const std::string& vertexPath, const std::string& fragmentPath) {
        return std::make_shared<Shader>(vertexPath.c_str(), fragmentPath.c_str());
    }

    static std::shared_ptr<Texture> loadTexture(const std::string& path) {
        return std::make_shared<Texture>(path.c_str());
    }

    std::shared_ptr<Mesh> loadMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) {
        std::string key = std::to_string(vertices.size()) + "_" + std::to_string(indices.size());
        if (meshes.count(key)) return meshes[key];
        auto mesh = std::make_shared<Mesh>(vertices, indices);
        meshes[key] = mesh;
        return mesh;
    }
};