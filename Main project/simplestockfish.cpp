#include "players.h"
#include "mystockfish.h"

using namespace std;

void SimpleStockfish::Initialize(Color color)
{
    c = color;
}

Move SimpleStockfish::GetMove(const Board& b0, const Board& b)
{
    return Stockfish::Run(b, c, 3).second;
}

