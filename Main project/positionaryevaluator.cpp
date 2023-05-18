#include "players.h"

using namespace std;

void PositionaryEvaluator::Initialize(Color color)
{
    c = color;

    vector<double> init_params(5 + 6*64, 0);
    ifstream fin("positionaryevaluator_middle.txt");
    for (double& d : init_params)
        fin>>d;
    params = init_params;
}

int pe_id(Type t)
{
    if (t == Type::Pawn) return 0;
    if (t == Type::Knight) return 1;
    if (t == Type::Bishop) return 2;
    if (t == Type::Rook) return 3;
    if (t == Type::Queen) return 4;
    if (t == Type::King) return 5;
    return 0;
}

Move PositionaryEvaluator::GetMove(const Board& b0, const Board& b)
{
    vector<pair<Move, double> > v;
    for (Move m : GetMoves(b, c))
    {
        double score = 0;
        if (b[m.to.x][m.to.y].type == Type::King)
            return m;

        Type t = m.piece.type;
        int cfrom = 8*((c == Color::White) ? m.from.x : (7 - m.from.x)) + m.from.y;
        int cto   = 8*((c == Color::White) ? m.to.x   : (7 - m.to.x))   + m.to.y;
        score -= params[ 5 + 64 * pe_id(t) + cfrom ];
        score += params[ 5 + 64 * pe_id(t) + cto ];
        if (b[m.to.x][m.to.y].type != Type::None && b[m.to.x][m.to.y].type != Type::Fog)
            score += params[pe_id(b[m.to.x][m.to.y].type)];

        v.push_back({m, exp(score)});
    }

    double s = 0;
    for (auto& pa : v)
        s += pa.second;
    s = s*rand()*1.0/RAND_MAX;
    for (auto& pa : v)
    {
        s -= pa.second;
        if (s < 0)
            return pa.first;
    }
    return v.back().first;
}
