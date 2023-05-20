#include "players.h"

using namespace std;

double mysupvalue(Type t)
{
    if (t == Type::Pawn) return 0;
    if (t == Type::Knight) return 1;
    if (t == Type::Bishop) return 1;
    if (t == Type::Rook) return 1;
    if (t == Type::Queen) return 1;
    if (t == Type::King) return 0;
    return 0;
}
double opsupvalue(Type t)
{
    if (t == Type::Pawn) return 1;
    if (t == Type::Knight) return 3;
    if (t == Type::Bishop) return 3;
    if (t == Type::Rook) return 5;
    if (t == Type::Queen) return 10;
    if (t == Type::King) return 100;
    return 0;
}

void Supporter::Initialize(Color color)
{
    c = color;
}

Move Supporter::GetMove(const Board& b0, const Board& b)
{
    double res = -1e9;
    Move best;
    for (Move m : GetMoves(b, c))
    {
        double score = opsupvalue(b[m.to.x][m.to.y].type) + rand()*1e-6/RAND_MAX;
        Board bx = ApplyMove(b, m);
        for (int i=0; i<8; i++)
            for (int j=0; j<8; j++)
                if (bx[i][j].type != Type::None && bx[i][j].type != Type::Fog && bx[i][j].color == c)
        {
            Board bx0 = bx;
            bx0[i][j].type = Type::Pawn;
            bx0[i][j].color = (c == Color::White) ? Color::Black : Color::White;
            for (Move mx : GetMoves(bx0, c))
                if (mx.to.x == i && mx.to.y == j)
                {
                    score += mysupvalue(bx[i][j].type);
                    break;
                }
        }

        if (score > res)
        {
            res = score;
            best = m;
        }
    }
    return best;
}

