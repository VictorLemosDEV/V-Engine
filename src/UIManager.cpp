#include "UIManager.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "glad/glad.h"
#include <iostream>

#include "algebra.hpp"

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

void UIManager::renderUI() {
    // Main Window Function

    ImGui::ShowDemoWindow();

    ImGui::Begin("Engine Control");
    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
    ImGui::Separator();

    static float cor_do_fundo[3] = {0.2f, 0.3f, 0.3f};
    ImGui::ColorEdit3("Cor de Fundo", cor_do_fundo);
    glClearColor(cor_do_fundo[0], cor_do_fundo[1], cor_do_fundo[2], 1.0f); // Usa a cor escolhida

    ImGui::End();
}

