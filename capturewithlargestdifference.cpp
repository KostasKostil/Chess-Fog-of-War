#include "players.h"

using namespace std;

int cwldvalue(Type t)
{
    if (t == Type::Pawn) return 1;
    if (t == Type::Knight) return 3;
    if (t == Type::Bishop) return 3;
    if (t == Type::Rook) return 5;
    if (t == Type::Queen) return 10;
    if (t == Type::King) return 100;
    return 0;
}

void CaptureWithLargestDifference::Initialize(Color color)
{
    c = color;
}

Move CaptureWithLargestDifference::GetMove(const Board& b0, const Board& b)
{
    auto v = GetMoves(b, c);
    sort(v.begin(), v.end(), [&](const Move& l, const Move& r){ return cwldvalue(b[l.to.x][l.to.y].type) - cwldvalue(b[l.from.x][l.from.y].type)
                                                                     > cwldvalue(b[r.to.x][r.to.y].type) - cwldvalue(b[r.from.x][r.from.y].type); });
    for (size_t i=1; i<v.size(); i++)
        if (cwldvalue(b[v[i].to.x][v[i].to.y].type) - cwldvalue(b[v[i].from.x][v[i].from.y].type)
         != cwldvalue(b[v[i-1].to.x][v[i-1].to.y].type))
            return v[rand()%i];
    return v[rand()%v.size()];
}

