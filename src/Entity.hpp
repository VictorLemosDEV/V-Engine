// Entity.hpp

#pragma once
#include "algebra.hpp"
#include "imgui.h"
#include "string"
#include "memory"

class Entity {
public:
    std::string name;
    bool enabled = true;


    Entity(const std::string& name = "Entity")
        : name(name), position(0.0f, 0.0f, 0.0f), rotation(0.0f, 0.0f, 0.0f), scale(1.0f,1.0f,1.0f) {}

    virtual ~Entity() = default;

    const alg::Vec3& getPosition() const { return position; }
    const alg::Vec3& getRotation() const { return rotation; }
    const alg::Vec3& getScale() const { return scale; }

    // Setters
    void setPosition(const alg::Vec3& newPos) { position = newPos; }
    void setRotation(const alg::Vec3& newRot) { rotation = newRot; }
    void setScale(const alg::Vec3& newScale) { scale = newScale; }

    virtual alg::Mat4 getTransformMatrix() const {
        return alg::Mat4::create_translation(position) *
            alg::Mat4::create_rotation_xyz(rotation) *
                alg::Mat4::create_scale(scale);
    }

    virtual void drawUI() {
       char nameBuffer[256];
        strncpy(nameBuffer, name.c_str(), sizeof(nameBuffer));
        if (ImGui::InputText("Name", nameBuffer, sizeof(nameBuffer))) {
            name = nameBuffer;
        }

        ImGui::Checkbox("Enabled", &enabled);

        ImGui::Separator();
        ImGui::Text("Transform");
        ImGui::DragFloat3("Position", &position.x, 0.1f);
        ImGui::DragFloat3("Rotation", &rotation.x, 1.0f, -180.0f, 180.0f);
        ImGui::DragFloat3("Scale", &scale.x, 0.1f, 0.01f, 10.0f);
    }

protected:
    alg::Vec3 position = alg::Vec3(0.0f,0.0f,0.0f);
    alg::Vec3 rotation = alg::Vec3(0.0f,0.0f,0.0f);
    alg::Vec3 scale = alg::Vec3(1.0f,1.0f,1.0f);
};