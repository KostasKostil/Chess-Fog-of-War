#include "board.h"

class Player
{
public:
    virtual void Initialize(Color color) = 0;
    virtual Move GetMove(const Board& b1, const Board& b2) = 0;
};

class RandomMove : public Player
{
public:
    void Initialize(Color color) override;
    Move GetMove(const Board& b1, const Board& b2) override;
private:
    Color c;
};

class CaptureLargest : public Player
{
public:
    void Initialize(Color color) override;
    Move GetMove(const Board& b1, const Board& b2) override;
private:
    Color c;
};

class CaptureWithLargestDifference : public Player
{
public:
    void Initialize(Color color) override;
    Move GetMove(const Board& b1, const Board& b2) override;
private:
    Color c;
};
