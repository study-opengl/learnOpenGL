//
//  main.cpp
//  texture
//
//  Created by Ashoka on 2018/4/30.
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

int createHelloTriangleWindow();
void frameBufferSizeCallback(GLFWwindow *window, int width, int height);
void progressInput(GLFWwindow *window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const float fps = 70.0;

float mixValue = 0.2f;

glm::vec3 cameraPos = glm::vec3(0, 0, 3);
glm::vec3 cameraFront = glm::vec3(0, 0, -1);
glm::vec3 cameraUp = glm::vec3(0, 1, 0);

float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main(int argc, const char *argv[])
{
    // insert code here...
    std::cout << "Hello, World!\n";
    createHelloTriangleWindow();
    return 0;
}

unsigned int vaoGenerate(float *vertices, unsigned int vertexCount)
{
    // 顶点数组对象
    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // 顶点缓冲对象
    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(float), vertices, GL_STATIC_DRAW);

    // 告诉OpenGL该如何解析顶点数据
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // 第二个属性
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // 此时可以相关数据已经存到vao中，可以解绑vbo和vao
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glDeleteBuffers(1, &vbo);

    return vao;
}

unsigned int textureGenarate(const char *imagePath)
{
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height, nrChannels;
    // OpenGL要求y轴0.0坐标是在图片的底部的，但是图片的y轴0.0坐标通常在顶部。很幸运，stb_image.h能够在图像加载时帮助我们翻转y轴，只需要在加载任何图像前加入以下语句即可：
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(imagePath, &width, &height, &nrChannels, 0);
    asLog("%s width: %d, height: %d, channels: %d", imagePath, width, height, nrChannels);
    if (data) {
        GLenum format = GL_RGB;
        // 当channel == 4 时，说明有alpha通道， == 3 时，只有rgb通道
        if (nrChannels == 4) {
            format = GL_RGBA;
        }
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        asLog("can't load image at path: %s", imagePath);
    }
    stbi_image_free(data);
    return texture;
}

int createHelloTriangleWindow()
{
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
    ShaderProgram shaderProgram = ShaderProgram("camera.vs", "camera.fs");

    //    float vertices[] = {
    //        //     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
    //        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
    //        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
    //        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
    //        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
    //    };
    float vertices[] = {
        -0.5f, -0.5f,  -0.5f,   0.0f,    0.0f,
        0.5f,  -0.5f,  -0.5f,   1.0f,    0.0f,
        0.5f,  0.5f,   -0.5f,   1.0f,    1.0f,
        0.5f,  0.5f,   -0.5f,   1.0f,    1.0f,
        -0.5f, 0.5f,   -0.5f,   0.0f,    1.0f,
        -0.5f, -0.5f,  -0.5f,   0.0f,    0.0f,

        -0.5f, -0.5f,  0.5f,    0.0f,    0.0f,
        0.5f,  -0.5f,  0.5f,    1.0f,    0.0f,
        0.5f,  0.5f,   0.5f,    1.0f,    1.0f,
        0.5f,  0.5f,   0.5f,    1.0f,    1.0f,
        -0.5f, 0.5f,   0.5f,    0.0f,    1.0f,
        -0.5f, -0.5f,  0.5f,    0.0f,    0.0f,

        -0.5f, 0.5f,   0.5f,    1.0f,    0.0f,
        -0.5f, 0.5f,   -0.5f,   1.0f,    1.0f,
        -0.5f, -0.5f,  -0.5f,   0.0f,    1.0f,
        -0.5f, -0.5f,  -0.5f,   0.0f,    1.0f,
        -0.5f, -0.5f,  0.5f,    0.0f,    0.0f,
        -0.5f, 0.5f,   0.5f,    1.0f,    0.0f,

        0.5f,  0.5f,   0.5f,    1.0f,    0.0f,
        0.5f,  0.5f,   -0.5f,   1.0f,    1.0f,
        0.5f,  -0.5f,  -0.5f,   0.0f,    1.0f,
        0.5f,  -0.5f,  -0.5f,   0.0f,    1.0f,
        0.5f,  -0.5f,  0.5f,    0.0f,    0.0f,
        0.5f,  0.5f,   0.5f,    1.0f,    0.0f,

        -0.5f, -0.5f,  -0.5f,   0.0f,    1.0f,
        0.5f,  -0.5f,  -0.5f,   1.0f,    1.0f,
        0.5f,  -0.5f,  0.5f,    1.0f,    0.0f,
        0.5f,  -0.5f,  0.5f,    1.0f,    0.0f,
        -0.5f, -0.5f,  0.5f,    0.0f,    0.0f,
        -0.5f, -0.5f,  -0.5f,   0.0f,    1.0f,

        -0.5f, 0.5f,   -0.5f,   0.0f,    1.0f,
        0.5f,  0.5f,   -0.5f,   1.0f,    1.0f,
        0.5f,  0.5f,   0.5f,    1.0f,    0.0f,
        0.5f,  0.5f,   0.5f,    1.0f,    0.0f,
        -0.5f, 0.5f,   0.5f,    0.0f,    0.0f,
        -0.5f, 0.5f,   -0.5f,   0.0f,    1.0f
    };

    asLog("vertices size: %d", sizeof(vertices));
    // 顶点数组对象
    unsigned int vao = vaoGenerate(vertices, sizeof(vertices) / sizeof(float));

    unsigned int texture1 = textureGenarate("container.jpg");
    unsigned int texture2 = textureGenarate("awesomeface.png");

    // uncomment this call to draw in wireframe polygons. 线条模式
    //    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // 激活着色器程序
    shaderProgram.use();
    shaderProgram.setInt("texture1", 0);
    shaderProgram.setInt("texture2", 1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    //    glm::mat4 trans;
    //    trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
    //    trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
    //
    //    unsigned int transformLoc = glGetUniformLocation(shaderProgram.ID, "transform");
    //    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

    glBindVertexArray(vao);

    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  0.0f,   0.0f),
        glm::vec3(2.0f,  5.0f,   -15.0f),
        glm::vec3(-1.5f, -2.2f,  -2.5f),
        glm::vec3(-3.8f, -2.0f,  -12.3f),
        glm::vec3(2.4f,  -0.4f,  -3.5f),
        glm::vec3(-1.7f, 3.0f,   -7.5f),
        glm::vec3(1.3f,  -2.0f,  -2.5f),
        glm::vec3(1.5f,  2.0f,   -2.5f),
        glm::vec3(1.5f,  0.2f,   -1.5f),
        glm::vec3(-1.3f, 1.0f,   -1.5f)
    };
    float expectedFrameTime = 1 / fps;
    // 当帧数过高时，usleep(diffTime * 1000000) 来降低帧数
    float diffTime = 0.0;
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        if (deltaTime < expectedFrameTime) {
            diffTime = expectedFrameTime - deltaTime;
            usleep(diffTime * 1000000);
            deltaTime += diffTime;
            asLog("sleep:%f", diffTime);
        }
        lastFrame = currentFrame;
        asLog("deltaTime:%f", deltaTime);

        progressInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // 开启深度测试
        glEnable(GL_DEPTH_TEST);

        shaderProgram.setFloat("mixValue", mixValue);

        glm::mat4 view;
        glm::mat4 projection;

        // 创建一个 lookAt 观察矩阵
//        float radius = 10.0f;
        // camera 随时间转圈
//        float camX = sin((float)glfwGetTime()) * radius;
//        float camZ = cos((float)glfwGetTime()) * radius;
//        view = glm::lookAt(glm::vec3(camX, 0, camZ), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        // 投影矩阵
        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH  / (float)SCR_HEIGHT, 0.1f, 100.0f);

        shaderProgram.setMatrix4fv("view", view);
        shaderProgram.setMatrix4fv("projection", projection);

        for (unsigned int i = 0; i < sizeof(cubePositions) / sizeof(glm::vec3); i += 1) {
            glm::mat4 model;
            model = glm::translate(model, cubePositions[i]);
            float angle = i == 0 ? 1 : glm::radians(20.0f * i);
            if (i % 3 == 0) {
                angle = (float)glfwGetTime() * angle;
            }
            model = glm::rotate(model, angle, glm::vec3(1.5, 0.3, 0.5));
            shaderProgram.setMatrix4fv("model", model);
            // 画矩形
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    glDeleteVertexArrays(1, &vao);

    glfwTerminate();
    return 0;
}

void frameBufferSizeCallback(GLFWwindow *window, int width, int height)
{
    asLog("current window width: %d, height: %d", width, height);
    glViewport(0, 0, width, height);
}

void progressInput(GLFWwindow *window)
{
    float cameraSpeed = 3.0f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        asLog("escape pressed");
        glfwSetWindowShouldClose(window, true);
    } else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        mixValue += 0.01;
        mixValue = mixValue > 1.0f ? 1.0f : mixValue;
        asLog("mix value: %f", mixValue);
    } else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        mixValue -= 0.01;
        mixValue = mixValue < 0.0f ? 0.0f : mixValue;
        asLog("mix value: %f", mixValue);
    } else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cameraPos += cameraSpeed * cameraFront;
    } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cameraPos -= cameraSpeed * cameraFront;
    } else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    } else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
}
