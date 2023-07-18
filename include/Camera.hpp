#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

enum class CameraMoveDir
{
    Forward,
    Back,
    Left,
    Right,
    Up,
    Down
};

class Camera
{

public:
    Camera() = default;

    void updateDirection(glm::vec2 mouseRotation);

    void move(CameraMoveDir moveDir, float speed, float deltaTime);

    glm::mat4 calcViewMatrix();
    glm::mat4 calcProjectionMatrix(float windowWidth, float windowHeight);

    void setPosition(glm::vec3 newPos);
    glm::vec3 getPosition();

private:
    const float NEAR_CLIP = 0.1;
    const float FAR_CLIP = 100;

    const glm::vec3 UP = glm::vec3(0, 1, 0);
    
    glm::vec3 position = glm::vec3(0, 0, 0);
    glm::vec3 direction = glm::vec3(0, 0, -1);

    float fieldOfView = 110;
    float lookSensitivity = 30;

};

#endif