#pragma once
#include "Entity.hpp"
#include "algebra.hpp"
#include "Shader.hpp"

class Light : public Entity {
public:
    enum class Type {
        POINT,
        DIRECTIONAL,
        SPOT,
        AREA
    };

    Type type = Type::POINT;
    bool enabled = true;
    alg::Vec3 color = alg::Vec3(1.0f, 1.0f, 1.0f);
    float intensity = 1.0f;
    float range = 10.0f;
    float innerAngle = 15.0f;  // Para spotlight
    float outerAngle = 30.0f;  // Para spotlight
    
    // Construtor
    Light(const std::string& name = "Light") : Entity(name) {}

    void drawUI() override {
        Entity::drawUI();  // Herda a UI básica de Entity (nome, transformações)
        
        ImGui::Separator();
        ImGui::Text("Light Properties");
        
        // Checkbox para ativar/desativar
        ImGui::Checkbox("Enabled", &enabled);
        
        // Tipo de luz
        const char* types[] = { "Point", "Directional", "Spot", "Area" };
        ImGui::Combo("Type", (int*)&type, types, 4);
        
        // Propriedades básicas
        ImGui::ColorEdit3("Color", &color.x);
        ImGui::DragFloat("Intensity", &intensity, 0.1f, 0.0f, 100.0f);
        
        // Propriedades específicas por tipo
        if (type != Type::DIRECTIONAL) {
            ImGui::DragFloat("Range", &range, 0.1f, 0.1f, 1000.0f);
        }
        
        if (type == Type::SPOT) {
            ImGui::DragFloat("Inner Angle", &innerAngle, 0.5f, 0.0f, outerAngle);
            ImGui::DragFloat("Outer Angle", &outerAngle, 0.5f, innerAngle, 90.0f);
        }
    }

    void setupInShader(Shader& shader, int index = 0) const {
        std::string prefix = "lights[" + std::to_string(index) + "].";
        
        shader.setInt(prefix + "type", (int)type);
        shader.setVec3(prefix + "position", position);
        shader.setVec3(prefix + "direction", getForwardDirection());
        shader.setVec3(prefix + "color", color);
        shader.setFloat(prefix + "intensity", intensity);
        shader.setFloat(prefix + "range", range);
        shader.setFloat(prefix + "innerAngle", cos(alg::degrees_to_radians(innerAngle)));
        shader.setFloat(prefix + "outerAngle", cos(alg::degrees_to_radians(outerAngle)));
        shader.setBool(prefix + "enabled", enabled);
    }

private:
    alg::Vec3 getForwardDirection() const {
        // Calcula a direção forward baseada na rotação
        float pitch = rotation.x;
        float yaw = rotation.y;
        
        return alg::Vec3(
            cos(yaw) * cos(pitch),
            sin(pitch),
            sin(yaw) * cos(pitch)
        ).normalized();
    }
};