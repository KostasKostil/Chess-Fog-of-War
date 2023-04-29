#pragma once
#include "board.h"

class Player
{
public:
    virtual void Initialize(Color color) = 0;
    virtual Move GetMove(const Board& b0, const Board& b) = 0;
};

class RandomMove : public Player
{
public:
    void Initialize(Color color) override;
    Move GetMove(const Board& b0, const Board& b) override;
private:
    Color c;
};

class CaptureLargest : public Player
{
public:
    void Initialize(Color color) override;
    Move GetMove(const Board& b0, const Board& b) override;
private:
    Color c;
};

class CaptureWithLargestDifference : public Player
{
public:
    void Initialize(Color color) override;
    Move GetMove(const Board& b0, const Board& b) override;
private:
    Color c;
};

class ParametrizedPlayer : public Player
{
public:
    void SetParameters(std::vector<double> parameters);
protected:
    std::vector<double> params;
};

void RunGeneticAlgorithm(std::vector<std::shared_ptr<ParametrizedPlayer> > players, std::vector<double> default_params, int population, int victorious, int rounds, double step);
void RunGeneticAlgorithm(std::vector<std::shared_ptr<ParametrizedPlayer> > players, std::vector<double> default_params, int population, int victorious, int rounds, std::shared_ptr<Player>, double step);

class PositionaryEvaluator : public ParametrizedPlayer
{
public:
    void Initialize(Color color) override;
    Move GetMove(const Board& b0, const Board& b) override;
private:
    Color c;
};
