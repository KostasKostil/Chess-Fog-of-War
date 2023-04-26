#include "players.h"

void RandomMove::Initialize(Color color)
{
    c = color;
}

Move RandomMove::GetMove(const Board& b0, const Board& b)
{
    auto v = GetMoves(b, c);
    int r = rand()%v.size();
    return v[r];
}
