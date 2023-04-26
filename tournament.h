#include "players.h"

std::ofstream& operator << (std::ofstream& fout, Move m);
int PlayGame(std::shared_ptr<Player> w, std::shared_ptr<Player> b);
int PlayGame(std::shared_ptr<Player> w, std::shared_ptr<Player> b, std::string filename);
struct MatchResult
{
    int wins;
    int draws;
    int loses;
};
MatchResult PlayMatch(std::shared_ptr<Player> w, std::shared_ptr<Player> b, int cntgames);
void PlayTournament(std::vector<std::shared_ptr<Player> > v, int cntgames, std::string filename);
