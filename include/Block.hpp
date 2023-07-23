#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <vector>

enum class BlockType
{
    Air,
    Grass,
    Dirt,
    Stone
};

struct Block
{
    BlockType type;

    /*
    bool pXVisible = true;
    bool nXVisible = true;
    bool pYVisible = true;
    bool nYVisible = true;
    bool pZVisible = true;
    bool nZVisible = true;

    std::vector<unsigned short> face_indexes;
    */
};

#endif