#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

#include "algebra.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"
#include "Camera.hpp"

// Screen dimensions
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Camera system
Camera camera(alg::Vec3(0.0f, 0.0f, 5.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// Frame timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Function prototypes
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);


int main() {
    // Initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "3D Graphics Engine - FPS Camera", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Register callback functions
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Capture mouse and hide cursor for FPS-style camera
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Load OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Enable depth testing for proper 3D rendering
    glEnable(GL_DEPTH_TEST);

    // Initialize rendering resources
    std::cout << "\n=== INITIALIZING ENGINE OBJECTS ===" << std::endl;
    Shader ourShader("shaders/basic.vert", "shaders/basic.frag");
    std::cout << "Shader created with ID: " << ourShader.ID << std::endl;

    // Cube geometry with positions, normals, and texture coordinates
    std::vector<Vertex> vertices = {
        // Position             // Normal               // TexCoords
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
    std::cout << "Mesh created with " << vertices.size() << " vertices and " << indices.size() << " indices" << std::endl;
    
    Texture cubeTexture("textures/container.jpg");
    std::cout << "=== INITIALIZATION COMPLETE ===\n" << std::endl;

    // Main render loop
    std::cout << "Starting render loop..." << std::endl;
    std::cout << "Initial camera position: (" << camera.Position.x << ", " << camera.Position.y << ", " << camera.Position.z << ")" << std::endl;
    std::cout << "Yaw: " << camera.Yaw << ", Pitch: " << camera.Pitch << std::endl;
    
    while (!glfwWindowShouldClose(window)) {
        // Calculate delta time for frame-rate independent movement
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Process input
        processInput(window);

        // Clear screen
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Activate shader
        ourShader.use();

        // Set projection and view matrices
        alg::Mat4 projection = alg::Mat4::create_perspective(
            alg::degrees_to_radians(camera.Zoom), 
            (float)SCR_WIDTH / (float)SCR_HEIGHT, 
            0.1f, 100.0f
        );
        alg::Mat4 view = camera.getViewMatrix();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        // Bind texture
        cubeTexture.bind();
        ourShader.setInt("ourTexture", 0);
        
        // Define cube positions for testing FPS camera
        alg::Vec3 cubePositions[] = {
            alg::Vec3( 0.0f,  0.0f,  -3.0f),
            alg::Vec3( 2.0f,  5.0f, -15.0f),
            alg::Vec3(-1.5f, -2.2f, -2.5f),
            alg::Vec3(-3.8f, -2.0f, -12.3f),
            alg::Vec3( 2.4f, -0.4f, -3.5f),
            alg::Vec3(-1.7f,  3.0f, -7.5f),
            alg::Vec3( 1.3f, -2.0f, -2.5f),
            alg::Vec3( 1.5f,  2.0f, -2.5f),
            alg::Vec3( 1.5f,  0.2f, -1.5f),
            alg::Vec3(-1.3f,  1.0f, -1.5f)
        };
        
        // Render multiple cubes with different transformations
        for (int i = 0; i < 10; i++) {
            alg::Mat4 model = alg::Mat4::create_translation(cubePositions[i]);
            
            // Add rotation to some cubes
            float angle = 20.0f * i;
            model = model * alg::Mat4::create_rotation_xyz(alg::Vec3(
                alg::degrees_to_radians(angle),
                alg::degrees_to_radians(angle * 0.3f),
                alg::degrees_to_radians(angle * 0.5f)
            ));
            
            ourShader.setMat4("model", model);
            cubeMesh.draw(ourShader);
        }

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glfwTerminate();
    return 0;
}

// Input processing functions
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
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // Inverted for FPS-style controls
    lastX = xpos;
    lastY = ypos;

    camera.processMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.processMouseScroll(static_cast<float>(yoffset));
}
