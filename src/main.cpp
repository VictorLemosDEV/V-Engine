#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

#include "algebra.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"
#include "Camera.hpp"

#include "Model.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;
// --- Câmera ---
// Instancia a câmera com uma posição inicial mais afastada
Camera camera(alg::Vec3(0.0f, 0.0f, 5.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// --- Timing ---
// Garante que o movimento seja o mesmo independente do poder do computador
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// --- Protótipos das Funções de Callback e Input ---
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);


int main() {
    // --- 1. Inicialização ---
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Engine Gráfica - Câmera FPS", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Falha ao criar a janela GLFW" << std::endl;

        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Registra nossas funções de callback para input
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Diz ao GLFW para capturar nosso mouse e esconder o cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Falha ao inicializar GLAD" << std::endl;
        return -1;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark(); // Define o tema de cores

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // Habilita o teste de profundidade para desenhar o 3D corretamente
    glEnable(GL_DEPTH_TEST);

    // --- 2. Criação dos Objetos da Engine ---
    std::cout << "\n=== INICIALIZANDO OBJETOS DA ENGINE ===" << std::endl;
    Shader ourShader("shaders/basic.vert", "shaders/basic.frag");
    std::cout << "Shader criado com ID: " << ourShader.ID << std::endl;

    // Geometria completa do cubo com posições, normais e coordenadas de textura
    std::vector<Vertex> vertices = {
        // Posição                // Normal(não usada ainda) // TexCoords
        { {-0.5f, -0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {0.0f, 0.0f} },
        { { 0.5f, -0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {1.0f, 0.0f} },
        { { 0.5f,  0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {1.0f, 1.0f} },
        { {-0.5f,  0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {0.0f, 1.0f} },

        { {-0.5f, -0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {0.0f, 0.0f} },
        { { 0.5f, -0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {1.0f, 0.0f} },
        { { 0.5f,  0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {1.0f, 1.0f} },
        { {-0.5f,  0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {0.0f, 1.0f} },

        { {-0.5f,  0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 0.0f} },
        { {-0.5f,  0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 1.0f} },
        { {-0.5f, -0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}, {0.0f, 1.0f} },
        { {-0.5f, -0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}, {0.0f, 0.0f} },

        { { 0.5f,  0.5f,  0.5f}, { 1.0f,  0.0f,  0.0f}, {1.0f, 0.0f} },
        { { 0.5f,  0.5f, -0.5f}, { 1.0f,  0.0f,  0.0f}, {1.0f, 1.0f} },
        { { 0.5f, -0.5f, -0.5f}, { 1.0f,  0.0f,  0.0f}, {0.0f, 1.0f} },
        { { 0.5f, -0.5f,  0.5f}, { 1.0f,  0.0f,  0.0f}, {0.0f, 0.0f} },

        { {-0.5f, -0.5f, -0.5f}, { 0.0f, -1.0f,  0.0f}, {0.0f, 1.0f} },
        { { 0.5f, -0.5f, -0.5f}, { 0.0f, -1.0f,  0.0f}, {1.0f, 1.0f} },
        { { 0.5f, -0.5f,  0.5f}, { 0.0f, -1.0f,  0.0f}, {1.0f, 0.0f} },
        { {-0.5f, -0.5f,  0.5f}, { 0.0f, -1.0f,  0.0f}, {0.0f, 0.0f} },

        { {-0.5f,  0.5f, -0.5f}, { 0.0f,  1.0f,  0.0f}, {0.0f, 1.0f} },
        { { 0.5f,  0.5f, -0.5f}, { 0.0f,  1.0f,  0.0f}, {1.0f, 1.0f} },
        { { 0.5f,  0.5f,  0.5f}, { 0.0f,  1.0f,  0.0f}, {1.0f, 0.0f} },
        { {-0.5f,  0.5f,  0.5f}, { 0.0f,  1.0f,  0.0f}, {0.0f, 0.0f} }
    };
    std::vector<unsigned int> indices = {
        0, 1, 2,   2, 3, 0,    4, 5, 6,   6, 7, 4,
        8, 9, 10,  10, 11, 8,  12, 13, 14, 14, 15, 12,
        16, 17, 18, 18, 19, 16, 20, 21, 22, 22, 23, 20
    };
    Mesh cubeMesh(vertices, indices);
    std::cout << "Mesh criado com " << vertices.size() << " vertices e " << indices.size() << " indices" << std::endl;
    
    Texture cubeTexture("textures/container.jpg");
    std::cout << "=== INICIALIZAÇÃO COMPLETA ===\n" << std::endl;

    // --- 3. Loop de Renderização ---
    std::cout << "Iniciando loop de renderização..." << std::endl;
    std::cout << "Posição inicial da câmera: (" << camera.Position.x << ", " << camera.Position.y << ", " << camera.Position.z << ")" << std::endl;
    std::cout << "Yaw: " << camera.Yaw << ", Pitch: " << camera.Pitch << std::endl;
    int frameCount = 0;
while (!glfwWindowShouldClose(window)) {
    // --- Start ImGui Frame ---
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // --- Timing ---
    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // --- Input Processing ---
    processInput(window);

    // --- Clear Screen ---
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // --- 3D Rendering ---
    ourShader.use();

    // Set up projection and view matrices
    alg::Mat4 projection = alg::Mat4::create_perspective(
        alg::degrees_to_radians(camera.Zoom),
        (float)SCR_WIDTH / (float)SCR_HEIGHT,
        0.1f,
        100.0f
    );
    alg::Mat4 view = camera.getViewMatrix();
    ourShader.setMat4("projection", projection);
    ourShader.setMat4("view", view);

    // Bind texture
    cubeTexture.bind();
    ourShader.setInt("ourTexture", 0);

    // Render cubes
    alg::Vec3 cubePositions[] = {
        alg::Vec3(0.0f, 0.0f, -3.0f),
        alg::Vec3(2.0f, 5.0f, -15.0f),
        alg::Vec3(-1.5f, -2.2f, -2.5f),
        alg::Vec3(-3.8f, -2.0f, -12.3f),
        alg::Vec3(2.4f, -0.4f, -3.5f),
        alg::Vec3(-1.7f, 3.0f, -7.5f),
        alg::Vec3(1.3f, -2.0f, -2.5f),
        alg::Vec3(1.5f, 2.0f, -2.5f),
        alg::Vec3(1.5f, 0.2f, -1.5f),
        alg::Vec3(-1.3f, 1.0f, -1.5f)
    };

    for (int i = 0; i < 10; i++) {
        alg::Mat4 model = alg::Mat4::create_translation(cubePositions[i]);
        float angle = 20.0f * i;
        model = model * alg::Mat4::create_rotation_xyz(alg::Vec3(
            alg::degrees_to_radians(angle),
            alg::degrees_to_radians(angle * 0.3f),
            alg::degrees_to_radians(angle * 0.5f)
        ));
        ourShader.setMat4("model", model);
        cubeMesh.draw(ourShader);
    }

    // --- ImGui Rendering ---
    ImGui::ShowDemoWindow();
    ImGui::Render();

    // Disable depth test for UI rendering
    glDisable(GL_DEPTH_TEST);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glEnable(GL_DEPTH_TEST);

    // --- Buffer Swap and Events ---
    glfwSwapBuffers(window);
    glfwPollEvents();
}

    // --- 4. Limpeza ---
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}

// --- Implementação das Funções de Input ---
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.processKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.processKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.processKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.processKeyboard(RIGHT, deltaTime);
    // Tecla TAB para alternar entre modo UI e modo câmera
    static bool tabPressed = false;
    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS && !tabPressed) {
        tabPressed = true;
        int currentMode = glfwGetInputMode(window, GLFW_CURSOR);
        int newMode = (currentMode == GLFW_CURSOR_DISABLED)
                    ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED;
        glfwSetInputMode(window, GLFW_CURSOR, newMode);
    }
    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_RELEASE) {
        tabPressed = false;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    ImGuiIO& io = ImGui::GetIO();
    if (io.WantCaptureMouse) {
        // Quando a UI está ativa, mostra o cursor
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        return;
    } else {
        // Quando a UI não está ativa, esconde o cursor para controle da câmera
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }


    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // Inverted: mouse up = look up
    lastX = xpos;
    lastY = ypos;

    if (!io.WantCaptureMouse) {
        camera.processMouseMovement(xoffset, yoffset);
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    ImGuiIO& io = ImGui::GetIO();
    if (!io.WantCaptureMouse) {
        camera.processMouseScroll(static_cast<float>(yoffset));
    }

}