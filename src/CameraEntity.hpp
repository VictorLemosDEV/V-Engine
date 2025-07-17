// CameraEntity.hpp

#pragma once
#include "Entity.hpp"
#include "Camera.hpp"

class CameraEntity : public Entity {
public:
    Camera camera;
    bool isActive;

    CameraEntity(const std::string& name = "Camera")
        : Entity(name), isActive((false)) {
        camera.Position = position;
    }

    void updateCamera() {
        camera.Position = position;
        camera.updateCameraVectors();
    }

    void drawUI() override {
        Entity::drawUI();
        ImGui::Checkbox("Active", &isActive);
    }

};
