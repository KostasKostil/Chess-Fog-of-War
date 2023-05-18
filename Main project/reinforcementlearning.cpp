#include "players.h"
#include "kknn.h"

using namespace std;

#define pb push_back

KKNN decisionmaker;

std::vector<double> PieceToVector(Piece p)
{
    vector<double> res;
    if (p.type == Type::Fog) res.pb(1); else res.pb(0);
    if (p.type == Type::None) res.pb(1); else res.pb(0);

    if (p.type == Type::Pawn && p.color == Color::White) res.pb(1); else res.pb(0);
    if (p.type == Type::Knight && p.color == Color::White) res.pb(1); else res.pb(0);
    if (p.type == Type::Bishop && p.color == Color::White) res.pb(1); else res.pb(0);
    if (p.type == Type::Rook && p.color == Color::White) res.pb(1); else res.pb(0);
    if (p.type == Type::Queen && p.color == Color::White) res.pb(1); else res.pb(0);
    if (p.type == Type::King && p.color == Color::White) res.pb(1); else res.pb(0);

    if (p.type == Type::Pawn && p.color == Color::Black) res.pb(1); else res.pb(0);
    if (p.type == Type::Knight && p.color == Color::Black) res.pb(1); else res.pb(0);
    if (p.type == Type::Bishop && p.color == Color::Black) res.pb(1); else res.pb(0);
    if (p.type == Type::Rook && p.color == Color::Black) res.pb(1); else res.pb(0);
    if (p.type == Type::Queen && p.color == Color::Black) res.pb(1); else res.pb(0);
    if (p.type == Type::King && p.color == Color::Black) res.pb(1); else res.pb(0);

    return res;
}

std::vector<double> BoardToVector(Board b)
{
    vector<double> res;
    for (int i=0; i<8; i++)
        for (int j=0; j<8; j++)
            for (double k : PieceToVector(b[i][j]))
                res.pb(k);
    return res;
}

std::vector<double> MoveToVector(Move m)
{
    vector<double> res;
    for (int i=0; i<8; i++)
        for (int j=0; j<8; j++)
            if (m.from.x == i && m.from.y == j) res.pb(1); else res.pb(0);
    for (int i=0; i<8; i++)
        for (int j=0; j<8; j++)
            if (m.to.x == i && m.to.y == j) res.pb(1); else res.pb(0);
    return res;
}

std::vector<double> MoveToVector(Move m, Color c)
{
    if (c == Color::Black)
        m.from.x = 7 - m.from.x, m.to.x = 7 - m.to.x;
    return MoveToVector(m);
}

Move rl_MakeMove(Board b, Color c, double pick_coeff = 0.2)
{
    vector<double> res = decisionmaker.Run(BoardToVector(c == Color::White ? b : InvertBoard(b)));
    vector<pair<double, Move> > moves;
    for (Move m_ : GetMoves(b, c))
    {
        Move m = m_;
        if (c == Color::Black)
            m.from.x = 7 - m.from.x, m.to.x = 7 - m.to.x;
        double s0 = res[ 0 + 8*m.from.x + m.from.y];
        double s1 = res[64 + 8*m.to.x + m.to.y];
        moves.pb({s0*s1, m_});
    }
    sort(moves.begin(), moves.end(), [&](auto x, auto y){ return x.first > y.first; });
    while (true)
        for (auto pa : moves)
            if (rand()*1.0/RAND_MAX < pick_coeff)
                return pa.second;
}

void rl_Create()
{
    decisionmaker = KKNN({64*14, 64, 64, 64*2});
}

void rl_Save()
{
    decisionmaker.Save("rl.txt");
}

void rl_Read()
{
    decisionmaker.Load("rl.txt");
}

vector<vector<double> > inputs;
vector<vector<double> > outputs;
vector<double> case_w;

void rl_PlayGame()
{
    Board b = InitialBoard();

    vector<vector<double> > white_inputs;
    vector<vector<double> > black_inputs;
    vector<vector<double> > white_outputs;
    vector<vector<double> > black_outputs;
    bool victory;
    while (true)
    {
        {
            Move m = rl_MakeMove(GetFoggedBoard(b, Color::White), Color::White);
            white_inputs.pb(BoardToVector(GetFoggedBoard(b, Color::White)));
            white_outputs.pb(MoveToVector(m, Color::White));
            if (b[m.to.x][m.to.y].type == Type::King)
            {
                victory = true;
                break;
            }
            b = ApplyMove(b, m);
        }
        {
            Move m = rl_MakeMove(GetFoggedBoard(b, Color::Black), Color::Black);
            black_inputs.pb(BoardToVector(InvertBoard(GetFoggedBoard(b, Color::Black))));
            black_outputs.pb(MoveToVector(m, Color::Black));
            if (b[m.to.x][m.to.y].type == Type::King)
            {
                victory = false;
                break;
            }
            b = ApplyMove(b, m);
        }
    }
    if (victory)
    {
        reverse(white_inputs.begin(), white_inputs.end());
        reverse(white_outputs.begin(), white_outputs.end());
        for (size_t i=0; i<white_inputs.size(); i++)
        {
            inputs.pb(white_inputs[i]);
            outputs.pb(white_outputs[i]);
            case_w.pb(pow(2, -i));
        }
    }
    else
    {
        reverse(black_inputs.begin(), black_inputs.end());
        reverse(black_outputs.begin(), black_outputs.end());
        for (size_t i=0; i<black_inputs.size(); i++)
        {
            inputs.pb(black_inputs[i]);
            outputs.pb(black_outputs[i]);
            case_w.pb(pow(2, -i));
        }
    }}

void rl_Test()
{
    Board b = InitialBoard();

    b[1][4].type = Type::None;

    b[5][4].type = Type::Rook;
    b[5][4].color = Color::White;

    b[5][5].type = Type::Knight;
    b[5][5].color = Color::Black;

    b[5][2].type = Type::Rook;
    b[5][2].color = Color::Black;

    cout<<fixed<<setprecision(2);
    cout<<b<<"\n";
    auto res = decisionmaker.Run(BoardToVector(GetFoggedBoard(b, Color::White)));
    for (int _=0; _<2; _++, cout<<"\n")
        for (int i=0; i<8; i++, cout<<"\n")
            for (int j=0; j<8; j++)
                cout<<res[64*_+8*i+j]<<" ";
}

void rl_Train()
{
    rl_Create();
    while (true)
    {
        if (inputs.size() > 500)
        {
            decisionmaker.RunTestset(inputs, outputs);
            rl_Save();

            inputs.clear();
            outputs.clear();

            rl_Test();
        }

        rl_PlayGame();
    }
}
