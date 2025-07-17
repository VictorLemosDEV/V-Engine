#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

#include "algebra.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"
#include "Camera.hpp"
#include "imgui.h"

#include "Model.hpp"
#include "ModelEntity.hpp"

#include "UIManager.hpp"
#include "ResourceManager.hpp"

const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;
// --- Câmera ---
// Instancia a câmera com uma posição inicial mais afastada
Camera camera(alg::Vec3(0.0f, 0.0f, 5.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
bool cameraActive = false;

// --- Timing ---
// Garante que o movimento seja o mesmo independente do poder do computador
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// --- Protótipos das Funções de Callback e Input ---
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

std::shared_ptr<Texture> createWhiteTexture() {
    unsigned int whiteTextureID;
    glGenTextures(1, &whiteTextureID);
    glBindTexture(GL_TEXTURE_2D, whiteTextureID);
    unsigned char whitePixel[] = {255, 255, 255, 255};
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, whitePixel);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Assuming your Texture class has a constructor that takes texture ID
    return std::make_shared<Texture>(whiteTextureID);
}


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
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    // Diz ao GLFW para capturar nosso mouse e esconder o cursor

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Falha ao inicializar GLAD" << std::endl;
        return -1;
    }



    // Habilita o teste de profundidade para desenhar o 3D corretamente
    glEnable(GL_DEPTH_TEST);

    // --- 2. Criação dos Objetos da Engine ---
    std::cout << "\n=== INICIALIZANDO OBJETOS DA ENGINE ===" << std::endl;
    Shader ourShader("shaders/basic.vert", "shaders/basic.frag");
    std::cout << "Shader criado com ID: " << ourShader.ID << std::endl;

    Scene scene;
    UIManager uiManager(window);
    ResourceManager resourceManager;

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

    // Create shared_ptr for Shader first (don't create stack-allocated ourShader)
    auto shaderPtr = std::make_shared<Shader>("shaders/basic.vert", "shaders/basic.frag");
    std::cout << "Shader criado com ID: " << shaderPtr->ID << std::endl;


    auto texturePtr = createWhiteTexture();  // We'll make this a function

    // Create mesh
    auto meshPtr = std::make_shared<Mesh>(vertices, indices);

    // Create entity
    auto cubeEntity = std::make_shared<ModelEntity>(
    meshPtr,
    texturePtr,  // Texture comes before Shader
    shaderPtr
);
    scene.addEntity(cubeEntity);

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


    // --- Timing ---
    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // --- Input Processing ---
    processInput(window);

    if (scene.activeCamera) {
        scene.activeCamera->updateCamera();
        camera = scene.activeCamera->camera;
    }

    uiManager.beginFrame();

    // --- Clear Screen ---
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // --- ImGui Rendering ---
    uiManager.renderUI(scene);

    // --- 3D Rendering ---
    for (auto& entity : scene.entities) {
        if (auto modelEntity = std::dynamic_pointer_cast<ModelEntity>(entity)) {
            modelEntity->shader->use();

            alg::Mat4 projection = alg::Mat4::create_perspective(
                alg::degrees_to_radians(camera.Zoom),
                (float)SCR_WIDTH / (float)SCR_HEIGHT,
                0.1f,
                100.0f
            );
            alg::Mat4 view = camera.getViewMatrix();

            modelEntity->shader->setMat4("projection", projection);
            modelEntity->shader->setMat4("view", view);
            modelEntity->shader->setMat4("model", modelEntity->getTransformMatrix());

            modelEntity->texture->bind();
            modelEntity->shader->setInt("ourTexture", 0);

            modelEntity->mesh->draw(*modelEntity->shader);
        }
    }

    uiManager.endFrame();




    // --- Buffer Swap and Events ---
    glfwSwapBuffers(window);
    glfwPollEvents();
}

    // --- 4. Limpeza ---
    glfwTerminate();
    return 0;
}

// --- Implementação das Funções de Input ---
void processInput(GLFWwindow *window) {
    if (cameraActive) {
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

    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    ImGuiIO& io = ImGui::GetIO();
    if (!cameraActive || io.WantCaptureMouse) {
        return;
    }

    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);




    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // Inverted: mouse up = look up
    lastX = xpos;
    lastY = ypos;

    camera.processMouseMovement(xoffset, yoffset);
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        ImGuiIO& io = ImGui::GetIO();
        if (io.WantCaptureMouse) {
            return;
        }

        if (action == GLFW_PRESS) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            cameraActive = true;
            firstMouse = true;
        } else if (action == GLFW_RELEASE) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            cameraActive = false;
        }
    }
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    ImGuiIO& io = ImGui::GetIO();
    if (io.WantCaptureMouse) {
       return;
    }

    camera.processMouseScroll(static_cast<float>(yoffset));

}