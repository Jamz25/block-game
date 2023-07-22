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

    void loadTextures();

    void beginMeshGeneration();
    void endMeshGeneration();

    void addMeshData(FaceDirection faceDir, glm::vec3 local_offset, float scale);

    void renderMesh();

private:
    GLuint VAO, VBO, EBO;
    GLuint textureID;

    std::vector<Vertex> vertex_data;
    std::vector<Vertex> vertex_data_generating;

    bool generating = false;

    const std::array<Vertex, 6> pX_verts = {
        Vertex{1, -1, 1, 0, 0},
        Vertex{1, -1, -1, 1, 0},
        Vertex{1, 1, 1, 0, 1},
        Vertex{1, -1, -1, 1, 0},
        Vertex{1, 1, 1, 0, 1},
        Vertex{1, 1, -1, 1, 1}
    };

    const std::array<Vertex, 6> nX_verts = {
        Vertex{-1, -1, -1, 0, 0},
        Vertex{-1, -1, 1, 1, 0},
        Vertex{-1, 1, -1, 0, 1},
        Vertex{-1, -1, 1, 1, 0},
        Vertex{-1, 1, -1, 0, 1},
        Vertex{-1, 1, 1, 1, 1}
    };

    const std::array<Vertex, 6> pZ_verts = {
        Vertex{-1, -1, 1, 0, 0},
        Vertex{1, -1, 1, 1, 0},
        Vertex{-1, 1, 1, 0, 1},
        Vertex{1, -1, 1, 1, 0},
        Vertex{-1, 1, 1, 0, 1},
        Vertex{1, 1, 1, 1, 1}
    };

    const std::array<Vertex, 6> nZ_verts = {
        Vertex{1, -1, -1, 0, 0},
        Vertex{-1, -1, -1, 1, 0},
        Vertex{1, 1, -1, 0, 1},
        Vertex{-1, -1, -1, 1, 0},
        Vertex{1, 1, -1, 0, 1},
        Vertex{-1, 1, -1, 1, 1}
    };

    const std::array<Vertex, 6> pY_verts = {
        Vertex{-1, 1, 1, 0, 0},
        Vertex{1, 1, 1, 1, 0},
        Vertex{-1, 1, -1, 0, 1},
        Vertex{1, 1, 1, 1, 0},
        Vertex{-1, 1, -1, 0, 1},
        Vertex{1, 1, -1, 1, 1}
    };

    const std::array<Vertex, 6> nY_verts = {
        Vertex{-1, -1, 1, 0, 0},
        Vertex{1, -1, 1, 1, 0},
        Vertex{-1, -1, -1, 0, 1},
        Vertex{1, -1, 1, 1, 0},
        Vertex{-1, -1, -1, 0, 1},
        Vertex{1, -1, -1, 1, 1}
    };

};

#endif