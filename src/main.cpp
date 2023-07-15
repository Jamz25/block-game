#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <cmath>

#include "Shader.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
} 

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

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

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    Shader shaderProgram("./shader.vs", "./shader.fs");

    float vertices[] = {
        -0.5f, -0.5f, 0.5f, 0.5f, 0.2f, 0.3f,
        0.5f, 0.5f, 0.5f, 0.5f, 0.2f, 0.3f,
        -0.5f, 0.5f, 0.5f, 0.5f, 0.2f, 0.3f,
        -0.5f, -0.5f, 0.5f, 0.5f, 0.2f, 0.3f,
        0.5f, 0.5f, 0.5f, 0.5f, 0.2f, 0.3f,
        0.5f, -0.5f, 0.5f, 0.5f, 0.2f, 0.3f,

        -0.5f, -0.5f, 0.5f, 0.2f, 0.2f, 0.3f,
        -0.5f, 0.5f, -0.5f, 0.2f, 0.2f, 0.3f,
        -0.5f, 0.5f, 0.5f, 0.2f, 0.2f, 0.3f,
        -0.5f, -0.5f, 0.5f, 0.2f, 0.2f, 0.3f,
        -0.5f, 0.5f, -0.5f, 0.2f, 0.2f, 0.3f,
        -0.5f, -0.5f, -0.5f, 0.2f, 0.2f, 0.3f,
    };

    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);

    glViewport(0, 0, 800, 600);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    const int CAMERA_SLOW = 1000;
    const float MOVE_SPEED = 1.6f;
    glm::vec3 camera_pos(0.0f, 0.0f, -3.0f);

    float lastTime = glfwGetTime();

    while (!glfwWindowShouldClose(window))
    {

        processInput(window);

        float deltaTime = glfwGetTime() - lastTime;
        lastTime = glfwGetTime();

        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            camera_pos.z -= MOVE_SPEED * deltaTime;
        }
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            camera_pos.z += std::cos((float)xpos / CAMERA_SLOW) * MOVE_SPEED * deltaTime;
            camera_pos.x += std::sin((float)xpos / CAMERA_SLOW) * MOVE_SPEED * deltaTime;
        }

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderProgram.use();

        glm::mat4 view = glm::mat4(1.0f);
        view = glm::rotate(view, (float)xpos / CAMERA_SLOW, glm::vec3(0.0f, 1.0f, 0.0f));
        view = glm::rotate(view, (float)ypos / CAMERA_SLOW, glm::vec3(1.0f, 0.0f, 0.0f));
        view = glm::translate(view, camera_pos);
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(55.0f), 800.0f / 600.0f, 0.1f, 100.0f);

        shaderProgram.setMat4("view", view);
        shaderProgram.setMat4("projection", projection);

        glBindVertexArray(VAO);
        for (int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            model = glm::rotate(model, (float)glfwGetTime() + i * 5, glm::vec3(0.4f, 0.6f, 0.2f));
            shaderProgram.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 12);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    glfwTerminate();

    return 0;

}