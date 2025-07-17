// ModelEntity.hpp

#pragma once
#include <memory>
#include "Entity.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "algebra.hpp"

class ModelEntity : public Entity {
public:
    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Texture> texture;
    std::shared_ptr<Shader> shader;
    alg::Vec3 position;
    alg::Vec3 rotation;
    alg::Vec3 scale;

    ModelEntity(
        std::shared_ptr<Mesh> mesh,
        std::shared_ptr<Texture> texture,
        std::shared_ptr<Shader> shader,
        alg::Vec3 position = alg::Vec3(0.0f, 0.0f, 0.0f),
        alg::Vec3 rotation = alg::Vec3(0.0f, 0.0f, 0.0f),
        alg::Vec3 scale = alg::Vec3(1.0f, 1.0f, 1.0f)
    )
        : mesh(mesh), texture(texture), shader(shader),
          position(position), rotation(rotation), scale(scale)
    {}

    alg::Mat4 getTransformMatrix() const {
        alg::Mat4 model = alg::Mat4::create_translation(position);
        model = model * alg::Mat4::create_rotation_x(1.0f);
        model = model * alg::Mat4::create_rotation_y(1.0f);
        model = model * alg::Mat4::create_rotation_z(1.0f);
        model = model * alg::Mat4::create_scale(scale);
        return model;
    }

    void drawModelUI() {
        ImGui::Text("Model Properties");
        // Adicione controles para transformação, textura, etc.
        ImGui::InputText("Name", &name[0], 256);
        ImGui::DragFloat3("Position", &position.x, 0.1f);
        ImGui::DragFloat3("Rotation", &rotation.x, 1.0f);
        ImGui::DragFloat3("Scale", &scale.x, 0.1f);
    }


    void drawUI() override {
        drawModelUI();     // Adiciona propriedades específicas do modelo
    }
};




