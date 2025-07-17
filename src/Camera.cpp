#include "Camera.hpp"

Camera::Camera(alg::Vec3 position, alg::Vec3 up, float yaw, float pitch) : Front(alg::Vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM) {
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}

alg::Mat4 Camera::getViewMatrix() {
    return alg::Mat4::lookAt(Position, Position + Front, Up);
}

void Camera::processKeyboard(Camera_Movement direction, float deltaTime) {
    float velocity = MovementSpeed * deltaTime;
    if (direction == FORWARD)
        Position += Front * velocity;
    if (direction == BACKWARD)
        Position -= Front * velocity;
    if (direction == LEFT)
        Position -= Right * velocity;
    if (direction == RIGHT)
        Position += Right * velocity;
}

void Camera::processMouseMovement(float xoffset, float yoffset, bool constrainPitch) {
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    // Para FPS: movimento horizontal do mouse = rotação no eixo Y (Yaw)
    // Para FPS: movimento vertical do mouse = rotação no eixo X (Pitch)
    Yaw   += xoffset;  // Mouse para direita = olhar para direita
    Pitch += yoffset;  // Mouse para cima = olhar para cima

    // Constrain pitch para evitar flip da câmera
    if (constrainPitch) {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }

    updateCameraVectors();
}

void Camera::processMouseScroll(float yoffset) {
    Zoom -= yoffset;
    if (Zoom < 1.0f)
        Zoom = 1.0f;
    if (Zoom > 45.0f)
        Zoom = 45.0f;
}

void Camera::updateCameraVectors() {
    // Calcula o novo vetor Front
    alg::Vec3 front;
    front.x = cos(alg::degrees_to_radians(Yaw)) * cos(alg::degrees_to_radians(Pitch));
    front.y = sin(alg::degrees_to_radians(Pitch));
    front.z = sin(alg::degrees_to_radians(Yaw)) * cos(alg::degrees_to_radians(Pitch));
    Front = front.normalized();
    // Recalcula os vetores Right e Up
    Right = cross(Front, WorldUp).normalized();
    Up    = cross(Right, Front).normalized();
}