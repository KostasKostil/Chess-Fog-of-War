#include "players.h"

using namespace std;

void CaptureKing::Initialize(Color color)
{
    c = color;
}

Move CaptureKing::GetMove(const Board& b0, const Board& b)
{
    auto v = GetMoves(b, c);
    for (auto m : v)
        if (b[m.to.x][m.to.y].type == Type::King)
            return m;
    return v[rand()%v.size()];
}

