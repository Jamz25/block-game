#include "TextureLoader.hpp"

GLuint TextureLoader::blockTextureAtlas = 0;

void TextureLoader::loadTextures()
{
    stbi_set_flip_vertically_on_load(true);

    int width, height, channels;
    unsigned char* data = stbi_load("blockAtlas.png", &width, &height, &channels, 0);

    glGenTextures(1, &blockTextureAtlas);
    glBindTexture(GL_TEXTURE_2D, blockTextureAtlas);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    //glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);
}

void TextureLoader::bindBlockAtlas()
{
    glBindTexture(GL_TEXTURE_2D, blockTextureAtlas);
}

void TextureLoader::unbindBlockAtlas()
{
    glBindTexture(GL_TEXTURE_2D, blockTextureAtlas);
}

BlockTextureMap TextureLoader::getBlockTextureMap(BlockType blockType)
{
    return blockTextures.at(blockType);
}

TextureUVOffset TextureLoader::getBlockUVOffset(TextureType textureType)
{
    return blockAtlasUVs.at(textureType);
}