#include "tournament.h"

using namespace std;

std::ofstream& operator << (std::ofstream& fout, Move m)
{
    if (m.piece.type==Type::Knight) fout<<"N";
    if (m.piece.type==Type::Bishop) fout<<"B";
    if (m.piece.type==Type::Rook) fout<<"R";
    if (m.piece.type==Type::Queen) fout<<"Q";
    if (m.piece.type==Type::King) fout<<"K";
    fout<<"abcdefgh"[m.from.y];
    fout<<8-m.from.x;
    fout<<"-";
    fout<<"abcdefgh"[m.to.y];
    fout<<8-m.to.x;
    return fout;
}

int PlayGame(std::shared_ptr<Player> w, std::shared_ptr<Player> b)
{
    w->Initialize(Color::White);
    b->Initialize(Color::Black);

    Board prev_brd = InitialBoard();
    Board brd = InitialBoard();
    int moves_no_cap = 0;
    int whites_turn = 1;
    int total_moves = 1;

    while (moves_no_cap < 200)
    {
        moves_no_cap++;

        Color c = whites_turn ? Color::White : Color::Black;
        vector<Move> v = GetMoves(brd, c);

        Move m;
        if (whites_turn) m = w->GetMove(GetFoggedBoard(prev_brd, Color::White), GetFoggedBoard(brd, Color::White));
        else             m = b->GetMove(GetFoggedBoard(prev_brd, Color::Black), GetFoggedBoard(brd, Color::Black));

        bool ok = false;
        for (auto p : v)
            if (m == p)
                ok = true;
        if (!ok)
        {
            cout<<"Wrong move\n";
            if (whites_turn)
                return -2;
            else
                return 2;
        }

        if (brd[m.to.x][m.to.y].type == Type::King)
        {
            if (whites_turn)
                return 1;
            else
                return -1;
        }

        if (brd[m.to.x][m.to.y].type != Type::None || brd[m.from.x][m.from.y].type == Type::Pawn)
            moves_no_cap = 0;
        prev_brd = brd;
        brd = ApplyMove(brd, m);

        if (!whites_turn)
            total_moves++;
        whites_turn = 1 - whites_turn;
    }
    return 0;
}

int PlayGame(std::shared_ptr<Player> w, std::shared_ptr<Player> b, std::string filename)
{
    ofstream fout(filename);

    w->Initialize(Color::White);
    b->Initialize(Color::Black);

    Board prev_brd = InitialBoard();
    Board brd = InitialBoard();
    int moves_no_cap = 0;
    int whites_turn = 1;
    int total_moves = 1;

    while (moves_no_cap < 200)
    {
        moves_no_cap++;

        Color c = whites_turn ? Color::White : Color::Black;
        vector<Move> v = GetMoves(brd, c);

        Move m;
        if (whites_turn) m = w->GetMove(GetFoggedBoard(prev_brd, Color::White), GetFoggedBoard(brd, Color::White));
        else             m = b->GetMove(GetFoggedBoard(prev_brd, Color::Black), GetFoggedBoard(brd, Color::Black));

        bool ok = false;
        for (auto p : v)
            if (m == p)
                ok = true;
        if (!ok)
        {
            fout.close();
            exit(0);
            if (whites_turn)
                return -2;
            else
                return 2;
        }
        if (whites_turn) fout<<total_moves<<". ";
        fout<<m<<" \n"[1-whites_turn];

        if (brd[m.to.x][m.to.y].type == Type::King)
        {
            if (whites_turn)
                return 1;
            else
                return -1;
        }

        if (brd[m.to.x][m.to.y].type != Type::None || brd[m.from.x][m.from.y].type == Type::Pawn)
            moves_no_cap = 0;
        prev_brd = brd;
        brd = ApplyMove(brd, m);

        if (!whites_turn)
            total_moves++;
        whites_turn = 1 - whites_turn;
    }
    return 0;
}

MatchResult PlayMatch(std::shared_ptr<Player> w, std::shared_ptr<Player> b, int cntgames)
{
    MatchResult m{0, 0, 0};
    for (int i=0; i<cntgames; i++)
    {
        int r = PlayGame(w, b);
        if (r > 0) m.wins++;
        else if (r == 0) m.draws++;
        else m.loses++;
    }
    return m;
}

void PlayTournament(std::vector<std::shared_ptr<Player> > v, int cntgames, std::string filename)
{
    ofstream fout(filename);

    int n = v.size();
    for (int i=0; i<n; i++, fout<<"\n")
        for (int j=0; j<n; j++)
    {
        if (i == j)
            fout<<"(---/---/---) ";
        else
        {
            MatchResult mr = PlayMatch(v[i], v[j], cntgames);
            fout<<"("<<setw(3)<<mr.wins<<"/"<<setw(3)<<mr.draws<<"/"<<setw(3)<<mr.loses<<") ";
            fout.flush();
        }
    }
}
