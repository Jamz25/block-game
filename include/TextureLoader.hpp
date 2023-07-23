#ifndef TEXTURE_LOADER_HPP
#define TEXTURE_LOADER_HPP

#include <unordered_map>
#include <array>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include "Block.hpp"

#define BLOCK_ATLAS_X_COUNT 4
#define BLOCK_ATLAS_Y_COUNT 1
#define BLOCK_ATLAS_X_MULTIPLIER (1.0f / BLOCK_ATLAS_X_COUNT)
#define BLOCK_ATLAS_Y_MULTIPLIER (1.0f / BLOCK_ATLAS_Y_COUNT)

enum class TextureType
{
    GrassTop, GrassSide,
    Dirt,
    Stone
};

struct TextureUVOffset
{
    float u, v;
};

struct BlockTextureMap
{
    TextureType top, side, bottom;
};

class TextureLoader
{
public:
    static void loadTextures();

    static void bindBlockAtlas();
    static void unbindBlockAtlas();

    static BlockTextureMap getBlockTextureMap(BlockType blockType);

    static TextureUVOffset getBlockUVOffset(TextureType textureType);

private:
    TextureLoader();

    static GLuint blockTextureAtlas;

    static const inline std::unordered_map<TextureType, TextureUVOffset> blockAtlasUVs = {
        {TextureType::GrassTop, TextureUVOffset{0, 0}},
        {TextureType::GrassSide, TextureUVOffset{1.0f * BLOCK_ATLAS_X_MULTIPLIER, 0}},
        {TextureType::Dirt, TextureUVOffset{2.0f * BLOCK_ATLAS_X_MULTIPLIER, 0}},
        {TextureType::Stone, TextureUVOffset{3.0f * BLOCK_ATLAS_X_MULTIPLIER, 0}}
    };

    static const inline std::unordered_map<BlockType, BlockTextureMap> blockTextures = {
        {BlockType::Grass, BlockTextureMap{TextureType::GrassTop, TextureType::GrassSide, TextureType::Dirt}},
        {BlockType::Dirt, BlockTextureMap{TextureType::Dirt, TextureType::Dirt, TextureType::Dirt}},
        {BlockType::Stone, BlockTextureMap{TextureType::Stone, TextureType::Stone, TextureType::Stone}}
    };

};

#endif