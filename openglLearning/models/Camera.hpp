//
//  Camera.hpp
//  createWindow
//
//  Created by Ashoka on 2019/3/15.
//

#ifndef Camera_hpp
#define Camera_hpp

#include <stdio.h>
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.05f;
const float ZOOM = 45.0f;

class Camera {
    glm::vec3 cameraPos;
    glm::vec3 cameraFront;
    glm::vec3 worldUp;
    // 右轴
    glm::vec3 right;
    // 上轴
    glm::vec3 up;

public:
    // 欧拉角
    float yaw;
    float pitch;
    // 缩放
    float zoom;
    // 移动速度
    float speed;
    // 鼠标灵敏度
    float mouseSensitivity;

    Camera(glm::vec3 pos, glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float newYaw = YAW, float newPitch = PITCH) : cameraFront(glm::vec3(0.0f, 0.0f, -1.0f)), speed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM) {
        cameraPos = pos;
        worldUp = up;
        yaw = newYaw;
        pitch = newPitch;
        
        updateCameraData();
    }

    void progressKeyboard(CameraMovement direction, float deltatime)
    {
        float velocity = speed * deltatime;
        switch (direction) {
            case FORWARD:
                cameraPos += velocity * cameraFront;
                break;
            case BACKWARD:
                cameraPos -= velocity * cameraFront;
                break;
            case LEFT:
                cameraPos -= velocity * right;
                break;
            case RIGHT:
                cameraPos += velocity * right;
                break;
            default:
                break;
        }
    }
    
    void progressMouseMove(float offsetX, float offsetY, bool constrainPitch = true) {
        yaw += offsetX * mouseSensitivity;
        pitch += offsetY * mouseSensitivity;
        if (constrainPitch) {
            if (pitch > 89.0f) {
                pitch = 89.0f;
            }
            if (pitch < -89.0f) {
                pitch = -89.0f;
            }
        }
        updateCameraData();
    }
    
    void progressScroll(float yOffset) {
        if (zoom >= 1.0f && zoom <= 45.0f) {
            zoom -= yOffset;
        }
        if (zoom < 1.0f) {
            zoom = 1.0f;
        }
        if (zoom > 45.0f) {
            zoom = 45.0f;
        }
    }

    glm::mat4 viewMatrix()
    {
        return glm::lookAt(cameraPos, cameraPos + cameraFront, up);
    }
    
private:
    void updateCameraData() {
        glm::vec3 front;
        front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
        front.y = sin(glm::radians(pitch));
        front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
        cameraFront = glm::normalize(front);
        
        right = glm::normalize(glm::cross(front, worldUp));
        up = glm::normalize(glm::cross(right, front));
    }
};

#endif /* Camera_hpp */
