//
//  main.cpp
//  texture
//
//  Created by Ashoka on 2018/4/30.
//

#include <iostream>
#include <string>
#include <vector>
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
#include "openGlHelper.hpp"
#include "Model.hpp"

int createHelloTriangleWindow();
void frameBufferSizeCallback(GLFWwindow *window, int width, int height);
void progressInput(GLFWwindow *window);
void mousePositionCallback(GLFWwindow *window, double xpos, double ypos);
void mouseScrollCallback(GLFWwindow *window, double xpos, double ypos);

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

const float fps = 70.0;

float mixValue = 0.2f;

glm::vec3 cameraPos = glm::vec3(0, 0, 3);
glm::vec3 cameraFront = glm::vec3(0, 0, -1);
glm::vec3 cameraUp = glm::vec3(0, 1, 0);

/*
 作为我们摄像机系统的一个附加内容，我们还会来实现一个缩放(Zoom)接口。在之前的教程中我们说视野(Field of View)或fov定义了我们可以看到场景中多大的范围。当视野变小时，场景投影出来的空间就会减小，产生放大(Zoom In)了的感觉。我们会使用鼠标的滚轮来放大。
 */
float fov = 45.0f;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

Camera camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));

float screenWidth = SCR_WIDTH;
float screenHeight = SCR_HEIGHT;

float skyboxVertices[] = {
    // positions
    -1.0f, 1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,

    -1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, -1.0f, 1.0f,

    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f, 1.0f,

    -1.0f, 1.0f, -1.0f,
    1.0f, 1.0f, -1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, 1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, 1.0f,
    1.0f, -1.0f, 1.0f};
std::string skyboxPath = "skybox/";
std::vector<std::string> faces{
    skyboxPath + "right.jpg",
    skyboxPath + "left.jpg",
    skyboxPath + "top.jpg",
    skyboxPath + "bottom.jpg",
    skyboxPath + "front.jpg",
    skyboxPath + "back.jpg"};

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // 第二个属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // 此时可以相关数据已经存到vao中，可以解绑vbo和vao
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glDeleteBuffers(1, &vbo);

    return vao;
}

unsigned int generateSkyboxVao(float *vertices, unsigned int vertexCount)
{
    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(float), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glDeleteBuffers(1, &vbo);

    return vao;
}
unsigned int cubmapTexture;
void drawModel(ShaderProgram &shader, Model modelObject)
{
    shader.use();
    glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), screenWidth / screenHeight, 0.1f, 100.0f);
    glm::mat4 view = camera.viewMatrix();
    shader.setMatrix4fv("projection", projection);
    shader.setMatrix4fv("view", view);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0, -1.75, 0.0));
    model = glm::scale(model, glm::vec3(0.2f));
    shader.setMatrix4fv("model", model);
    shader.setVec3("cameraPos", camera.cameraPos.x, camera.cameraPos.y, camera.cameraPos.z);
    shader.setInt("skybox", 3);
    glActiveTexture(GL_TEXTURE0 + 3);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubmapTexture);
    modelObject.Draw(shader);
}

void drawCubes(ShaderProgram &shaderProgram, unsigned int vao, unsigned int cubmapTexture, glm::vec3 cubePositions[], unsigned int cubeCount, glm::mat4 view, glm::mat4 projection)
{
    // 激活着色器程序
    shaderProgram.use();
    glBindVertexArray(vao);
    // shaderProgram.setInt("texture1", 0);
    // shaderProgram.setInt("texture2", 1);

    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D, texture1);
    // glActiveTexture(GL_TEXTURE1);
    // glBindTexture(GL_TEXTURE_2D, texture2);
    // shaderProgram.setFloat("mixValue", mixValue);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubmapTexture);

    shaderProgram.setInt("skybox", 0);
    shaderProgram.setVec3("cameraPos", camera.cameraPos.x, camera.cameraPos.y, camera.cameraPos.z);
    shaderProgram.setMatrix4fv("view", view);
    shaderProgram.setMatrix4fv("projection", projection);

    for (unsigned int i = 0; i < cubeCount; i += 1)
    {
        glm::mat4 model;
        model = glm::translate(model, cubePositions[i]);
        float angle = i == 0 ? 1 : glm::radians(20.0f * i);
        if (i % 3 == 0)
        {
            angle = (float)glfwGetTime() * angle;
        }
        model = glm::rotate(model, angle, glm::vec3(1.5, 0.3, 0.5));
        shaderProgram.setMatrix4fv("model", model);
        // 画矩形
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    glBindVertexArray(0);
}

int createHelloTriangleWindow()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    GLFWwindow *window = glfwCreateWindow(screenWidth, screenHeight, "Hello Triangle", NULL, NULL);
    if (window == NULL)
    {
        asLog("Failed to create glfw window");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mousePositionCallback);
    glfwSetScrollCallback(window, mouseScrollCallback);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
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
    ShaderProgram shaderProgram = ShaderProgram("4.6.3_cubmaps.vs", "4.6.3_cubmaps.fs");
    ShaderProgram skyboxShader = ShaderProgram("4.6.1_skybox.vs", "4.6.1_skybox.fs");
    ShaderProgram modelShader = ShaderProgram("load_model.vs", "load_model.fs");
    Model modelObject = Model("nanosuit/nanosuit.obj");

    float vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f};

    asLog("vertices size: %d", sizeof(vertices));
    // 顶点数组对象
    unsigned int vao = vaoGenerate(vertices, sizeof(vertices) / sizeof(float));
    unsigned int skyboxVao = generateSkyboxVao(skyboxVertices, sizeof(skyboxVertices) / sizeof(float));

    // unsigned int texture1 = textureGenarate("container.jpg");
    // unsigned int texture2 = textureGenarate("awesomeface.png");
    cubmapTexture = loadCubemap(faces);

    // uncomment this call to draw in wireframe polygons. 线条模式
    //    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //    glm::mat4 trans;
    //    trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
    //    trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
    //
    //    unsigned int transformLoc = glGetUniformLocation(shaderProgram.ID, "transform");
    //    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)};
    float expectedFrameTime = 1 / fps;
    // 当帧数过高时，usleep(diffTime * 1000000) 来降低帧数
    float diffTime = 0.0;
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        float temp = currentFrame - lastFrame;
        if (temp < expectedFrameTime)
        {
            diffTime = expectedFrameTime - temp;
            usleep(diffTime * 1000000);
            temp += diffTime;
            //            asLog("sleep:%f", diffTime);
        }
        deltaTime = temp;
        lastFrame = currentFrame;
        //        asLog("deltaTime:%f", deltaTime);

        progressInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        // GL_DEPTH_BUFFER_BIT 将 深度值 设置为 1
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // 开启深度测试
        glEnable(GL_DEPTH_TEST);

        glm::mat4 model = glm::mat4(1.0);
        glm::mat4 view;
        glm::mat4 projection;

        // 创建一个 lookAt 观察矩阵
        //        float radius = 10.0f;
        // camera 随时间转圈
        //        float camX = sin((float)glfwGetTime()) * radius;
        //        float camZ = cos((float)glfwGetTime()) * radius;
        //        view = glm::lookAt(glm::vec3(camX, 0, camZ), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
        view = camera.viewMatrix();

        // 投影矩阵
        projection = glm::perspective(glm::radians(camera.zoom), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);

        /**
         * 使用提前深度测试，使天空盒的深度值为1（在顶点着色器里改z为w），先渲染盒子后渲染天空盒，优化性能
         */

        // drawCubes(shaderProgram, vao, cubmapTexture, cubePositions, sizeof(cubePositions) / sizeof(glm::vec3), view, projection);
        drawModel(modelShader, modelObject);

        // 禁用深度写入
        // glDepthMask(GL_FALSE);
        glDepthFunc(GL_LEQUAL);
        skyboxShader.use();
        glBindVertexArray(skyboxVao);
        skyboxShader.setInt("skybox", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubmapTexture);
        skyboxShader.setMatrix4fv("view", glm::mat4(glm::mat3(view)));
        skyboxShader.setMatrix4fv("projection", projection);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // glDepthMask(GL_TRUE);

        glBindVertexArray(0);
        glDepthFunc(GL_LESS);

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
    screenWidth = width;
    screenHeight = height;
    glViewport(0, 0, width, height);
}

void progressInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        asLog("escape pressed");
        glfwSetWindowShouldClose(window, true);
    }
    else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        mixValue += 0.01;
        mixValue = mixValue > 1.0f ? 1.0f : mixValue;
        asLog("mix value: %f", mixValue);
    }
    else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        mixValue -= 0.01;
        mixValue = mixValue < 0.0f ? 0.0f : mixValue;
        asLog("mix value: %f", mixValue);
    }
    else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.progressKeyboard(FORWARD, deltaTime);
    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.progressKeyboard(BACKWARD, deltaTime);
    }
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.progressKeyboard(LEFT, deltaTime);
    }
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.progressKeyboard(RIGHT, deltaTime);
    }
}

float lastXpos = screenWidth / 2;
float lastYpos = screenHeight / 2;
bool firstMouse = true;
float sensitivity = 0.05f;
// 俯仰角
float pitch = 0.0f;
// 偏航角
float yaw = -90.0f; // 初始化成pitch =0, yaw = -90 相当于 camerafront = (0, 0, -1)
void mousePositionCallback(GLFWwindow *window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastXpos = xpos;
        lastYpos = ypos;
        firstMouse = false;
    }
    asLog("xpos: %f, y: %f", xpos, ypos);
    float offsetX = xpos - lastXpos;
    float offsetY = lastYpos - ypos;
    asLog("offsetX: %f, offsetY: %f", offsetX, offsetY);
    lastXpos = xpos;
    lastYpos = ypos;

    camera.progressMouseMove(offsetX, offsetY);
}

void mouseScrollCallback(GLFWwindow *window, double xpos, double ypos)
{
    asLog("xpos: %f, ypos: %f", xpos, ypos);
    camera.progressScroll(ypos);
}
