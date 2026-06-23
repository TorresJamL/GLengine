#define GLM_ENABLE_EXPERIMENTAL

#include <iostream>

#include <glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include <stb_image.h>

#include "VertexArray.hpp"
#include "ElementBuffer.hpp"
#include "VertexBuffer.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "../includes/Texture.hpp" // There's definitely a better solution to this but ehh

using namespace std;

// GLOBALS
float width = 800.0f;
float height = 600.0f;
float aspect = width / height;
bool firstMouse = true;
float fov = 45.0f;
float lastX = 400, lastY = 300;
float yaw = -90.0f;
float pitch = 0.0f;
// camera shenanigans
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
Camera cam;
// time
double deltaTime = 0.0;
double lastFrame = 0.0;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    aspect = (float)width / (float)height;
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) 
        glfwSetWindowShouldClose(window, true);
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cam.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cam.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cam.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cam.ProcessKeyboard(RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        cam.ProcessKeyboard(ASCEND, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        cam.ProcessKeyboard(DESCEND, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        cam.ProcessKeyboard(SPEEDUP, deltaTime);
    } else {
        cam.ProcessKeyboard(SPEEDDOWN, deltaTime);
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    // if (firstMouse) {
    //     lastX = xpos;
    //     lastY = ypos;
    //     firstMouse = false;
    // }
  
    // float xoffset = xpos - lastX;
    // float yoffset = lastY - ypos; 
    // lastX = xpos;
    // lastY = ypos;

    // float sensitivity = 0.1f;
    // xoffset *= sensitivity;
    // yoffset *= sensitivity;

    // yaw   += xoffset;
    // pitch += yoffset;

    // if (pitch > 89.0f)
    //     pitch = 89.0f;
    // if (pitch < -89.0f)
    //     pitch = -89.0f;

    // glm::vec3 direction;
    // direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    // direction.y = sin(glm::radians(pitch));
    // direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    // cameraFront = glm::normalize(direction);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}

int main(){
    cout << "Hello, World!" << endl;
    
    GLFWwindow* window;

    if (!glfwInit()){
        cout << "GLFW failed to start." << endl;
        return -1;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    window = glfwCreateWindow(800, 600, "My Window", NULL, NULL);
    if (window == NULL){
        cout << "Failed to create window." << endl;
        glfwTerminate();
        return -1;   
    }
    
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){ 
        cout << "GLAD failed to initialize." << endl;
        return -1;
    }
    
    glViewport(0, 0, 800, 600);

    // Vertex stuff
    // float vertices[] = { 
    //     // pos              // colors           // tex coords
    //      0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
    //      0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
    //     -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
    //     -0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
    // }; 
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f), 
        glm::vec3( 2.0f,  5.0f, -15.0f), 
        glm::vec3(-1.5f, -2.2f, -2.5f),  
        glm::vec3(-3.8f, -2.0f, -12.3f),  
        glm::vec3( 2.4f, -0.4f, -3.5f),  
        glm::vec3(-1.7f,  3.0f, -7.5f),  
        glm::vec3( 1.3f, -2.0f, -2.5f),  
        glm::vec3( 1.5f,  2.0f, -2.5f), 
        glm::vec3( 1.5f,  0.2f, -1.5f), 
        glm::vec3(-1.3f,  1.0f, -1.5f)    
    };

    GLuint indicies[] = {
        0, 1, 3, // Tri 1
        1, 2, 3  // Tri 2
    };

    //** Light cube
    glm::vec3 lightCubePosition = glm::vec3(0.0f, 0.0f, 6.0f); // 3f backwards from camera
    float lightCubeVerticies[] = {
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f
    };

    //* Light Cube VAO
    VAO lightVao;
    lightVao.Unbind();

    // EBO ebo(indicies, sizeof(indicies));
    VBO vbo(vertices, sizeof(vertices));
    VAO vao;
    
    vao.Bind();

    // ebo.Bind();

    vbo.Bind();
    vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, 5 * sizeof(float), (void*)0); 
    vao.LinkAttrib(vbo, 1, 2, GL_FLOAT, 5 * sizeof(float), (void*)(3 * sizeof(float))); 
    vao.Unbind();
    
    lightVao.Bind();
    lightVao.LinkAttrib(vbo, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
    lightVao.Unbind();

    vbo.Unbind();
    // ebo.Unbind();
    
    Texture texture("../../resources/assets/me.png", GL_TEXTURE_2D);
    texture.setTexParamInt(GL_TEXTURE_WRAP_S, GL_REPEAT);
    texture.setTexParamInt(GL_TEXTURE_WRAP_T, GL_REPEAT);
    texture.setTexParamInt(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    texture.setTexParamInt(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    texture.Free();

    Shader shdr("../../resources/shaders/default.vert", "../../resources/shaders/default.frag");
    Shader lightingShdr();

    glEnable(GL_DEPTH_TEST);
    
    shdr.Use();
    shdr.setInt("ourTex", 0);
    
    glm::mat4 view;
    
    const float radius = 10.0f;
    
	double currentFrame = 0.0;
    
    double lastTime = glfwGetTime();
    int nbFrames = 0;
    
    cout << "Beginning render loop." << endl;
    while (!glfwWindowShouldClose(window)){
        currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
        // input
        processInput(window);
        
        nbFrames++;
        if (currentFrame - lastTime >= 1.0) {
            printf("%f ms/frame\n", 1000.0/double(nbFrames));
            printf("%f fps\n", double(nbFrames));
            nbFrames = 0;
            lastTime += 1.0;
        }
        // rendering commands here
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        // glBindTexture(GL_TEXTURE_2D, texture1);

        shdr.Use();

        cam.ProcessMouse(window, deltaTime);
        cam.Matrix(fov, aspect, 0.1f, 100.0f, shdr);

        vao.Bind(); 
        for (unsigned int i = 0; i < 10; i++) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20 * i;
            if (i % 3 == 0) {
                model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            } else model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            shdr.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Event / buffer handling here
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    cout << "The End!" << endl;
    
    // ebo.Delete();
    vao.Delete();
    vbo.Delete();
    shdr.Delete();
    return 0;
}