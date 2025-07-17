#include "UIManager.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "glad/glad.h"
#include <iostream>
#include <memory>

#include "algebra.hpp"
#include "ModelEntity.hpp"
#include "Scene.hpp"

UIManager::UIManager(GLFWwindow *window) {

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();

    if (!ImGui_ImplGlfw_InitForOpenGL(window, true)) {
        std::cerr << "Failed to initializ ImGui!" << std::endl;
    }
    if (!ImGui_ImplOpenGL3_Init()) {
        std::cerr << "Failed to initializ OpenGL loader!" << std::endl;
    }

}

UIManager::~UIManager() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void UIManager::beginFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}
void UIManager::endFrame() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UIManager::renderUI(Scene& scene) {
    // Main Window Function

    ImGui::ShowDemoWindow();

    ImGui::Begin("Engine Control");
    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);




    ImGui::End();

    scene.drawSceneUI();
}

