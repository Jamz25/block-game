#ifndef CHUNK_MESH_HPP
#define CHUNK_MESH_HPP

#include <array>
#include <vector>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>

#include "Shader.hpp"
#include "TextureLoader.hpp"
#include "Block.hpp"

struct Vertex
{
    float x, y, z;
    float u, v;
    float lightStrength;
};

enum class FaceDirection
{
    pX, nX,
    pZ, nZ,
    pY, nY
};

class ChunkMesh
{

public:
    ChunkMesh() = default;

    void initialiseBuffers();

    void beginMeshGeneration();
    void endMeshGeneration();

    void addMeshData(BlockType blockType, FaceDirection faceDir, glm::vec3 local_offset, float scale);

    void renderMesh();

private:
    GLuint VAO, VBO, EBO;

    std::vector<Vertex> vertex_data;
    std::vector<Vertex> vertex_data_generating;

    std::vector<unsigned int> vertex_indexes;

    bool generating = false;

    static constexpr std::array<Vertex, 4> pX_verts = {
        Vertex{1, -1, 1, 0, 0},
        Vertex{1, -1, -1, 1, 0},
        Vertex{1, 1, 1, 0, 1},
        //Vertex{1, -1, -1, 1, 0},
        //Vertex{1, 1, 1, 0, 1},
        Vertex{1, 1, -1, 1, 1}
    };

    static constexpr std::array<Vertex, 4> nX_verts = {
        Vertex{-1, -1, -1, 0, 0},
        Vertex{-1, -1, 1, 1, 0},
        Vertex{-1, 1, -1, 0, 1},
        //Vertex{-1, -1, 1, 1, 0},
        //Vertex{-1, 1, -1, 0, 1},
        Vertex{-1, 1, 1, 1, 1}
    };

    static constexpr std::array<Vertex, 4> pZ_verts = {
        Vertex{-1, -1, 1, 0, 0},
        Vertex{1, -1, 1, 1, 0},
        Vertex{-1, 1, 1, 0, 1},
        //Vertex{1, -1, 1, 1, 0},
        //Vertex{-1, 1, 1, 0, 1},
        Vertex{1, 1, 1, 1, 1}
    };

    static constexpr std::array<Vertex, 4> nZ_verts = {
        Vertex{1, -1, -1, 0, 0},
        Vertex{-1, -1, -1, 1, 0},
        Vertex{1, 1, -1, 0, 1},
        //Vertex{-1, -1, -1, 1, 0},
        //Vertex{1, 1, -1, 0, 1},
        Vertex{-1, 1, -1, 1, 1}
    };

    static constexpr std::array<Vertex, 4> pY_verts = {
        Vertex{-1, 1, 1, 0, 0},
        Vertex{1, 1, 1, 1, 0},
        Vertex{-1, 1, -1, 0, 1},
        //Vertex{1, 1, 1, 1, 0},
        //Vertex{-1, 1, -1, 0, 1},
        Vertex{1, 1, -1, 1, 1}
    };

    static constexpr std::array<Vertex, 4> nY_verts = {
        Vertex{-1, -1, 1, 0, 0},
        Vertex{-1, -1, -1, 0, 1},
        Vertex{1, -1, 1, 1, 0},
        //Vertex{1, -1, 1, 1, 0},
        //Vertex{-1, -1, -1, 0, 1},
        Vertex{1, -1, -1, 1, 1}
    };

};

#endif