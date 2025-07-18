#pragma once
#include "Entity.hpp"
#include "Mesh.hpp"
#include "Material.hpp"
#include "Shader.hpp"
#include "algebra.hpp"

class ModelEntity : public Entity {
public:
    std::shared_ptr<Mesh> mesh;
    Material material;
    std::shared_ptr<Shader> shader;

    ModelEntity(std::shared_ptr<Mesh> mesh,
               std::shared_ptr<Texture> diffuseTexture,
               std::shared_ptr<Shader> shader,
               alg::Vec3 position = alg::Vec3(0.0f,0.0f,0.0f),
               alg::Vec3 rotation = alg::Vec3(0.0f,0.0f,0.0f),
               alg::Vec3 scale = alg::Vec3(1.0f,1.0f,1.0f))
       : Entity("ModelEntity"), mesh(mesh), shader(shader) {
        setPosition(position);
        setRotation(rotation);
        setScale(scale);
        material.setDiffuseTexture(diffuseTexture);
    }



    // Renderiza o modelo
    void render(const alg::Mat4& view, const alg::Mat4& projection, const alg::Vec3& viewPos) {
        shader->use();
        
        // Configura transformações
        shader->setMat4("model", getTransformMatrix());
        shader->setMat4("view", view);
        shader->setMat4("projection", projection);
        shader->setVec3("viewPos", viewPos);
        
        // Configura material
        material.setupInShader(*shader);
        
        // Renderiza o mesh
        mesh->draw(*shader);
    }

    // Interface de usuário
    void drawUI() override {
        Entity::drawUI();


        
        ImGui::Separator();
        material.drawUI();
    }

    // Atualiza a textura difusa
    void setDiffuseTexture(std::shared_ptr<Texture> tex) {
        material.setDiffuseTexture(tex);
    }

    // Atualiza a textura especular
    void setSpecularTexture(std::shared_ptr<Texture> tex) {
        material.setSpecularTexture(tex);
    }

};