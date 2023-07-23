#include "ChunkMesh.hpp"

void ChunkMesh::initialiseBuffers()
{

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, u));
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, lightStrength));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

}

void ChunkMesh::beginMeshGeneration()
{
    generating = true;
    vertex_data_generating.clear();
    vertex_indexes.clear();
}

void ChunkMesh::endMeshGeneration()
{
    generating = false;

    vertex_data = vertex_data_generating;
    vertex_data_generating.clear();

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertex_data.size(), vertex_data.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * vertex_indexes.size(), vertex_indexes.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);

}

void ChunkMesh::addMeshData(BlockType blockType, FaceDirection faceDir, glm::vec3 local_offset, float scale)
{

    if (!generating)
        return;
    
    BlockTextureMap blockTextureMap = TextureLoader::getBlockTextureMap(blockType);

    std::array<Vertex, 4> face_verts;
    TextureUVOffset faceUVOffset;
    float lightStrength;
    if (faceDir == FaceDirection::pX)
    {
        face_verts = pX_verts;
        lightStrength = 0.6;

        // Load side uv coords
        faceUVOffset = TextureLoader::getBlockUVOffset(blockTextureMap.side);
    }
    else if (faceDir == FaceDirection::nX)
    {
        face_verts = nX_verts;
        lightStrength = 0.6;

        // Load side uv coords
        faceUVOffset = TextureLoader::getBlockUVOffset(blockTextureMap.side);
    }
    else if (faceDir == FaceDirection::pZ)
    {
        face_verts = pZ_verts;
        lightStrength = 0.6;

        // Load side uv coords
        faceUVOffset = TextureLoader::getBlockUVOffset(blockTextureMap.side);
    }
    else if (faceDir == FaceDirection::nZ)
    {
        face_verts = nZ_verts;
        lightStrength = 0.6;

        // Load side uv coords
        faceUVOffset = TextureLoader::getBlockUVOffset(blockTextureMap.side);
    }
    else if (faceDir == FaceDirection::pY)
    {
        face_verts = pY_verts;
        lightStrength = 0.9;

        // Load top uv coords
        faceUVOffset = TextureLoader::getBlockUVOffset(blockTextureMap.top);
    }
    else if (faceDir == FaceDirection::nY)
    {
        face_verts = nY_verts;
        lightStrength = 0.5;

        // Load bottom uv coords
        faceUVOffset = TextureLoader::getBlockUVOffset(blockTextureMap.bottom);
    }

    //std::cout << faceUVOffset.u << ", " << faceUVOffset.v << std::endl;

    for (Vertex vertex : face_verts)
    {
        // Apply transform to set face position correctly in local chunk space
        vertex.x = vertex.x * scale + local_offset.x;
        vertex.y = vertex.y * scale + local_offset.y;
        vertex.z = vertex.z * scale + local_offset.z;

        // Apply UV coords offset (based on block type)
        vertex.u = vertex.u * BLOCK_ATLAS_X_MULTIPLIER + faceUVOffset.u;
        vertex.v = vertex.v * BLOCK_ATLAS_Y_MULTIPLIER + faceUVOffset.v;

        // Apply light strength (based on face direction)
        vertex.lightStrength = lightStrength;

        vertex_data_generating.push_back(vertex);
    }

    // Push face indexes into EBO
    vertex_indexes.push_back(vertex_data_generating.size() - 4);
    vertex_indexes.push_back(vertex_data_generating.size() - 3);
    vertex_indexes.push_back(vertex_data_generating.size() - 2);
    vertex_indexes.push_back(vertex_data_generating.size() - 3);
    vertex_indexes.push_back(vertex_data_generating.size() - 2);
    vertex_indexes.push_back(vertex_data_generating.size() - 1);

}

void ChunkMesh::renderMesh()
{
    glBindVertexArray(VAO);

    glDrawElements(GL_TRIANGLES, vertex_indexes.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}