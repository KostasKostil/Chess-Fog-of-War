#include "players.h"
#include "mystockfish.h"
#define TIME_LIMIT 0.2

using namespace std;

void PossibleBoardList::Initialize(Color color)
{
    c = color;
    possibleBoards = { InitialBoard() };
    isfirstmove = true;
    otherwise = make_shared<SimpleStockfish>();
    otherwise -> Initialize(c);
}

Move PossibleBoardList::GetMove(const Board& b0, const Board& b)
{
    if (!isfirstmove)
    { // our last move
        vector<Board> nb;
        for (Board b : possibleBoards)
            if (GetFoggedBoard(ApplyMove(b, mylastmove), c) == b0)
                nb.push_back(ApplyMove(b, mylastmove));
        possibleBoards = nb;
    }
    if (possibleBoards.empty())
        return otherwise->GetMove(b0, b);
    if (!isfirstmove || c==Color::Black)
    { // opponent last move
        vector<Board> nb;
        auto t = clock();
        while ((clock() - t)*1.0/CLOCKS_PER_SEC < TIME_LIMIT * 0.45)
        {
            Board bx = possibleBoards[rand()%possibleBoards.size()];
            vector<Move> mx = GetMoves(bx, c==Color::Black ? Color::White : Color::Black);
            Move m = mx[rand()%mx.size()];
            Board newb = ApplyMove(bx, m);
            if (GetFoggedBoard(newb, c) == b)
                nb.push_back(newb);
        }
        sort(nb.begin(), nb.end());
        nb.erase(unique(nb.begin(), nb.end()), nb.end());
        random_shuffle(nb.begin(), nb.end());
        possibleBoards = nb;
    }
    if (possibleBoards.empty())
        return otherwise->GetMove(b0, b);

    cout<<possibleBoards.size()<<"\n";

    vector<Move> moves = GetMoves(b, c);
    int n = moves.size();
    vector<double> rating(n, c == Color::White ? 1e18 : -1e18);

    auto t = clock();
    for (Board b : possibleBoards)
        if ((clock() - t)*1.0/CLOCKS_PER_SEC < TIME_LIMIT * 0.45)
    {
        auto v = Stockfish::All(b, c, 2);
        for (int i=0; i<n; i++)
        {
            double r = v[i].first;
            if (c == Color::White)
                rating[i] = min(rating[i], r);
            else
                rating[i] = max(rating[i], r);
        }
    }

    double resultrating = rating[0];
    Move resultmove = moves[0];
    for (int i=1; i<n; i++)
        if ((c==Color::White && rating[i] > resultrating) || (c==Color::Black && rating[i] < resultrating))
            resultrating = rating[i], resultmove = moves[i];

    isfirstmove = false;
    mylastmove = resultmove;
    return resultmove;
}

