#ifndef CHUNK_HPP
#define CHUNK_HPP

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
#include "Block.hpp"
#include "ChunkMesh.hpp"

#define CHUNK_X_SIZE 16
#define CHUNK_Z_SIZE 16
#define CHUNK_Y_SIZE 50

typedef std::array<std::array<std::array<Block, CHUNK_Y_SIZE>, CHUNK_Z_SIZE>, CHUNK_X_SIZE> ChunkData;

class Chunk
{
public:
    Chunk() = default;

    void initialiseMesh();

    void setWorldPosition(int x, int z);

    void setData(ChunkData chunkData);
    ChunkData getData();

    void createMesh();
    void render(Shader& shader);

private:
    void testMeshSurroundingFaces(int x, int y, int z);

private:
    const float SCALE = 0.1;

    ChunkData data;
    ChunkMesh mesh;

    int world_x, world_z;
    glm::mat4 modelMatrix;
};

#endif