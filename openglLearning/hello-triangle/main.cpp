//
//  main.cpp
//  hello-triangle
//
//  Created by Ashoka on 2018/4/26.
//

#include <iostream>
#include "asLog.h"
//#include <OpenGL/OpenGL.h>
#include "glad.h"
#include "glfw3.h"

int createHelloTriangleWindow();
unsigned int shaderProgramGenerate(char **vertexShaderSource, char **fragmentShaderSource);
void frameBufferSizeCallback(GLFWwindow* window, int width, int height);
void progressInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

const char *yellowFragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.59f, 0.23f, 0.76f, 1.0f);\n"
"}\n\0";

int main(int argc, const char * argv[]) {
    // insert code here...
    int exitValue = createHelloTriangleWindow();
    return exitValue;
}

unsigned int shaderProgramGenerate(const char *const*vertexShaderSource, const char *const*fragmentShaderSource) {
    
    // 编译顶点着色器
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    
    ///=============================================================================
    /// @name 编译片段着色器
    ///=============================================================================
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    
    ///=============================================================================
    /// @name 着色器程序
    ///=============================================================================
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    // 已经不需要着色器对象了
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    return shaderProgram;
    
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    
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
    
    ///=============================================================================
    /// @name 着色器程序
    ///=============================================================================
    unsigned int shaderProgram = shaderProgramGenerate(&vertexShaderSource, &fragmentShaderSource);
    unsigned int shaderProgram2 = shaderProgramGenerate(&vertexShaderSource, &yellowFragmentShaderSource);
    
//    float vertices[] = {
//        -0.5f, -0.5f, 0.0f,
//        0.5f, -0.5f, 0.0f,
//        0.0f,  0.5f, 0.0f
//    };
    float vertices[] = {
        0.5f, 0.5f, 0.0f,   // 右上角
        0.5f, -0.5f, 0.0f,  // 右下角
        -0.5f, -0.5f, 0.0f, // 左下角
        -0.5f, 0.5f, 0.0f   // 左上角
    };
    
    unsigned int indices[] = { // 注意索引从0开始!
        0, 1, 3, // 第一个三角形
        1, 2, 3  // 第二个三角形
    };
    asLog("vertices size: %d", sizeof(vertices));
    // 顶点数组对象
    unsigned int vao = vaoGenerate(vertices, sizeof(vertices) / sizeof(float), indices, sizeof(indices) / sizeof(unsigned int));
    
    unsigned int indices2[] = { // 注意索引从0开始!
        1, 2, 3  // 第二个三角形
    };
    // 顶点数组对象
    unsigned int vao2 = vaoGenerate(vertices, sizeof(vertices) / sizeof(float), indices2, sizeof(indices2) / sizeof(unsigned int));
    
    // uncomment this call to draw in wireframe polygons. 线条模式
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    while (!glfwWindowShouldClose(window)) {
        progressInput(window);
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        // 激活着色器程序
        glUseProgram(shaderProgram);
        glBindVertexArray(vao);
        // 画三角形
//        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        
        glUseProgram(shaderProgram2);
        glBindVertexArray(vao2);
        // 画三角形
//        glDrawArrays(GL_TRIANGLES, 1, 3);
        // 画矩形
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    glDeleteVertexArrays(1, &vao);
//    glDeleteBuffers(1, &vbo);
//    glDeleteBuffers(1, &ebo);
    glDeleteVertexArrays(1, &vao2);
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
