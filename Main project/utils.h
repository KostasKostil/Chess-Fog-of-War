#pragma once
#include <vector>
#include "board.h"

namespace utils
{

Board Invert(Board b);
Move Invert(Move m);

struct PlayerState
{
int queen = 1;
int rooks = 2;
int bishop_sum0 = 1;
int bishop_sum1 = 1;
int knigts = 2;
int pawns = 8;
};

struct Test
{
    Board b;
    Move m;
};

inline std::vector<Test> dataset;
void GetDataset();

}
