//
//  main.cpp
//  light
//
//  Created by ashoka on 2019/3/19.
//

#include <iostream>
#include "ShaderProgram.hpp"
#include "glfw3.h"
#include "asLog.h"
#include <unistd.h>
#include <math.h>
#include "stb_image.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "Camera.hpp"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float screenWidth = SCR_WIDTH;
float screenHeight = SCR_HEIGHT;
GLFWwindow *window;
Camera camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));
ShaderProgram cubeShader = ShaderProgram("cube.vs", "camera.fs");
ShaderProgram lightShader = ShaderProgram("cube.vs", "light.fs");

float vertices[] = {
    -0.5f, -0.5f,  -0.5f,
    0.5f,  -0.5f,  -0.5f,
    0.5f,  0.5f,   -0.5f,
    0.5f,  0.5f,   -0.5f,
    -0.5f, 0.5f,   -0.5f,
    -0.5f, -0.5f,  -0.5f,
    
    -0.5f, -0.5f,  0.5f,
    0.5f,  -0.5f,  0.5f,
    0.5f,  0.5f,   0.5f,
    0.5f,  0.5f,   0.5f,
    -0.5f, 0.5f,   0.5f,
    -0.5f, -0.5f,  0.5f,
    
    -0.5f, 0.5f,   0.5f,
    -0.5f, 0.5f,   -0.5f,
    -0.5f, -0.5f,  -0.5f,
    -0.5f, -0.5f,  -0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f, 0.5f,   0.5f,
    
    0.5f,  0.5f,   0.5f,
    0.5f,  0.5f,   -0.5f,
    0.5f,  -0.5f,  -0.5f,
    0.5f,  -0.5f,  -0.5f,
    0.5f,  -0.5f,  0.5f,
    0.5f,  0.5f,   0.5f,
    
    -0.5f, -0.5f,  -0.5f,
    0.5f,  -0.5f,  -0.5f,
    0.5f,  -0.5f,  0.5f,
    0.5f,  -0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f,  -0.5f,
    
    -0.5f, 0.5f,   -0.5f,
    0.5f,  0.5f,   -0.5f,
    0.5f,  0.5f,   0.5f,
    0.5f,  0.5f,   0.5f,
    -0.5f, 0.5f,   0.5f,
    -0.5f, 0.5f,   -0.5f
};

int createWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    GLFWwindow *window = glfwCreateWindow(screenWidth, screenHeight, "Light", NULL, NULL);
    if (window == NULL) {
        asLog("failed to create window");
        glfwTerminate();
        return -1;
    }
    return 1;
}
void frameBufferSizeCallback(GLFWwindow *window, int width, int height) {
    
}
void mouseMoveCallback(GLFWwindow *window, double xpos, double ypos) {
    
}
void scrollCallback(GLFWwindow *window, double xpos, double ypos) {
    
}
void setupWindow() {
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouseMoveCallback);
    glfwSetScrollCallback(window, scrollCallback);
}

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    createWindow();
    setupWindow();
    return 0;
}


