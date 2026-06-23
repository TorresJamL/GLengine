#pragma once
#define GLM_ENABLE_EXPERIMENTAL

#include <sstream>
#include <iostream>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/string_cast.hpp>

#include "utils.hpp"
#include "Shader.hpp"

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum CameraDirection { FORWARD, BACKWARD, LEFT, RIGHT, ASCEND, DESCEND, SPEEDUP, SPEEDDOWN };

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float ACC_SPEED = 7.5f;
const float SENSITIVITY = 100.0f;
const float ZOOM = 45.0f;

class Camera {
public:
    glm::vec3 position;
    glm::vec3 orientation;
    glm::mat4 projection = glm::mat4(1.0f);
    glm::mat4 view;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

    float yaw;
    float pitch;

    float speed;
    float mouseSensitivity;
    float zoom;

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH): 
        orientation(glm::vec3(0.0f, 0.0f, -1.0f)), 
        speed(SPEED), 
        mouseSensitivity(SENSITIVITY), 
        zoom(ZOOM){
            this->position = position;
            this->up = up;
            this->yaw = yaw;
            this->pitch = pitch;
            right = glm::normalize(glm::cross(this->orientation, worldUp));
    }

    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch): 
        orientation(glm::vec3(0.0f, 0.0f, -1.0f)), speed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM) {
        position = glm::vec3(posX, posY, posZ);
        worldUp = glm::vec3(upX, upY, upZ);
        this->yaw = yaw;
        this->pitch = pitch;
    }

    void Matrix(float FOVdeg, float aspect, float nearPlane, float farPlane, Shader& shader) {
        projection = glm::mat4(1.0f);

        view = glm::lookAt(position, position + orientation, up);
	    projection = glm::perspective(glm::radians(FOVdeg), aspect, nearPlane, farPlane);

        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
    }

    void ProcessKeyboard(CameraDirection direction, float deltaTime) {
        float velocity = speed * deltaTime;
        switch (direction) {
            case FORWARD: {
                position += orientation * velocity;
                break;
            }
            case BACKWARD: {
                position -= orientation * velocity;
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
            case ASCEND: {
                position += worldUp * velocity;
                break;
            }
            case DESCEND: {
                position -= worldUp * velocity;
                break;
            } 
            case SPEEDUP: {
                speed = ACC_SPEED;
                break;
            } 
            case SPEEDDOWN: {
                speed = SPEED;
                break;
            }
            default: throw NotImplemented();
        }
        updateVectors();
    }

    void ProcessMouse(GLFWwindow* window, float deltaTime) {
        // mouse + scroll wheel
        // Handles mouse inputs
        int width, height;
        glfwGetWindowSize(window, &width, &height);

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
			// Hides mouse cursor
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

			// Prevents camera from jumping on the first click
			if (firstClick) {
				glfwSetCursorPos(window, (double)(width / 2), (double)(height / 2));
				firstClick = false;
			}

            double mouseX;
			double mouseY;

            glfwGetCursorPos(window, &mouseX, &mouseY);

			float rotX = mouseSensitivity * (float)(mouseY - (height / 2)) / height;
			float rotY = mouseSensitivity * (float)(mouseX - (width / 2)) / width;

			glm::vec3 newOrientation = glm::rotate(orientation, glm::radians(-rotX), glm::normalize(glm::cross(orientation, up)));

			if (abs(glm::angle(newOrientation, up) - glm::radians(90.0f)) <= glm::radians(85.0f)) {
				orientation = newOrientation;
			}
			orientation = glm::rotate(orientation, glm::radians(-rotY), up);

			glfwSetCursorPos(window, (double)(width / 2), (double)(height / 2));
		}
		else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
			// Unhides cursor since camera is not looking around anymore
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			// Makes sure the next time the camera looks around it doesn't jump
			firstClick = true;
		}  
        // updateVectors();
    }

    static std::string to_string(Camera c) {
        std::stringstream ss("");
        ss << "---------------------------------------------------------------------\n";
        ss << "Position: " << glm::to_string(c.position) << "\n";
        ss << "Orientation: " << glm::to_string(c.orientation) << "\n";
        ss << "View: " << glm::to_string(c.view) << "\n";
        ss << "Projection: " << glm::to_string(c.projection) << "\n";
        ss << "Relative Up: " << glm::to_string(c.up) << "\n";
        ss << "Relative Right: " << glm::to_string(c.right); 
        std::string s = ss.str(); 
        return s;
    }

private:
    bool firstClick = true;

    void updateVectors() {
        right = glm::normalize(glm::cross(this->orientation, worldUp));  
        // up = glm::normalize(glm::cross(right, this->orientation));
    }
};