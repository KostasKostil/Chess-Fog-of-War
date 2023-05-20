#include "players.h"
#include "mystockfish.h"
#define TIME_LIMIT 5.0

using namespace std;

#define fi first
#define se second

void PossibleBoardGenerator::Initialize(Color color)
{
    c = color;
    possibleBoard = InitialBoard();
}

double score[8][8];
vector<pair<int, int> > tiles = [](){
    vector<pair<int, int> > v;
    for (int i=0; i<8; i++)
        for (int j=0; j<8; j++)
            v.push_back({i, j});
    return v;
}();
vector<pair<int, int> > tiles0 = [](){
    vector<pair<int, int> > v;
    for (int i=0; i<8; i++)
        for (int j=0; j<8; j++)
            v.push_back({i, j});
    return v;
}();

Board GenerateBoard(Board previousBoard, Board foggedBoard, Color c)
{
    set<pair<int, int> > fog;
    set<pair<int, int> > need;
    for (int i=0; i<8; i++)
        for (int j=0; j<8; j++)
            if (foggedBoard[i][j].type == Type::Fog)
                fog.insert({i, j});

    for (int i=0; i<8; i++)
        for (int j=0; j<8; j++)
            if (foggedBoard[i][j].type == Type::Pawn && foggedBoard[i][j].color == c)
            {
                int dx = (c == Color::White) ? -1 : 1;
                fog.erase({i+dx, j-1});
                fog.erase({i+dx, j+1});
                if (fog.count({i+dx, j}))
                    fog.erase({i+dx, j}), need.insert({i+dx, j});
                if (i == ((c == Color::White) ? 6 : 1))
                {
                    if (foggedBoard[i+dx][j].type == Type::None)
                        if (fog.count({i+2*dx, j}))
                            fog.erase({i+2*dx, j}), need.insert({i+2*dx, j});
                }
            }
    Board result;
    for (int i=0; i<8; i++)
        for (int j=0; j<8; j++)
            result[i][j].type = Type::None;
    for (int i=0; i<8; i++)
        for (int j=0; j<8; j++)
            if (foggedBoard[i][j].type != Type::Fog && foggedBoard[i][j].type != Type::None && foggedBoard[i][j].color == c)
            {
                result[i][j] = foggedBoard[i][j];
                previousBoard[i][j].type = Type::None;
            }

    random_shuffle(tiles0.begin(), tiles0.end());
    for (auto pa : tiles0)
        if (foggedBoard[pa.first][pa.second].type != Type::None && foggedBoard[pa.first][pa.second].type != Type::Fog
         && foggedBoard[pa.first][pa.second].color != c)
    {
        for (int i=0; i<8; i++)
            for (int j=0; j<8; j++)
                score[i][j] = abs((i-pa.first)*(i-pa.first) + (j-pa.second)*(j-pa.second)) + rand()*0.5/RAND_MAX;
        sort(tiles.begin(), tiles.end(), [](pair<int, int> x, pair<int, int> y){return score[x.fi][x.se] < score[y.fi][y.se]; });
        for (auto pax : tiles)
            if (previousBoard[pax.fi][pax.se].type == foggedBoard[pa.fi][pa.se].type
             && previousBoard[pax.fi][pax.se].color == foggedBoard[pa.fi][pa.se].color)
        {
            result[pa.fi][pa.se] = previousBoard[pax.fi][pax.se];
            previousBoard[pax.fi][pax.se].type = Type::None;
            break;
        }
    }

    for (auto pa : need)
    {
        for (int i=0; i<8; i++)
            for (int j=0; j<8; j++)
                score[i][j] = abs((i-pa.first)*(i-pa.first) + (j-pa.second)*(j-pa.second)) + rand()*0.5/RAND_MAX;
        sort(tiles.begin(), tiles.end(), [](pair<int, int> x, pair<int, int> y){return score[x.fi][x.se] < score[y.fi][y.se]; });
        for (auto pax : tiles)
        {
            Piece p = previousBoard[pax.fi][pax.se];
            if (p.type != Type::None && p.color != c)
            {
                result[pax.fi][pax.se] = p;
                previousBoard[pax.fi][pax.se].type = Type::None;
                break;
            }
        }
    }

    random_shuffle(tiles0.begin(), tiles0.end());
    for (auto pa : tiles0)
        if (previousBoard[pa.first][pa.second].type != Type::None && previousBoard[pa.first][pa.second].color != c)
    {
        for (int i=0; i<8; i++)
            for (int j=0; j<8; j++)
                score[i][j] = abs((i-pa.first)*(i-pa.first) + (j-pa.second)*(j-pa.second)) + rand()*0.5/RAND_MAX;
        sort(tiles.begin(), tiles.end(), [](pair<int, int> x, pair<int, int> y){return score[x.fi][x.se] < score[y.fi][y.se]; });
        for (auto pax : tiles)
            if (fog.count(pax))
        {
            result[pax.fi][pax.se] = previousBoard[pa.fi][pa.se];
            previousBoard[pa.fi][pa.se].type = Type::None;
            fog.erase(pax);
            break;
        }
    }

    return result;
}

Move PossibleBoardGenerator::GetMove(const Board& b0, const Board& b)
{
    possibleBoard = GenerateBoard(possibleBoard, b, c);

    Move m = Stockfish::Run(possibleBoard, c, 3).second;
    possibleBoard = ApplyMove(possibleBoard, m);
    return m;
}

