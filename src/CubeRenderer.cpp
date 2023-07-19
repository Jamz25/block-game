#include "CubeRenderer.hpp"

void CubeRenderer::initialiseBuffers()
{

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertex_data.size() * sizeof(float), vertex_data.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicies.size() * sizeof(float), indicies.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);

}

void CubeRenderer::loadTextures()
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

void CubeRenderer::renderCube(Shader& shader, Block& block_data, glm::vec3 translation, glm::vec3 rotation, float rotation_amount, glm::vec3 scale)
{

    glm::mat4 modelMatrix(1);
    modelMatrix = glm::translate(modelMatrix, translation);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation_amount), rotation);
    modelMatrix = glm::scale(modelMatrix, scale);

    shader.setMat4("model", modelMatrix);

    glBindVertexArray(VAO);

    glBindTexture(GL_TEXTURE_2D, textureID);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, block_data.face_indexes.size() * sizeof(unsigned short), block_data.face_indexes.data(), GL_STATIC_DRAW);

    //glDrawElements(GL_TRIANGLES, block_data.face_indexes.size(), GL_UNSIGNED_SHORT, 0);
/*
    if (block_data.pZVisible)
    {
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
    }
    if (block_data.pXVisible)
    {
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)(6 * sizeof(unsigned short)));
    }
    if (block_data.nZVisible)
    {
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)(12 * sizeof(unsigned short)));
    }
    if (block_data.nXVisible)
    {
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)(18 * sizeof(unsigned short)));
    }
    if (block_data.pYVisible)
    {
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)(24 * sizeof(unsigned short)));
    }
    if (block_data.nYVisible)
    {
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)(30 * sizeof(unsigned short)));
    }
*/

    glBindVertexArray(0);

}