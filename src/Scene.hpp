// Scene.hpp

#pragma once
#include <vector>
#include <memory>
#include "Entity.hpp"
#include "CameraEntity.hpp"

class Scene {
public:
    std::vector<std::shared_ptr<Entity>> entities;
    std::vector<std::shared_ptr<CameraEntity>> cameras;

    std::shared_ptr<CameraEntity> activeCamera;

    Scene() {
        auto defaultCamera = std::make_shared<CameraEntity>("Main Camera");
        defaultCamera->isActive = true;
        cameras.push_back(defaultCamera);
        activeCamera = defaultCamera;
        entities.push_back(defaultCamera);
    }

    void addEntity(std::shared_ptr<Entity> entity) {
        entities.push_back(entity);

        if (auto camera = std::dynamic_pointer_cast<CameraEntity>(entity)) {
            cameras.push_back(camera);
        }
    }

    void setActiveCamera(std::shared_ptr<CameraEntity> camera) {
        if (activeCamera) activeCamera->isActive = false;
        activeCamera = camera;
        camera->isActive = true;
    }

    void drawSceneUI() {
        ImGui::Begin("Scene Hierarchy");

        for (auto& entity: entities) {
            bool isSelected = (activeCamera && entity == activeCamera); {
            if (ImGui::Selectable(entity->name.c_str(), isSelected)) {
                if (auto camera = std::dynamic_pointer_cast<CameraEntity>(entity)) {
                    setActiveCamera(camera);
                }
            }
            }
        }

        ImGui::End();

        ImGui::Begin("Properties");

        if (activeCamera) {
            activeCamera->drawUI();
        }
        ImGui::End();
    }

};

