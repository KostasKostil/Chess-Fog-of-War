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

class CaptureKing : public Player
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

class Supporter : public Player
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
void RunGeneticAlgorithmPair(std::vector<std::shared_ptr<ParametrizedPlayer> > players1, std::vector<std::shared_ptr<ParametrizedPlayer> > players2,
                             std::vector<double> default_params, int population, int rounds, double step);

class PositionaryEvaluator : public ParametrizedPlayer
{
public:
    void Initialize(Color color) override;
    Move GetMove(const Board& b0, const Board& b) override;
private:
    Color c;
};

class SimpleStockfish : public Player
{
public:
    void Initialize(Color color) override;
    Move GetMove(const Board& b0, const Board& b) override;
private:
    Color c;
};

Board GenerateBoard(Board previousBoard, Board foggedBoard, Color c);
class PossibleBoardGenerator : public Player
{
public:
    void Initialize(Color color) override;
    Move GetMove(const Board& b0, const Board& b) override;
private:
    Board possibleBoard;
    Color c;
};

class PossibleBoardList : public Player
{
public:
    void Initialize(Color color) override;
    Move GetMove(const Board& b0, const Board& b) override;
private:
    std::vector<Board> possibleBoards;
    std::vector<Board> lastpossibleBoards;
    bool isfirstmove;
    Move mylastmove;
    Color c;
    std::shared_ptr<Player> otherwise;
};

std::vector<double> PieceToVector(Piece p);
std::vector<double> BoardToVector(Board b);
std::vector<double> MoveToVector(Move m);
std::vector<double> MoveToVector(Move m, Color c);
void rl_Train();
void nn_Train();

class NeuralNetworkPlayer : public Player
{
public:
    void Initialize(Color color) override;
    Move GetMove(const Board& b0, const Board& b) override;
private:
    Color c;
};
