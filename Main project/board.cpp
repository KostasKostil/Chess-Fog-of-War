#include "board.h"

using namespace std;

Board InitialBoard()
{
    Board b;
    for (int i=0; i<8; i++)
        for (int j=0; j<8; j++)
    {
        if (i==1 || i==6) b[i][j].type = Type::Pawn;
        else b[i][j].type = Type::None;
        if (i<=1) b[i][j].color = Color::Black;
        else b[i][j].color = Color::White;
    }
    for (int i : {0, 7})
    {
        b[i][0].type = Type::Rook;
        b[i][1].type = Type::Knight;
        b[i][2].type = Type::Bishop;
        b[i][3].type = Type::Queen;
        b[i][4].type = Type::King;
        b[i][5].type = Type::Bishop;
        b[i][6].type = Type::Knight;
        b[i][7].type = Type::Rook;
    }
    return b;
}

Move::Move(Piece p, int x1, int y1, int x2, int y2)
{
    piece = p;
    from = Square{x1, y1};
    to = Square{x2, y2};
}

bool operator ==(const Move& m1, const Move& m2)
{
    return m1.piece.color == m2.piece.color && m1.piece.type == m2.piece.type
    && m1.from.x == m2.from.x && m1.from.y == m2.from.y;
}

vector<pair<int, int> > knight_moves = { {-2, -1}, {-2, 1}, {-1, -2}, {-1, 2}, {1, -2}, {1, 2}, {2, -1}, {2, 1} };
vector<pair<int, int> > bishop_moves = { {-1, -1}, {-1, 1}, {1, -1}, {1, 1} };
vector<pair<int, int> > rook_moves = { {-1, 0}, {0, -1}, {0, 1}, {1, 0} };
vector<pair<int, int> > king_moves = { {-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1} };

bool ok(int x, int y)
{
    return (x >= 0) && (x < 8) && (y >= 0) && (y < 8);
}

bool none(Piece p)
{
    return p.type == Type::None;
}

bool fog(Piece p)
{
    return p.type == Type::Fog;
}

bool my_col(Piece p, Color c)
{
    return !none(p) && !fog(p) && p.color == c;
}

bool oppos_col(Piece p, Color c)
{
    return !none(p) && !fog(p) && p.color != c;
}

std::vector<Move> GetMoves(const Board& b, Color c)
{
    vector<Move> moves;
    for (int i=0; i<8; i++)
        for (int j=0; j<8; j++)
            if (b[i][j].color == c)
            {
                if (b[i][j].type == Type::Pawn)
                {
                    if (c == Color::White)
                    {
                        if (ok(i-1, j) && none(b[i-1][j])) moves.push_back(Move(b[i][j], i, j, i-1, j));
                        if (ok(i-2, j) && i==6 && none(b[i-1][j]) && none(b[i-2][j])) moves.push_back(Move(b[i][j], i, j, i-2, j));
                        if (ok(i-1, j-1) && oppos_col(b[i-1][j-1], c)) moves.push_back(Move(b[i][j], i, j, i-1, j-1));
                        if (ok(i-1, j+1) && oppos_col(b[i-1][j+1], c)) moves.push_back(Move(b[i][j], i, j, i-1, j+1));
                    }
                    if (c == Color::Black)
                    {
                        if (ok(i+1, j) && none(b[i+1][j])) moves.push_back(Move(b[i][j], i, j, i+1, j));
                        if (ok(i+2, j) && i==1 && none(b[i+1][j]) && none(b[i+2][j])) moves.push_back(Move(b[i][j], i, j, i+2, j));
                        if (ok(i+1, j-1) && oppos_col(b[i+1][j-1], c)) moves.push_back(Move(b[i][j], i, j, i+1, j-1));
                        if (ok(i+1, j+1) && oppos_col(b[i+1][j+1], c)) moves.push_back(Move(b[i][j], i, j, i+1, j+1));
                    }
                }
                if (b[i][j].type == Type::Knight)
                {
                    for (auto pa : knight_moves)
                        if (ok(i + pa.first, j+pa.second) && !my_col(b[i+pa.first][j+pa.second], c))
                            moves.push_back(Move(b[i][j], i, j, i+pa.first, j+pa.second));
                }
                if (b[i][j].type == Type::Bishop || b[i][j].type == Type::Queen)
                {
                    for (auto pa : bishop_moves)
                        for (int k=1; ; k++)
                    {
                        if (ok(i+k*pa.first, j+k*pa.second) && !my_col(b[i+k*pa.first][j+k*pa.second], c))
                            moves.push_back(Move(b[i][j], i, j, i+k*pa.first, j+k*pa.second));
                        else
                            break;
                        if (!none(b[i+k*pa.first][j+k*pa.second]))
                            break;
                    }
                }
                if (b[i][j].type == Type::Rook || b[i][j].type == Type::Queen)
                {
                    for (auto pa : rook_moves)
                        for (int k=1; ; k++)
                    {
                        if (ok(i+k*pa.first, j+k*pa.second) && !my_col(b[i+k*pa.first][j+k*pa.second], c))
                            moves.push_back(Move(b[i][j], i, j, i+k*pa.first, j+k*pa.second));
                        else
                            break;
                        if (!none(b[i+k*pa.first][j+k*pa.second]))
                            break;
                    }
                }
                if (b[i][j].type == Type::King)
                {
                    for (auto pa : king_moves)
                        if (ok(i + pa.first, j+pa.second) && !my_col(b[i+pa.first][j+pa.second], c))
                            moves.push_back(Move(b[i][j], i, j, i+pa.first, j+pa.second));
                }
            }
    int cline = (c == Color::White) ? 7 : 0;
    if (b[cline][4].type ==  Type::King && b[cline][4].color == c)
    {
        if (none(b[cline][5]) && none(b[cline][6]) && (b[cline][7].type == Type::Rook) && (b[cline][7].color == c))
            moves.push_back(Move(b[cline][4], cline, 4, cline, 6));
        if (none(b[cline][3]) && none(b[cline][2]) && none(b[cline][1]) && (b[cline][0].type == Type::Rook) && (b[cline][0].color == c))
            moves.push_back(Move(b[cline][4], cline, 4, cline, 2));
    }
    return moves;
}

Board ApplyMove(const Board& b, Move m)
{
    Board brd = b;
    brd[m.to.x][m.to.y] = brd[m.from.x][m.from.y];
    brd[m.from.x][m.from.y].type = Type::None;
    if (brd[m.to.x][m.to.y].type == Type::Pawn && (m.to.x == 0 || m.to.x == 7))
        brd[m.to.x][m.to.y].type = Type::Queen;
    if (brd[m.to.x][m.to.y].type == Type::King && abs(m.to.y - m.from.y) == 2)
    {
        int m2y1 = (m.to.y == 6) ? 7 : 0;
        int m2y2 = (m.to.y + m.from.y) / 2;
        brd[m.to.x][m2y2] = brd[m.to.x][m2y1];
        brd[m.to.x][m2y1].type = Type::None;
    }
    return brd;
}

Board GetFoggedBoard(const Board& b, Color c)
{
    Board res;
    for (int i=0; i<8; i++)
        for (int j=0; j<8; j++)
        {
            res[i][j] = Piece{Color::White, Type::Fog};
            if (b[i][j].type != Type::None && b[i][j].color == c)
                res[i][j] = b[i][j];
        }
    for (auto m : GetMoves(b, c))
    {
        int x = m.to.x;
        int y = m.to.y;
        res[x][y] = b[x][y];
    }
    return res;
}

Board InvertBoard(const Board& b)
{
    Board res;
    for (int i=0; i<8; i++)
        for (int j=0; j<8; j++)
    {
        res[i][j] = b[7-i][j];
        if (res[i][j].color == Color::Black) res[i][j].color = Color::White;
        else res[i][j].color = Color::Black;
    }
    return res;
}

bool operator == (const Board& b1, const Board& b2)
{
    for (int i=0; i<8; i++)
        for (int j=0; j<8; j++)
    {
        if (b1[i][j].type == Type::Fog && b2[i][j].type == Type::Fog)
            continue;
        if (b1[i][j].type == Type::None && b2[i][j].type == Type::None)
            continue;
        if (b1[i][j].type == b2[i][j].type && b1[i][j].color == b2[i][j].color)
            continue;
        return false;
    }
    return true;
}

bool operator < (const Board& b1, const Board& b2)
{
    for (int i=0; i<8; i++)
        for (int j=0; j<8; j++)
    {
        if (b1[i][j].type == Type::Fog && b2[i][j].type == Type::Fog)
            continue;
        if (b1[i][j].type == Type::None && b2[i][j].type == Type::None)
            continue;
        if (b1[i][j].type == b2[i][j].type && b1[i][j].color == b2[i][j].color)
            continue;
        return b1[i][j].type < b2[i][j].type || (b1[i][j].type == b2[i][j].type && b1[i][j].color < b2[i][j].color);
    }
    return false;
}

std::ostream& operator << (std::ostream& out, const Board& b)
{
    for (int i=0; i<8; i++, out<<"\n")
        for (int j=0; j<8; j++)
    {
        if (b[i][j].type == Type::Fog)
            out<<"#";
        else
        if (b[i][j].type == Type::None)
            out<<".";
        else
        if (b[i][j].type == Type::Pawn)
            out<<"pP"[b[i][j].color == Color::White];
        else
        if (b[i][j].type == Type::Knight)
            out<<"nN"[b[i][j].color == Color::White];
        else
        if (b[i][j].type == Type::Bishop)
            out<<"bB"[b[i][j].color == Color::White];
        else
        if (b[i][j].type == Type::Rook)
            out<<"rR"[b[i][j].color == Color::White];
        else
        if (b[i][j].type == Type::Queen)
            out<<"qQ"[b[i][j].color == Color::White];
        else
        if (b[i][j].type == Type::King)
            out<<"kK"[b[i][j].color == Color::White];
    }
    return out;
}

