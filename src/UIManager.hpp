#ifndef UIMANAGER_HPP
#define UIMANAGER_HPP

struct GLFWwindow;

class UIManager {
    public:

    UIManager(GLFWwindow* window);
    ~UIManager();

    void beginFrame();
    void endFrame();

    void renderUI();

private:
};

#endif