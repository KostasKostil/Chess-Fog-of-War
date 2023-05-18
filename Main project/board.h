#pragma once
#include <bits/stdc++.h>

enum Color
{
    Black,
    White
};

enum class Type
{
    Fog,
    None,
    Pawn,
    Knight,
    Bishop,
    Rook,
    Queen,
    King
};

struct Piece
{
    Color color;
    Type type;
};

typedef std::array<std::array<Piece, 8>, 8> Board;
Board InitialBoard();

struct Square
{
    int x;
    int y;
};
struct Move
{
    Piece piece;
    Square from;
    Square to;
    Move() = default;
    Move(Piece p, int x1, int y1, int x2, int y2);
    friend bool operator ==(const Move&, const Move&);
};

std::vector<Move> GetMoves(const Board& b, Color c);
Board ApplyMove(const Board& b, Move m);
Board GetFoggedBoard(const Board& b, Color c);
Board InvertBoard(const Board& b);
bool operator == (const Board& b1, const Board& b2);
bool operator < (const Board& b1, const Board& b2);
std::ostream& operator << (std::ostream& out, const Board& b);
