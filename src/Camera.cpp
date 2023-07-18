#include "Camera.hpp"

void Camera::updateDirection(glm::vec2 mouseRotation)
{

    glm::vec3 verticalRotationAxis = glm::normalize(glm::cross(direction, glm::vec3(0, 1, 0)));

    glm::vec3 newDirection = glm::rotate(direction, glm::radians(-mouseRotation.y * lookSensitivity), verticalRotationAxis);

    bool lookingBelowUp = glm::angle(newDirection, glm::vec3(0, 1, 0)) > glm::radians(5.0f);
    bool lookingAboveDown = glm::angle(newDirection, glm::vec3(0, -1, 0)) > glm::radians(5.0f);
    
    if (lookingBelowUp && lookingAboveDown)
    {
        direction = newDirection;
    }

    direction = glm::rotate(direction, glm::radians(-mouseRotation.x * lookSensitivity), glm::vec3(0, 1, 0));

}

void Camera::move(CameraMoveDir moveDir, float speed, float deltaTime)
{

    if (moveDir == CameraMoveDir::Forward)
    {
        position += direction * speed * deltaTime;
    }
    if (moveDir == CameraMoveDir::Back)
    {
        position -= direction * speed * deltaTime;
    }
    if (moveDir == CameraMoveDir::Left)
    {
        position -= glm::normalize(glm::cross(direction, glm::vec3(0, 1, 0))) * speed * deltaTime;
    }
    if (moveDir == CameraMoveDir::Right)
    {
        position += glm::normalize(glm::cross(direction, glm::vec3(0, 1, 0))) * speed * deltaTime;
    }

    if (moveDir == CameraMoveDir::Up)
    {
        position.y += speed * deltaTime;
    }
    if (moveDir == CameraMoveDir::Down)
    {
        position.y -= speed * deltaTime;
    }

}

glm::mat4 Camera::calcViewMatrix()
{
    glm::mat4 view = glm::lookAt(position, position + direction, glm::vec3(0, 1, 0));
    return view;
}

glm::mat4 Camera::calcProjectionMatrix(float windowWidth, float windowHeight)
{
    float aspectRatio = windowWidth / windowHeight;
    glm::mat4 projection = glm::perspective(glm::radians(fieldOfView / 2), aspectRatio, NEAR_CLIP, FAR_CLIP);
    return projection;
}

void Camera::setPosition(glm::vec3 newPos)
{
    position = newPos;
}

glm::vec3 Camera::getPosition()
{
    return position;
}