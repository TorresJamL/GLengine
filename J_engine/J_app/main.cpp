#define GLM_ENABLE_EXPERIMENTAL

#include <iostream>
#include <ctime>

#include <glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "../J_renderer/includes/Renderer.h"

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

// time based on cpu 
double cDeltaTime = 0.0;
double lastCFrame = 0.0;

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
    cout << "Window Created Successfully." << endl;
    
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){ 
        cout << "GLAD failed to initialize." << endl;
        return -1;
    }
    cout << "GLAD initialized" << endl;
    
    glViewport(0, 0, 800, 600);

    using namespace G_Framework;
    vector<Cube> cubes = {};
    cubes.push_back(Cube(glm::vec3(0, 0, -3)));

    cout << "Cubes Made." << endl;

    Cube lightCube(glm::vec3(0.0f, 2.0f, 6.0f), glm::vec3(1.0f));
    cout << "Light Cube Made." << endl;

    //* Light Cube VAO
    VAO lightVao;
    lightVao.Unbind();

    // EBO ebo(indicies, sizeof(indicies));
    VBO vbo(Cube::cubeVerticesData);
    VAO vao;
    
    vao.Bind();

    // ebo.Bind();
    
    vbo.Bind();
    vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0); 
    vao.LinkAttrib(vbo, 1, 2, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float))); 
    vao.LinkAttrib(vbo, 2, 3, GL_FLOAT, 8 * sizeof(float), (void*)(5 * sizeof(float))); 
    vao.Unbind();
    
    lightVao.Bind();
    lightVao.LinkAttrib(vbo, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    lightVao.Unbind();

    vbo.Unbind();
    // ebo.Unbind();
    cout << "Vertex Array/Buffer Objects Setup" << endl;
    
    fs::path sourcePath = fs::absolute(__FILE__);
    fs::path sourceDir = sourcePath.parent_path();
    fs::current_path(sourceDir);

    Texture texture;
    try {
        texture.create2DTexture("../J_renderer/resources/assets/me.png");
        texture.setTexParamInt(GL_TEXTURE_WRAP_S, GL_REPEAT);
        texture.setTexParamInt(GL_TEXTURE_WRAP_T, GL_REPEAT);
        texture.setTexParamInt(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        texture.setTexParamInt(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        texture.Free();
        cout << "Textures Defined." << endl;
    } catch (const exception& err) {
        cerr << "Error: " << err.what() << endl;
        exit(1);
    }

    // Texture cubemap_tex;
    // try {
    //     cubemap_tex.createCubeMapTexture("../J_renderer/resources/assets/metalboxidk.png");
    //     cubemap_tex.Free();
    // } catch (const exception& err) {
    //     cerr << "Error: " << err.what() << endl;
    //     exit(1);
    // }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    Shader shdr("../J_renderer/resources/shaders/default.vert", "../J_renderer/resources/shaders/default.frag");
    try {
        shdr.Use();
        shdr.setInt("ourTex", 0);
        shdr.setBool("useTex", true);
        shdr.setVec3("light.ambient",  0.2f, 0.2f, 0.2f);
        shdr.setVec3("light.diffuse",  0.5f, 0.5f, 0.5f); 
        shdr.setVec3("light.specular", 1.0f, 1.0f, 1.0f); 
        shdr.setVec3("material.ambient", 1.0f, 1.0f, 1.0f); // Changing this might cause texture coloring issues
        shdr.setVec3("material.diffuse", 1.0f, 1.0f, 1.0f); // Changing this might cause texture coloring issues
        shdr.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
        shdr.setFloat("material.shininess", 32.0f);
        cout << "Normal Shader Setup." << endl;
    } catch (const exception& err) {
        cerr << "Error: " << err.what() << endl;
        exit(1);
    } 
    Shader lightShdr("../J_renderer/resources/shaders/default.vert", "../J_renderer/resources/shaders/light.frag");
    try {
        lightShdr.Use();
        lightShdr.setVec3("objectColor",  1.0f, 1.0f, 1.0f);
        lightShdr.setVec3("lightColor",  1.0f, 1.0f, 1.0f);
        lightShdr.setVec3("viewPos", cam.position);
        cout << "Light Cube Shader Setup." << endl;
    } catch (const exception& err) {
        cerr << "Error: " << err.what() << endl;
        exit(1);
    }

    Shader cubemapShdr("../J_renderer/resources/shaders/cubemap.vert", "../J_renderer/resources/shaders/cubemap.frag");
    try {
        cubemapShdr.Use();
        cubemapShdr.setInt("cubeTex", 0);
        cubemapShdr.setBool("useTex", true);
        cubemapShdr.setVec3("light.ambient",  0.2f, 0.2f, 0.2f);
        cubemapShdr.setVec3("light.diffuse",  0.5f, 0.5f, 0.5f); 
        cubemapShdr.setVec3("light.specular", 1.0f, 1.0f, 1.0f); 
        cubemapShdr.setVec3("material.ambient", 1.0f, 1.0f, 1.0f); // Changing this might cause texture coloring issues
        cubemapShdr.setVec3("material.diffuse", 1.0f, 1.0f, 1.0f); // Changing this might cause texture coloring issues
        cubemapShdr.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
        cubemapShdr.setFloat("material.shininess", 32.0f);
        cout << "Normal Shader Setup." << endl;
    } catch (const exception& err) {
        cerr << "Error: " << err.what() << endl;
        exit(1);
    } 
    glm::mat4 view;
    
    const float radius = 10.0f;
    
	double currentFrame = 0.0;
	double currentCFrame = 0.0;
    
    double lastTime = glfwGetTime();
    double lastCTime;

    int nbFrames = 0;
    
    float velocity = 3.0f;

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
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0); // TODO: Replace with abstracted call
        // glBindTexture(GL_TEXTURE_2D, texture1); // Texture is pre-binded        

        shdr.Use();

        cam.ProcessMouse(window, deltaTime);
        cam.Matrix(fov, aspect, 0.1f, 100.0f, shdr);

        // if (cubes[0].get_position().z >= 5.0f && velocity > 0) {
        //     velocity *= -1;
        // } else if (cubes[0].get_position().z <= -15.0f && velocity < 0) {
        //     velocity *= -1;
        // } 
        // cubes[0].set_z(cubes[0].get_position().z + (velocity * deltaTime));

        shdr.setVec3("light.position", lightCube.get_position());
        shdr.setVec3("viewPos", cam.position);
        shdr.setMat4("view", cam.view);
        shdr.setMat4("projection", cam.projection);

        vao.Bind(); 
        for (auto cube : cubes) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cube.get_position());
            shdr.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        
        lightShdr.Use();
        lightShdr.setMat4("view", cam.view);
        lightShdr.setMat4("projection", cam.projection);

        lightVao.Bind();

        float lightX = 10.0f * glm::sin(glfwGetTime() + ((velocity - 1) * deltaTime)); 
        float lightZ = 10.0f * glm::cos(glfwGetTime() + ((velocity - 1) * deltaTime));
        lightCube.set_x(lightX);
        lightCube.set_z(lightZ);

        glm::mat4 lightModel = glm::mat4(1.0f);
        lightModel = glm::translate(lightModel, lightCube.get_position());
        lightModel = glm::scale(lightModel, lightCube.get_scale()); 
        lightShdr.setMat4("model", lightModel);
        lightShdr.setVec3("lightPos", lightCube.get_position());
        lightShdr.setVec3("viewPos", cam.position);

        glDrawArrays(GL_TRIANGLES, 0, 36);

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