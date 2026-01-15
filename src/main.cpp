#include <iostream>
#include <vector> 

#include "config.h"

using namespace std;

int main(){
    cout << "Hello, World!" << endl;
    
    GLFWwindow* window;

    if (!glfwInit()){
        cout << "GLFW failed to start." << endl;
        return -1;
    }

    window = glfwCreateWindow(640, 480, "My Window", NULL, NULL);

    return 0;
}