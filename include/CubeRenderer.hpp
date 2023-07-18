#ifndef CUBE_RENDERER_HPP
#define CUBE_RENDERER_HPP

#include <array>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>

#include "Shader.hpp"
#include "Block.hpp"

class CubeRenderer
{

public:
    CubeRenderer() = default;

    void initialiseBuffers();

    void loadTextures();

    void renderCube(Shader& shader, Block block_data, glm::vec3 translation, glm::vec3 rotation_axis, float rotation_amount, glm::vec3 scale);

private:
    GLuint VBO, EBO, VAO;
    GLuint textureID;

    const float TOP_LIGHT = 1.0;
    const float SIDE_LIGHT = 0.6;
    const float BOTTOM_LIGHT = 0.3;

    std::array<float, 6 * 24> vertex_data = {
        -1, -1,  1,     0, 0,   SIDE_LIGHT,   // 0  - 0
        -1, -1,  1,     1, 0,   SIDE_LIGHT,   //      1

         1, -1,  1,     1, 0,   SIDE_LIGHT,   // 1  - 2
         1, -1,  1,     0, 0,   SIDE_LIGHT,   //      3
         
        -1,  1,  1,     1, 1,   SIDE_LIGHT,   // 2  - 4
        -1,  1,  1,     0, 1,   SIDE_LIGHT,   //      5
        -1,  1,  1,     0, 0,   TOP_LIGHT,    //      6

         1,  1,  1,     1, 1,   SIDE_LIGHT,   // 3  - 7
         1,  1,  1,     0, 1,   SIDE_LIGHT,   //      8
         1,  1,  1,     1, 0,   TOP_LIGHT,    //      9

        -1, -1, -1,     0, 0,   SIDE_LIGHT,   // 4  - 10
        -1, -1, -1,     1, 0,   SIDE_LIGHT,   //      11
        -1, -1, -1,     0, 1,   BOTTOM_LIGHT, //      12

         1, -1, -1,     0, 0,   SIDE_LIGHT,   // 5  - 13
         1, -1, -1,     1, 0,   SIDE_LIGHT,   //      14
         1, -1, -1,     1, 1,   BOTTOM_LIGHT, //      15

        -1,  1, -1,     1, 1,   SIDE_LIGHT,   // 6  - 16
        -1,  1, -1,     0, 1,   SIDE_LIGHT,   //      17

         1,  1, -1,     0, 1,   SIDE_LIGHT,   // 7  - 18
         1,  1, -1,     1, 1,   SIDE_LIGHT,   //      19
         1,  1, -1,     1, 1,   TOP_LIGHT,    //      20

        -1, -1,  1,     0, 0,   BOTTOM_LIGHT, // 0  - 21
         1, -1,  1,     1, 0,   BOTTOM_LIGHT, // 1  - 22
        -1,  1, -1,     0, 1,   TOP_LIGHT,    // 6  - 23
    };

    std::array<unsigned short, 36> indicies = {
        0, 2, 5, // +z
        2, 5, 7,
        
        3, 14, 8, // +x
        14, 8, 19,

        13, 11, 18, //-z
        11, 18, 16,

        10, 1, 17, //-x
        1, 17, 4,

        6, 9, 23, //top (+y)
        9, 23, 20,

        21, 22, 12, //bottom (-y)
        22, 12, 15
    };

};

#endif