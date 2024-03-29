#include "Camera.h"

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(Position, Position+Front, Up);
}

glm::mat4 Camera::GetLightMatrix()
{
    return glm::lookAt(Position+light_offset, Position+Front, Up);
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
    float velocity = MovementSpeed * deltaTime;
    if(direction == FORWARD){
        Position += Front * velocity;
    }else if(direction == BACKWARD){
        Position -= Front * velocity;
    }else if(direction == LEFT){
        Position -= Right * velocity;
    }else{
        Position += Right * velocity;
    }
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    if(constrainPitch){
        if(Pitch > 89.0f)
            Pitch = 89.0f;
        if(Pitch < -89.0f)
            Pitch = -89.0f;
    }    
    updateCameraVectors();
}

void Camera::updateCameraVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);

    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up = glm::normalize(glm::cross(Right, Front));
}
