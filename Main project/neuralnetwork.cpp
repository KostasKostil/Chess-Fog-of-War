#include "players.h"
#include "kknn.h"
#include "utils.h"

using namespace std;

#define pb push_back

KKNN nn;

void nn_Create()
{
    nn = KKNN({64*14, 64, 64, 64*2});
    nn.SetEducationCoeff(0.001);
}

void nn_Save()
{
    nn.Save("nn.txt");
}

void nn_Read()
{
    nn.Load("nn.txt");
}

void nn_Train()
{
//    nn_Create();
    nn_Read();

    vector<vector<double> > inputs, outputs;
    for (size_t i = 0; i < utils::dataset.size(); i++)
    {
        inputs.pb(BoardToVector(GetFoggedBoard(utils::dataset[i].b, Color::White)));
        outputs.pb(MoveToVector(utils::dataset[i].m));
    }

    while (true)
    {
        cout<<nn.RunTestset(inputs, outputs)<<"\n";
        for (int i=0; i<10; i++)
        {
            int C = 10;
            int s = inputs.size();
            for (int i=0; i<C; i++)
            {
                vector<vector<double> > vin, vout;
                for (int j=s*i/C; j<s*(i+1)/C; j++)
                    vin.pb(inputs[j]), vout.pb(outputs[j]);
                nn.RunTestset(vin, vout);
            }
        }
        nn_Save();
    }
}

void NeuralNetworkPlayer::Initialize(Color color)
{
    c = color;
    nn_Read();
}

Move NeuralNetworkPlayer::GetMove(const Board& b0, const Board& b)
{
    auto v = GetMoves(b, c);
    for (auto m : v)
        if (b[m.to.x][m.to.y].type == Type::King)
            return m;

    double score = -1;
    Move best;
    vector<double> res = nn.Run(BoardToVector(c == Color::White ? b : InvertBoard(b)));
    for (auto m_ : v)
    {
        Move m = m_;
        if (c == Color::Black)
            m.from.x = 7 - m.from.x, m.to.x = 7 - m.to.x;
        double s0 = res[ 0 + 8*m.from.x + m.from.y];
        double s1 = res[64 + 8*m.to.x + m.to.y];
        if (s0*s1 > score)
        {
            score = s0*s1;
            best = m_;
        }
    }
    return best;
}
