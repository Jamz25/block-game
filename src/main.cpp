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
#include <FastNoiseLite.h>

#include "Shader.hpp"
#include "Camera.hpp"
#include "Chunk.hpp"
#include "ChunkMesh.hpp"
#include "Block.hpp"
#include "TextureLoader.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow* window);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

void mouse_callback(GLFWwindow* window, int button, int action, int mods);

void updateCamera(GLFWwindow* window, Camera& camera, float moveSpeed, float deltaTime);

glm::vec4 mousePicker(GLFWwindow* window, Camera& camera);

int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 600;

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

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    Shader shaderProgram("./shader.vs", "./shader.fs");
    Camera playerCamera;
    playerCamera.setPosition(glm::vec3(20 * 0.5, 30, 20 * 0.5));

    TextureLoader::loadTextures();

    FastNoiseLite noise((unsigned)time(0));
    noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    FastNoiseLite noise_hills((unsigned)time(0) * 2);
    noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    FastNoiseLite noise_caves((unsigned)time(0) / 2);
    noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);

    std::array<std::array<std::unique_ptr<Chunk>, 20>, 20> chunks;

    for (int i = 0; i < 20; i++)
    {
        for (int j = 0; j < 20; j++)
        {
            
            std::unique_ptr<Chunk> chunk = std::make_unique<Chunk>();

            chunk->initialiseMesh();

            ChunkData chunkData;

            for (int x = 0; x < CHUNK_X_SIZE; x++)
            {
                for (int z = 0; z < CHUNK_Z_SIZE; z++)
                {
                    
                    float height_value = noise.GetNoise((float)(x + i * CHUNK_X_SIZE), (float)(z + j * CHUNK_Z_SIZE)) * 5 + 15;
                    float extra_height = noise_hills.GetNoise((float)((x + i * CHUNK_X_SIZE) * 0.5), (float)((z + j * CHUNK_Z_SIZE) * 0.5)) * 50;
                    height_value = std::max(height_value, extra_height) + 80;
                    //float height_value = x + j;
                    for (int y = 0; y < CHUNK_Y_SIZE; y++)
                    {

                        float cave_value = noise.GetNoise((float)(x + i * CHUNK_X_SIZE), (float)(z + j * CHUNK_Z_SIZE), (float)y);

                        if (y > height_value || cave_value > 0.5)
                        {
                            chunkData[x][z][y].type = BlockType::Air;
                        }
                        else
                        {
                            if (y + 1 >= height_value)
                            {
                                chunkData[x][z][y].type = BlockType::Grass;
                            }
                            else if (y > height_value - 5)
                            {
                                chunkData[x][z][y].type = BlockType::Dirt;
                            }
                            else
                            {
                                chunkData[x][z][y].type = BlockType::Stone;
                            }
                        }
                    }
                    
                }
            }

            chunk->setData(chunkData);

            chunk->setWorldPosition(i, j);

            chunks[i][j].swap(chunk);

        }
    }
    
    for (int i = 0; i < 20; i ++)
    {
        for (int j = 0; j < 20; j++)
        {

            std::unique_ptr<Chunk>& chunk = chunks[i][j];

            Chunk* pXChunk = nullptr;
            if (i < 19)
            {
                pXChunk = chunks[i + 1][j].get();
            }
            Chunk* nXChunk = nullptr;
            if (i > 0)
            {
                nXChunk = chunks[i - 1][j].get();
            }
            Chunk* pZChunk = nullptr;
            if (j < 19)
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


    const float MOVE_SPEED = 2.0f;

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
        glm::mat4 projectionMatrix = playerCamera.calcProjectionMatrix(WINDOW_WIDTH, WINDOW_HEIGHT);

        shaderProgram.setMat4("view", viewMatrix);
        shaderProgram.setMat4("projection", projectionMatrix);

        TextureLoader::bindBlockAtlas();

        for (int i = 0; i < 20; i++)
        {
            for (int j = 0; j < 20; j++)
            {
                chunks[i][j]->render(shaderProgram);
            }
        }

        TextureLoader::unbindBlockAtlas();

        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    glfwTerminate();

    return 0;

}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    WINDOW_WIDTH = width;
    WINDOW_HEIGHT = height;
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
    mouse_rotation.y = static_cast<float>(ypos - (WINDOW_WIDTH / 2)) / WINDOW_WIDTH;
    mouse_rotation.x = static_cast<float>(xpos - (WINDOW_HEIGHT / 2)) / WINDOW_HEIGHT;

    glfwSetCursorPos(window, WINDOW_HEIGHT / 2, WINDOW_WIDTH / 2);
    
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
