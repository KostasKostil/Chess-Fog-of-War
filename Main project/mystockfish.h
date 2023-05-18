#include "board.h"

namespace Stockfish
{
    std::pair<double, Move> Run(Board b, Color c, int depth);
    std::vector<std::pair<double, Move> > All(Board b, Color c, int depth);
}
