#ifndef UIMANAGER_HPP
#define UIMANAGER_HPP

#include "Scene.hpp"

struct GLFWwindow;

class UIManager {
    public:

    UIManager(GLFWwindow* window);
    ~UIManager();

    void beginFrame();
    void endFrame();

    void renderUI(Scene& scene);

private:
};

#endif