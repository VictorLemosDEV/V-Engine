// Entity.hpp

#pragma once
#include "algebra.hpp"
#include "imgui.h"
#include "string"
#include "memory"

class Entity {
public:
    std::string name;
    alg::Vec3 position;
    alg::Vec3 rotation;
    alg::Vec3 scale;

    Entity(const std::string& name = "Entity")
        : name(name), position(0.0f, 0.0f, 0.0f), rotation(0.0f, 0.0f, 0.0f), scale(1.0f,1.0f,1.0f) {}

    virtual ~Entity() = default;

    virtual alg::Mat4 getTransformMatrix() const {
        return alg::Mat4::create_translation(position) *
            alg::Mat4::create_rotation_xyz(rotation) *
                alg::Mat4::create_scale(scale);
    }

    virtual void drawUI() {
        ImGui::InputText("Name", &name[0], 256);
        ImGui::DragFloat3("Position", &position.x, 0.1f);
        ImGui::DragFloat3("Rotation", &rotation.x, 0.1f);
        ImGui::DragFloat3("Scale", &scale.x, 0.1f);
    }
};