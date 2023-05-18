#include "players.h"

using namespace std;

int clvalue(Type t)
{
    if (t == Type::Pawn) return 1;
    if (t == Type::Knight) return 3;
    if (t == Type::Bishop) return 3;
    if (t == Type::Rook) return 5;
    if (t == Type::Queen) return 10;
    if (t == Type::King) return 100;
    return 0;
}

void CaptureLargest::Initialize(Color color)
{
    c = color;
}

Move CaptureLargest::GetMove(const Board& b0, const Board& b)
{
    auto v = GetMoves(b, c);
    sort(v.begin(), v.end(), [&](const Move& l, const Move& r){ return clvalue(b[l.to.x][l.to.y].type) > clvalue(b[r.to.x][r.to.y].type); });
    for (size_t i=1; i<v.size(); i++)
        if (clvalue(b[v[i].to.x][v[i].to.y].type)!=clvalue(b[v[i-1].to.x][v[i-1].to.y].type))
            return v[rand()%i];
    return v[rand()%v.size()];
}

