#include "mystockfish.h"

using namespace std;

namespace Stockfish
{
double EvaluatePiece(Type t, int x, int y)
{
    // x == 0 => white side
    if (t == Type::Pawn) return 1+0.01*(x-1);
    if (t == Type::Knight) return 3-0.1*sqrt(pow(abs(x-3.5), 2) + pow(abs(y-3.5), 2));
    if (t == Type::Bishop) return 3-0.05*sqrt(pow(abs(x-3.5), 2) + pow(abs(y-3.5), 2));
    if (t == Type::Rook) return 5-0.01*abs(y-3.5);
    if (t == Type::Queen) return 8-0.2*sqrt(pow(abs(x-3.5), 2) + pow(abs(y-3.5), 2));
    if (t == Type::King) return 1000-0.02*x;
    return 0;
}

double EvaluatePosition(Board b, Color c)
{
    double res = (rand()*1.0/RAND_MAX-0.5)*0.001;
    for (int i=0; i<8; i++)
        for (int j=0; j<8; j++)
    {
        if (b[i][j].type != Type::Fog && b[i][j].type != Type::None)
        {
            if (b[i][j].color == Color::White)
                res += EvaluatePiece(b[i][j].type, 7-i, j);
            else
                res -= EvaluatePiece(b[i][j].type, i, j);
        }
    }
    return res;
}

double Evaluate(Board b, Color c, int depth, Square last)
{
    if (GetMoves(b, c).empty())
        return EvaluatePosition(b, c);
    for (Move m : GetMoves(b, c))
        if (b[m.to.x][m.to.y].type == Type::King)
            return (c == Color::White) ? 1000*(100 + depth) : -1000*(100 + depth);

    if (depth <= 0)
    {
    double ans = EvaluatePosition(b, c);

    for (Move m : GetMoves(b, c))
        if (m.to.x == last.x && m.to.y == last.y)
        {
            double r = Evaluate(ApplyMove(b, m), (c == Color::White) ? Color::Black : Color::White, depth-1, m.to);
            if (c == Color::White) ans = max(ans, r);
            else ans = min(ans, r);
        }
    return ans;
    }

    double ans = (c == Color::White) ? -1e9 : 1e9;

    for (Move m : GetMoves(b, c))
    {
        double r = Evaluate(ApplyMove(b, m), (c == Color::White) ? Color::Black : Color::White, depth-1, m.to);
        if (c == Color::White) ans = max(ans, r);
        else ans = min(ans, r);
    }
    return ans;
}

std::pair<double, Move> Run(Board b, Color c, int depth)
{
    double ans = (c == Color::White) ? -1e9 : 1e9;
    if (GetMoves(b, c).empty())
        return {EvaluatePosition(b, c), Move{}};

    Move best;
    for (Move m : GetMoves(b, c))
        if (b[m.to.x][m.to.y].type == Type::King)
            return (c == Color::White) ? std::pair<double, Move>{1e9, m} : std::pair<double, Move>{-1e9, m};

    for (Move m : GetMoves(b, c))
    {
        double r = Evaluate(ApplyMove(b, m), (c == Color::White) ? Color::Black : Color::White, depth-1, m.to);
        if (c == Color::White)
        {
            if (r > ans)
            {
                ans = max(ans, r);
                best = m;
            }
        }
        else
        {
            if (r < ans)
            {
                ans = min(ans, r);
                best = m;
            }
        }
    }
    return {ans, best};
}

std::vector<std::pair<double, Move> > All(Board b, Color c, int depth)
{
    vector<pair<double, Move> > ans;

    for (Move m : GetMoves(b, c))
    {
        double r = (b[m.to.x][m.to.y].type == Type::King) ? ((c == Color::White) ? 1e9 : -1e9)
                 :  Evaluate(ApplyMove(b, m), (c == Color::White) ? Color::Black : Color::White, depth-1, m.to);
        ans.push_back({r, m});
    }
    return ans;
}

}
