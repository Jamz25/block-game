#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <vector>

struct Block
{
    int type;
    bool pXVisible = true;
    bool nXVisible = true;
    bool pYVisible = true;
    bool nYVisible = true;
    bool pZVisible = true;
    bool nZVisible = true;

    std::vector<unsigned short> face_indexes;
};

#endif