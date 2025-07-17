// ModelEntity.hpp

#pragma once
#include "Entity.hpp"
#include "Model.hpp"

class ModelEntity : public Entity {
    public:
    std::shared_ptr<Model> model;

    ModelEntity(std::shared_ptr<Model> model, const std::string& name = "Model")
        : Entity(name), model(model) {}

    void drawUI() override
    {
        Entity::drawUI();
    }

};




