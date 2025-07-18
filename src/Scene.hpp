// Scene.hpp

#pragma once
#include <vector>
#include <memory>
#include "Entity.hpp"
#include "CameraEntity.hpp"
#include "Light.hpp"
#include "ModelEntity.hpp"

class Scene {
public:
    std::vector<std::shared_ptr<Entity>> entities;
    std::vector<std::shared_ptr<CameraEntity>> cameras;
    std::vector<std::shared_ptr<Light>> lights;

    std::shared_ptr<CameraEntity> activeCamera;
    std::shared_ptr<Entity> selectedEntity;

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

    void addLight(std::shared_ptr<Light> light) {
        lights.push_back(light);
        addEntity(light);
    }

    void setupLightsInShader(Shader& shader) {
        int MAX_LIGHTS = 8;
        int lightCount = std::min((int)lights.size(), MAX_LIGHTS);
        shader.setInt("activeLightCount", lightCount);

        for (int i = 0; i < lightCount; i++) {
            lights[i]->setupInShader(shader, i);
        }
    }

    void setActiveCamera(std::shared_ptr<CameraEntity> camera) {
        if (activeCamera) activeCamera->isActive = false;
        activeCamera = camera;
        camera->isActive = true;
    }
    void drawSceneUI() {
        ImGui::Begin("Scene Hierarchy");

        for (auto& entity : entities) {
            bool isSelected = (selectedEntity == entity);
            if (ImGui::Selectable(entity->name.c_str(), isSelected)) {
                selectedEntity = entity;

                // Se for uma câmera, também a torna ativa
                if (auto camera = std::dynamic_pointer_cast<CameraEntity>(entity)) {
                    activeCamera = camera;
                }
            }
        }

        ImGui::End();

        ImGui::Begin("Properties");

        if (selectedEntity) {
            // Mostra UI genérica para qualquer entidade
            selectedEntity->drawUI();


        }

        ImGui::End();
    }
};