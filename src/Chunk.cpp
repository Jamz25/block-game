#include "Chunk.hpp"

void Chunk::initialiseMesh()
{
    mesh.initialiseBuffers();
    mesh.loadTextures();
}

void Chunk::setWorldPosition(int x, int z)
{
    world_x = x;
    world_z = z;

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(world_x * CHUNK_X_SIZE * SCALE * 2, 0, world_z * CHUNK_Z_SIZE * SCALE * 2));
}

void Chunk::setData(const ChunkData& chunkData)
{
    data = chunkData;
}

const ChunkData& Chunk::getData()
{
    return data;
}

void Chunk::createMesh(Chunk* pXChunk, Chunk* nXChunk, Chunk* pZChunk, Chunk* nZChunk)
{

    mesh.initialiseBuffers();
    mesh.loadTextures();

    mesh.beginMeshGeneration();

    // testing visible faces
    for (int x = 0; x < CHUNK_X_SIZE; x++)
    {
        for (int z = 0; z < CHUNK_Z_SIZE; z++)
        {
            for (int y = 0; y < CHUNK_Y_SIZE; y++)
            {
                testMeshSurroundingFaces(x, y, z, pXChunk, nXChunk, pZChunk, nZChunk);
            }
        }
    }

    mesh.endMeshGeneration();

}

void Chunk::testMeshSurroundingFaces(int x, int y, int z, Chunk* pXChunk, Chunk* nXChunk, Chunk* pZChunk, Chunk* nZChunk)
{

    Block& block = data[x][z][y];

    if (block.type == 0)
        return;
    
    glm::vec3 pos = glm::vec3(x * SCALE * 2, y * SCALE * 2, z * SCALE * 2);

    if (x > 0)
    {
        if (data[x - 1][z][y].type == 0)
        {
            mesh.addMeshData(FaceDirection::nX, pos, SCALE);
        }
    }
    else if (x == 0 && nXChunk != nullptr)
    {
        if (nXChunk->getData()[CHUNK_X_SIZE - 1][z][y].type == 0)
            mesh.addMeshData(FaceDirection::nX, pos, SCALE);
    }

    if (x < CHUNK_X_SIZE - 1)
    {
        if (data[x + 1][z][y].type == 0)
        {
            mesh.addMeshData(FaceDirection::pX, pos, SCALE);
        }
    }
    else if (x == CHUNK_X_SIZE - 1 && pXChunk != nullptr)
    {
        if (pXChunk->getData()[0][z][y].type == 0)
            mesh.addMeshData(FaceDirection::pX, pos, SCALE);
    }


    if (z > 0)
    {
        if (data[x][z - 1][y].type == 0)
        {
            mesh.addMeshData(FaceDirection::nZ, pos, SCALE);
        }
    }
    else if (z == 0 && nZChunk != nullptr)
    {
        if (nZChunk->getData()[x][CHUNK_Z_SIZE - 1][y].type == 0)
            mesh.addMeshData(FaceDirection::nZ, pos, SCALE);
    }
    

    if (z < CHUNK_Z_SIZE - 1)
    {
        if (data[x][z + 1][y].type == 0)
        {
            mesh.addMeshData(FaceDirection::pZ, pos, SCALE);
        }
    }
    else if (z == CHUNK_Z_SIZE - 1 && pZChunk != nullptr)
    {
        if (pZChunk->getData()[x][0][y].type == 0)
            mesh.addMeshData(FaceDirection::pZ, pos, SCALE);
    }


    if (y > 0)
    {
        if (data[x][z][y - 1].type == 0)
        {
            mesh.addMeshData(FaceDirection::nY, pos, SCALE);
        }
    }
    else if (y == 0)
    {
        mesh.addMeshData(FaceDirection::nY, pos, SCALE);
    }

    if (y < CHUNK_Y_SIZE - 1)
    {
        if (data[x][z][y + 1].type == 0)
        {
            mesh.addMeshData(FaceDirection::pY, pos, SCALE);
        }
    }
    else if (y == CHUNK_Y_SIZE - 1)
    {
        mesh.addMeshData(FaceDirection::pY, pos, SCALE);
    }


}

void Chunk::render(Shader& shader)
{

    shader.setMat4("model", modelMatrix);

    mesh.renderMesh();

}