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
const float FPS = 70.0f;

float screenWidth = SCR_WIDTH;
float screenHeight = SCR_HEIGHT;
GLFWwindow *window;
Camera camera = Camera(glm::vec3(0.5f, 0.5f, 5.0f));
float deltaTime = 0.0f;

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

int createWindow()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    window = glfwCreateWindow(screenWidth, screenHeight, "Light", NULL, NULL);
    if (window == NULL)
    {
        asLog("failed to create window");
        glfwTerminate();
        return -1;
    }
    return 1;
}

void frameBufferSizeCallback(GLFWwindow *window, int width, int height)
{
    screenWidth = width;
    screenHeight = height;
    glViewport(0, 0, width, height);
}

bool firstCursor = true;
float lastXpos = screenWidth / 2;
float lastYpos = screenHeight / 2;
void mouseMoveCallback(GLFWwindow *window, double xpos, double ypos)
{
    if (firstCursor)
    {
        lastXpos = xpos;
        lastYpos = ypos;
        firstCursor = false;
    }
    asLog("xpos: %f, y: %f", xpos, ypos);
    float offsetX = xpos - lastXpos;
    float offsetY = lastYpos - ypos;
    asLog("offsetX: %f, offsetY: %f", offsetX, offsetY);
    lastXpos = xpos;
    lastYpos = ypos;
    camera.progressMouseMove(offsetX, offsetY);
}

void scrollCallback(GLFWwindow *window, double xpos, double ypos)
{
    camera.progressScroll(ypos);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
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
    else if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

void setupWindow()
{
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouseMoveCallback);
    glfwSetScrollCallback(window, scrollCallback);
}

unsigned int genLampVAO(float *vertices, unsigned int length)
{
    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, length * sizeof(float), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glDeleteBuffers(1, &vbo);
    return vao;
}

unsigned int genCubeVAO(float *vertices, unsigned int length)
{
    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, length * sizeof(float), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glDeleteBuffers(1, &vbo);
    return vao;
}

int initOpenGL()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        glfwTerminate();
        return -1;
    }
    return 1;
}
glm::vec3 lampPos(1.2f, 1.0f, 2.0f);
void drawLamp(ShaderProgram &lampShader, unsigned int vao)
{
    lampShader.use();
    glBindVertexArray(vao);
    lampPos.x = sqrtf(1.2 * 1.2 + 2.0 * 2.0) * cosf((float)glfwGetTime());
    lampPos.z = sqrtf(1.2 * 1.2 + 2.0 * 2.0) * sinf((float)glfwGetTime());
    glm::mat4 model = glm::mat4();
    model = glm::translate(model, lampPos);
    model = glm::scale(model, glm::vec3(0.2f));
    glm::mat4 view = camera.viewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), screenWidth / screenHeight, 0.1f, 100.0f);
    lampShader.setMatrix4fv("model", model);
    lampShader.setMatrix4fv("view", view);
    lampShader.setMatrix4fv("projection", projection);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void drawCube(ShaderProgram &cubeShader, unsigned int vao)
{
    cubeShader.use();
    glBindVertexArray(vao);
    glm::vec3 cubPos(0.0f, 0.0f, 0.0f);
    glm::mat4 model = glm::mat4();
    model = glm::translate(model, cubPos);
    //    model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1.5, 0.3, 0.5));
    glm::mat4 view = camera.viewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), screenWidth / screenHeight, 0.1f, 100.0f);
    cubeShader.setMatrix4fv("model", model);
    cubeShader.setMatrix4fv("view", view);
    cubeShader.setMatrix4fv("projection", projection);
    cubeShader.setVec3("cubeColor", 1.0f, 0.5f, 0.31f);
    cubeShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
    cubeShader.setVec3("lampPos", lampPos.x, lampPos.y, lampPos.z);
    cubeShader.setVec3("viewPos", camera.cameraPos.x, camera.cameraPos.y, camera.cameraPos.z);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void draw()
{
    ShaderProgram cubeShader = ShaderProgram("cube.vs", "cube.fs");
    ShaderProgram lampShader = ShaderProgram("cube.vs", "lamp.fs");
    unsigned int lampVAO = genLampVAO(vertices, sizeof(vertices) / sizeof(float));
    unsigned int cubeVAO = genCubeVAO(vertices, sizeof(vertices) / sizeof(float));
    float fps = FPS;
    float expectedFrameTime = 1.0f / fps;
    float lastFrameTime = 0.0f;
    bool firstRender = true;
    float diffTime = 0.0f;
    while (!glfwWindowShouldClose(window))
    {
        float currentTime = (float)glfwGetTime();
        if (firstRender)
        {
            lastFrameTime = currentTime;
            firstRender = false;
        }
        float temp = currentTime - lastFrameTime;
        if (temp < expectedFrameTime)
        {
            diffTime = expectedFrameTime - temp;
            usleep(diffTime * 1000000);
            temp = expectedFrameTime;
        }
        deltaTime = temp;
        lastFrameTime = currentTime;
        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        drawLamp(lampShader, lampVAO);
        drawCube(cubeShader, cubeVAO);
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    glDeleteVertexArrays(1, &lampVAO);
    glDeleteVertexArrays(1, &cubeVAO);
    glfwTerminate();
}

int main(int argc, const char *argv[])
{
    // insert code here...
    std::cout << "Hello, World!\n";
    if (1 != createWindow())
    {
        return -1;
    }
    setupWindow();
    if (1 == initOpenGL())
    {
        draw();
    }
    return 0;
}
