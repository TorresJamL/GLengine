#include <iostream>

#include <glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "VertexArray.hpp"
#include "ElementBuffer.hpp"
#include "VertexBuffer.hpp"
#include "Shader.hpp"

using namespace std;

// GLOBALS
float width = 800.0f;
float height = 600.0f;
float aspect = width / height;

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
    aspect = (float)width / (float)height;
}

void processInput(GLFWwindow *window){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) 
        glfwSetWindowShouldClose(window, true);
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
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){ 
        cout << "GLAD failed to initialize." << endl;
        return -1;
    }
    
    glViewport(0, 0, 800, 600);

    // Vertex stuff
    float vertices[] = { 
        // pos              // colors           // tex coords
         0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
         0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f,
        -0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 0.0f,  0.0f, 0.0f,
    }; 

    GLuint indicies[] = {
        0, 1, 3, // Tri 1
        1, 2, 3  // Tri 2
    };

    // Element Buffer Object
    EBO ebo(indicies, sizeof(indicies));
    VBO vbo(vertices, sizeof(vertices));
    VAO vao;
    
    vao.Bind();

    ebo.Bind();
    vao.LinkAttrib(vbo, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float))); 

    vbo.Unbind();
    vao.Unbind();
    ebo.Unbind();

    // Texture shenanigans
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    Shader shdr("../../resources/shaders/default.vert", "../../resources/shaders/default.frag");

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    {// demo camera work
        glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f); // +z goes outta screen torward us
        glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);

        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));

        glm::vec3 cameraUp = glm::normalize(glm::cross(cameraDirection, cameraRight));

        glm::mat4 view = glm::lookAt(
            cameraPos,
            cameraTarget,
            up
        );

        const float radius = 10.0f;
    }

    cout << "Beginning render loop." << endl;
    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window)){
        // input
        processInput(window);

        // rendering commands here
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shdr.Use();
        shdr.ApplyAspectRatio(aspect);
        vao.Bind();

        // float camX = sin(glfwGetTime()) * radius;
        // float camZ = cos(glfwGetTime()) * radius;

        // glm::mat4 view = glm::lookAt(
        //     glm::vec3(camX, 0.0f, camZ),
        //     cameraTarget,
        //     up
        // );

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Event / buffer handling here
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    cout << "The End!" << endl;
    
    ebo.Delete();
    vao.Delete();
    vbo.Delete();
    shdr.Delete();
    return 0;
}