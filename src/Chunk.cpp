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
    modelMatrix = glm::translate(modelMatrix, glm::vec3(world_x * CHUNK_X_SIZE * SCALE, 0, world_z * CHUNK_Z_SIZE * SCALE));
}

void Chunk::setData(ChunkData chunkData)
{
    data = chunkData;
}

ChunkData Chunk::getData()
{
    return data;
}

void Chunk::createMesh()
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
                testMeshSurroundingFaces(x, y, z);
            }
        }
    }

    mesh.endMeshGeneration();

}

void Chunk::testMeshSurroundingFaces(int x, int y, int z)
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
    else if (x == 0)
    {
        mesh.addMeshData(FaceDirection::nX, pos, SCALE);
    }

    if (x < data.size() - 1)
    {
        if (data[x + 1][z][y].type == 0)
        {
            mesh.addMeshData(FaceDirection::pX, pos, SCALE);
        }
    }
    else if (x == data.size() - 1)
    {
        mesh.addMeshData(FaceDirection::pX, pos, SCALE);
    }


    if (z > 0)
    {
        if (data[x][z - 1][y].type == 0)
        {
            mesh.addMeshData(FaceDirection::nZ, pos, SCALE);
        }
    }
    else if (z == 0)
    {
        mesh.addMeshData(FaceDirection::nZ, pos, SCALE);
    }

    if (z < data[0].size() - 1)
    {
        if (data[x][z + 1][y].type == 0)
        {
            mesh.addMeshData(FaceDirection::pZ, pos, SCALE);
        }
    }
    else if (z == data[0].size() - 1)
    {
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

    if (y < data[0][0].size() - 1)
    {
        if (data[x][z][y + 1].type == 0)
        {
            mesh.addMeshData(FaceDirection::pY, pos, SCALE);
        }
    }
    else if (y == data[0][0].size() - 1)
    {
        mesh.addMeshData(FaceDirection::pY, pos, SCALE);
    }


}

void Chunk::render(Shader& shader)
{

    shader.setMat4("model", modelMatrix);

    mesh.renderMesh();

}