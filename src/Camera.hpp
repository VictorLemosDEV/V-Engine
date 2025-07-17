#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glad/glad.h>
#include "algebra.hpp"

// Enum para as direções de movimento possíveis
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Valores padrão da câmera
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.08f;  // Balanced sensitivity for FPS
const float ZOOM        =  45.0f;

class Camera {
public:
    // Atributos da Câmera
    alg::Vec3 Position;
    alg::Vec3 Front;
    alg::Vec3 Up;
    alg::Vec3 Right;
    alg::Vec3 WorldUp;

    // Ângulos de Euler
    float Yaw;
    float Pitch;

    // Opções da câmera
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    // Construtor
    Camera(alg::Vec3 position = alg::Vec3(0.0f, 0.0f, 0.0f), alg::Vec3 up = alg::Vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);

    // Retorna a matriz de visualização
    alg::Mat4 getViewMatrix();

    // Processa input do teclado
    void processKeyboard(Camera_Movement direction, float deltaTime);

    // Processa input do mouse
    void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);

    // Processa input do scroll do mouse
    void processMouseScroll(float yoffset);

    void updateCameraVectors();
};

#endif //CAMERA_HPP