//
//  main.cpp
//  timing
//
//  Created by Ashoka on 2018/4/29.
//

#include <iostream>
#include "ShaderProgram.hpp"
#include "glfw3.h"
#include "asLog.h"
#include <unistd.h>
#include <math.h>

int createHelloTriangleWindow();
void frameBufferSizeCallback(GLFWwindow* window, int width, int height);
void progressInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    createHelloTriangleWindow();
    return 0;
}

unsigned int vaoGenerate(float *vertices, unsigned int vertexCount, unsigned int *indices, unsigned int indexCount) {
    // 顶点数组对象
    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    // 顶点缓冲对象
    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(float), vertices, GL_STATIC_DRAW);
    
    // 索引缓冲对象
    unsigned int ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);
    
    // 告诉OpenGL该如何解析顶点数据
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    
    // 第二个属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    // 此时可以相关数据已经存到vao中，可以解绑vbo和vao
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    
    return vao;
}

int createHelloTriangleWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Hello Triangle", NULL, NULL);
    if (window == NULL) {
        asLog("Failed to create glfw window");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        asLog("failed to initialize glad");
        glfwTerminate();
        return -1;
    }
    char buffer[1024];
    getcwd(buffer, 1024);
    printf("The current directory is: %s", buffer);
    
    ///=============================================================================
    /// @name 着色器程序
    ///=============================================================================
//    ShaderProgram shaderProgram = ShaderProgram("timing.vs", "timing.fs");
    ShaderProgram shaderProgram = ShaderProgram("vertexWithColor.vs", "vertexWithColor.fs");
    
    //    float vertices[] = {
    //        -0.5f, -0.5f, 0.0f,
    //        0.5f, -0.5f, 0.0f,
    //        0.0f,  0.5f, 0.0f
    //    };
//    float vertices[] = {
//        0.5f, 0.5f, 0.0f,   // 右上角
//        0.5f, -0.5f, 0.0f,  // 右下角
//        -0.5f, -0.5f, 0.0f, // 左下角
//        -0.5f, 0.5f, 0.0f,   // 左上角
//        0.0f, 0.5f, 0.0f
//    };
    float vertices[] = {
        // 位置              // 颜色
        0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
        0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 顶部
    };
    
    unsigned int indices[] = { // 注意索引从0开始!
        0, 1, 2
    };
    asLog("vertices size: %d", sizeof(vertices));
    // 顶点数组对象
    unsigned int vao = vaoGenerate(vertices, sizeof(vertices) / sizeof(float), indices, sizeof(indices) / sizeof(unsigned int));
    
    // uncomment this call to draw in wireframe polygons. 线条模式
    //    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    while (!glfwWindowShouldClose(window)) {
        progressInput(window);
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        // 激活着色器程序
        shaderProgram.use();
        
        // 更新uniform颜色
//        float timeValue = glfwGetTime();
//        float greenValue = sin(timeValue) / 2 + 0.5;
//        shaderProgram.set4Float("ourColor", 0, greenValue, 0, 1.0);
//        shaderProgram.setFloat("offset", 0.5);
        
        glBindVertexArray(vao);
        // 画三角形
        //        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        
        
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    glDeleteVertexArrays(1, &vao);
    //    glDeleteBuffers(1, &vbo);
    //    glDeleteBuffers(1, &ebo);
    //    glDeleteBuffers(1, &vbo2);
    //    glDeleteBuffers(1, &ebo2);
    
    glfwTerminate();
    return 0;
}

void frameBufferSizeCallback(GLFWwindow* window, int width, int height) {
    asLog("current window width: %d, height: %d", width, height);
    glViewport(0, 0, width, height);
}

void progressInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        asLog("escape pressed");
        glfwSetWindowShouldClose(window, true);
    }
}
