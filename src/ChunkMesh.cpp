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

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBindVertexArray(0);

}

void ChunkMesh::loadTextures()
{

    stbi_set_flip_vertically_on_load(true);

    int width, height, channels;
    unsigned char* data = stbi_load("dirt.png", &width, &height, &channels, 0);

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);

}

void ChunkMesh::beginMeshGeneration()
{
    generating = true;
    vertex_data_generating.clear();
}

void ChunkMesh::endMeshGeneration()
{
    generating = false;

    vertex_data = vertex_data_generating;
    vertex_data_generating.clear();

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertex_data.size(), vertex_data.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);

}

void ChunkMesh::addMeshData(FaceDirection faceDir, glm::vec3 local_offset, float scale)
{

    if (!generating)
        return;
    
    std::array<Vertex, 6> face_verts;
    float lightStrength;
    if (faceDir == FaceDirection::pX)
    {
        face_verts = pX_verts;
        lightStrength = 0.8;
    }
    else if (faceDir == FaceDirection::nX)
    {
        face_verts = nX_verts;
        lightStrength = 0.8;
    }
    else if (faceDir == FaceDirection::pZ)
    {
        face_verts = pZ_verts;
        lightStrength = 0.8;
    }
    else if (faceDir == FaceDirection::nZ)
    {
        face_verts = nZ_verts;
        lightStrength = 0.8;
    }
    else if (faceDir == FaceDirection::pY)
    {
        face_verts = pY_verts;
        lightStrength = 1;
    }
    else if (faceDir == FaceDirection::nY)
    {
        face_verts = nY_verts;
        lightStrength = 0.5;
    }

    for (Vertex vertex : face_verts)
    {
        vertex.x = vertex.x * scale + local_offset.x;
        vertex.y = vertex.y * scale + local_offset.y;
        vertex.z = vertex.z * scale + local_offset.z;
        vertex.lightStrength = lightStrength;
        vertex_data_generating.push_back(vertex);
    }

    //vertex_data_generating.insert(vertex_data_generating.end(), additional_vertex_data.begin(), additional_vertex_data.end());

}

void ChunkMesh::renderMesh()
{
    glBindVertexArray(VAO);

    glBindTexture(GL_TEXTURE_2D, textureID);

    glDrawArrays(GL_TRIANGLES, 0, vertex_data.size());

    glBindTexture(GL_TEXTURE_2D, 0);

    glBindVertexArray(0);
}