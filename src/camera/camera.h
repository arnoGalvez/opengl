#ifndef __CAMERA_H
#define __CAMERA_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

using namespace glm;

enum directions {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

const float YAW = 0.f;
const float PITCH = 0.f;
const float SPEED = 8.f;
const float MOUSESENS = 0.05f;

class Camera {
    public:
        vec3 worldUp;
        vec3 position;
        vec3 targetPoint;
        
        vec3 front;
        vec3 up; 
        vec3 right;

        float yaw; // rotation around x-axis, in degrees
        float pitch;// rotation around y-axis, in degrees

        float speed;
        float mouseSensitivity;

        Camera() :
            worldUp(vec3(0.f, 1.f, 0.f)),
            position(vec3(0.f, 0.f, 5.f)),
            targetPoint(vec3(0.5f, 0.5f, 0.f)),
            speed(SPEED),
            mouseSensitivity(MOUSESENS)
        {
            front = vec3(0.f, 0.f, -1.f);
            UpdateVectors();
        }

        mat4 GetViewMatrix()
        {
            return lookAt(position, position + front, worldUp);
        }

        mat4 GetViewMatrix(vec3 target)
        {
            return lookAt(position, target, worldUp);
        }

        void ProcessKeyboard(directions dir, float deltaTime)
        {
            float velocity = speed * deltaTime;
            if (dir == FORWARD)
                position += front * velocity;
            if (dir == BACKWARD)
                position -= front * velocity;
            if (dir == LEFT)
                position -= right * velocity;
            if (dir == RIGHT)
                position += right * velocity;
        }

        void ProcessMouseMouvement(float xOffset, float yOffset, bool constrainPitch = true)
        {
            yaw += xOffset * mouseSensitivity;
            pitch += yOffset * mouseSensitivity;

            if (constrainPitch)
            {
                if (pitch > 89.f)
                    pitch = 89.f;
                if (pitch < -89.f)
                    pitch = -89.f;
            }
            UpdateVectors();
        }


    
    private:
        void UpdateVectors()
        {
            front.x = cos(radians(pitch)) * cos(radians(yaw));
            front.y = sin(radians(pitch));
            front.z = cos(radians(pitch)) * sin(radians(yaw));
            front = normalize(front);
            right = normalize(cross(front, worldUp));
            up = normalize(cross(right, front));
        }

};

#endif