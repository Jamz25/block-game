#include <iostream>
#include <cmath>
#include <array>
#include <memory>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>
#include <OpenSimplexNoise.h>

#include "Shader.hpp"
#include "Camera.hpp"
#include "CubeRenderer.hpp"
#include "Chunk.hpp"
#include "ChunkMesh.hpp"
#include "Block.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow* window);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

void mouse_callback(GLFWwindow* window, int button, int action, int mods);

void updateCamera(GLFWwindow* window, Camera& camera, float moveSpeed, float deltaTime);

glm::vec4 mousePicker(GLFWwindow* window, Camera& camera);

int main()
{
    
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Test", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); 
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    glViewport(0, 0, 800, 600);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    Shader shaderProgram("./shader.vs", "./shader.fs");
    Camera playerCamera;
    playerCamera.setPosition(glm::vec3(10, 10, 5));
    

    srand(time(0));
    OpenSimplexNoise::Noise terrain_noise(rand());
    OpenSimplexNoise::Noise terrain_noise1(rand() * 2);

    std::array<std::array<std::unique_ptr<Chunk>, 10>, 100> chunks;

    for (int i = 0; i < 100; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            
            std::unique_ptr<Chunk> chunk = std::make_unique<Chunk>();

            chunk->initialiseMesh();

            ChunkData chunkData;

            for (int x = 0; x < CHUNK_X_SIZE; x++)
            {
                for (int z = 0; z < CHUNK_Z_SIZE; z++)
                {
                    
                    double height_value = terrain_noise.eval((i + x) / 10, (j + z) / 10) * 4 + terrain_noise1.eval((i + x) / 3, (j + z) / 3) * 4 + 4;
                    for (int y = 0; y < CHUNK_Y_SIZE; y++)
                    {
                        if (y < height_value)
                        {
                            chunkData[x][z][y].type = 1;
                        }
                        else
                        {
                            chunkData[x][z][y].type = 0;
                        }
                    }
                    
                }
            }

            chunk->setData(chunkData);

            chunk->setWorldPosition(i, j);

            chunks[i][j].swap(chunk);

        }
    }
    
    for (int i = 0; i < 100; i ++)
    {
        for (int j = 0; j < 10; j++)
        {

            std::unique_ptr<Chunk>& chunk = chunks[i][j];

            Chunk* pXChunk = nullptr;
            if (i < 99)
            {
                pXChunk = chunks[i + 1][j].get();
            }
            Chunk* nXChunk = nullptr;
            if (i > 0)
            {
                nXChunk = chunks[i - 1][j].get();
            }
            Chunk* pZChunk = nullptr;
            if (j < 9)
            {
                pZChunk = chunks[i][j + 1].get();
            }
            Chunk* nZChunk = nullptr;
            if (j > 0)
            {
                nZChunk = chunks[i][j - 1].get();
            }
            
            chunk->createMesh(pXChunk, nXChunk, pZChunk, nZChunk);

        }
    }


    const float MOVE_SPEED = 1.6f;

    float lastTime = glfwGetTime();

    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    while (!glfwWindowShouldClose(window))
    {

        //processInput(window);

        float deltaTime = glfwGetTime() - lastTime;
        lastTime = glfwGetTime();

        glfwSetWindowTitle(window, std::string("original block game   running at " + std::to_string((int)(1 / deltaTime)) + "fps").c_str());

        updateCamera(window, playerCamera, MOVE_SPEED, deltaTime);

        glClearColor(0.2, 0.6, 0.7, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderProgram.use();

        glm::mat4 viewMatrix = playerCamera.calcViewMatrix();
        glm::mat4 projectionMatrix = playerCamera.calcProjectionMatrix(800, 600);

        shaderProgram.setMat4("view", viewMatrix);
        shaderProgram.setMat4("projection", projectionMatrix);

        for (int i = 0; i < 100; i++)
        {
            for (int j = 0; j < 10; j++)
            {
                chunks[i][j]->render(shaderProgram);
            }
        }

        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    glfwTerminate();

    return 0;

}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

    int inputMode = glfwGetInputMode(window, GLFW_CURSOR);

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        if (inputMode == GLFW_CURSOR_DISABLED)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        else
        {
            glfwSetWindowShouldClose(window, true);
        }
    }
    
}

void mouse_callback(GLFWwindow* window, int button, int action, int mods)
{

    int inputMode = glfwGetInputMode(window, GLFW_CURSOR);

    if (inputMode == GLFW_CURSOR_NORMAL)
    {
        if (button == GLFW_MOUSE_BUTTON_LEFT)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            glfwSetCursorPos(window, 800 / 2, 600 / 2);
        }
    }

}

void updateCamera(GLFWwindow* window, Camera& camera, float moveSpeed, float deltaTime)
{

    if (glfwGetInputMode(window, GLFW_CURSOR) != GLFW_CURSOR_DISABLED)
    {
        return;
    }

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    glm::vec2 mouse_rotation;
    mouse_rotation.y = static_cast<float>(ypos - (600 / 2)) / 600;
    mouse_rotation.x = static_cast<float>(xpos - (800 / 2)) / 800;

    glfwSetCursorPos(window, 800 / 2, 600 / 2);
    
    camera.updateDirection(mouse_rotation);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.move(CameraMoveDir::Forward, moveSpeed, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.move(CameraMoveDir::Back, moveSpeed, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.move(CameraMoveDir::Left, moveSpeed, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.move(CameraMoveDir::Right, moveSpeed, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        camera.move(CameraMoveDir::Up, moveSpeed, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        camera.move(CameraMoveDir::Down, moveSpeed, deltaTime);
    }

}

glm::vec4 mousePicker(GLFWwindow* window, Camera& camera)
{

    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);
    float x = (2 * mouseX) / 800 - 1;
    float y = (2 * mouseY) / 600 - 1;
    glm::vec2 normalizedCoords = glm::vec2(x, -y);
    glm::mat4 inverseProjection = glm::inverse(camera.calcProjectionMatrix(800, 600));
    //glm::vec4 eyeCoords = normalizedCoords * inverseProjection;
    

}
