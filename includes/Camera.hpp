#pragma once
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "utils.hpp"

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum CameraDirection { FORWARD, BACKWARD, LEFT, RIGHT };

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera {
public:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    float yaw;
    float pitch;

    float speed;
    float mouseSensitivity;
    float zoom;

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH): 
        front(glm::vec3(0.0f, 0.0f, -1.0f)), 
        speed(SPEED), 
        mouseSensitivity(SENSITIVITY), 
        zoom(ZOOM){
            this->position = position;
            this->up = up;
            this->yaw = yaw;
            this->pitch = pitch;
    }

    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch): 
        front(glm::vec3(0.0f, 0.0f, -1.0f)), speed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM) {
        position = glm::vec3(posX, posY, posZ);
        worldUp = glm::vec3(upX, upY, upZ);
        this->yaw = yaw;
        this->pitch = pitch;
        updateVectors();
    }

    void ProcessKeyboard(CameraDirection direction, float deltaTime) {
        float velocity = speed * deltaTime;
        switch (direction) {
            case FORWARD: {
                position += front * velocity;
                break;
            }
            case BACKWARD: {
                position -= front * velocity;
                break;
            }
            case RIGHT: {
                position += right * velocity;
                break;
            }
            case LEFT: {
                position -= right * velocity;
                break;
            }
            default: throw NotImplemented();
        }
    }

    void ProcessMouse() {

    }
private:
    void updateVectors() {
        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        this->front = glm::normalize(front);

        right = glm::normalize(glm::cross(this->front, worldUp));  
        up = glm::normalize(glm::cross(right, this->front));
    }
};