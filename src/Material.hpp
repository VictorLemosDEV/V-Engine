#pragma once
#include "algebra.hpp"
#include "Texture.hpp"
#include <memory>

class Material {
public:
    alg::Vec3 ambient;
    alg::Vec3 diffuse;
    alg::Vec3 specular;
    float shininess;
    std::shared_ptr<Texture> diffuseTexture;
    std::shared_ptr<Texture> specularTexture;

    // Construtores
    Material() :
        ambient(0.1f, 0.1f, 0.1f),
        diffuse(0.8f, 0.8f, 0.8f),
        specular(1.0f, 1.0f, 1.0f),
        shininess(32.0f),
        diffuseTexture(nullptr),
        specularTexture(nullptr) {}

    Material(alg::Vec3 amb, alg::Vec3 diff, alg::Vec3 spec, float shine) :
        ambient(amb),
        diffuse(diff),
        specular(spec),
        shininess(shine),
        diffuseTexture(nullptr),
        specularTexture(nullptr) {}

    // Métodos para configurar texturas
    void setDiffuseTexture(std::shared_ptr<Texture> tex) { diffuseTexture = tex; }
    void setSpecularTexture(std::shared_ptr<Texture> tex) { specularTexture = tex; }

    // Método para configurar o material no shader
    void setupInShader(Shader& shader) const {
        shader.setVec3("material.ambient", ambient);
        shader.setVec3("material.diffuse", diffuse);
        shader.setVec3("material.specular", specular);
        shader.setFloat("material.shininess", shininess);

        // Configura texturas se existirem
        if (diffuseTexture) {
            diffuseTexture->bind(0); // Usa texture unit 0
            shader.setInt("material.diffuseTexture", 0);
        }
        
        if (specularTexture) {
            specularTexture->bind(1); // Usa texture unit 1
            shader.setInt("material.specularTexture", 1);
        }
    }

    // Método para UI no ImGui
    void drawUI() {
        ImGui::Text("Material Properties");
        ImGui::ColorEdit3("Ambient", &ambient.x);
        ImGui::ColorEdit3("Diffuse", &diffuse.x);
        ImGui::ColorEdit3("Specular", &specular.x);
        ImGui::DragFloat("Shininess", &shininess, 1.0f, 1.0f, 256.0f);
    }
};