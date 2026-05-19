#include <iostream>

#include <glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stb_image.h>

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
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
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

    vbo.Bind();
    vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0); 
    vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float))); 
    vao.LinkAttrib(vbo, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float))); 

    // vbo.Unbind();
    // vao.Unbind();
    // ebo.Unbind();

    // GLM shenanigans
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
    trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5)); 
    // End of GLM 
    
    unsigned int texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    
    // Texture shenanigans
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    int width, height, nrChannels; // nrChannels is number of color channels
    stbi_set_flip_vertically_on_load(true); // stbi load flipped bcuz of gl coords
    unsigned char *data = stbi_load("../../resources/assets/me.png", &width, &height, &nrChannels, 0);
    if(data){
        GLenum format;
        if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        cerr << "Failed to load texture." << endl;
        exit(EXIT_FAILURE);
    }
    stbi_image_free(data);

    Shader shdr("../../resources/shaders/default.vert", "../../resources/shaders/default.frag");
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    cout << "Beginning render loop." << endl;
    glEnable(GL_DEPTH_TEST);

    shdr.Use();
    glUniform1i(glGetUniformLocation(shdr.ID, "ourTex"), 0);
    unsigned int transformLoc = glGetUniformLocation(shdr.ID, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

    while (!glfwWindowShouldClose(window)){
        // input
        processInput(window);

        // rendering commands here
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        shdr.Use();
        
        // continous rotation
        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
        trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

        glUniform1i(glGetUniformLocation(shdr.ID, "ourTex"), 0);
        unsigned int transformLoc = glGetUniformLocation(shdr.ID, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
        // continous rotation

        shdr.ApplyAspectRatio(aspect);
        vao.Bind();
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