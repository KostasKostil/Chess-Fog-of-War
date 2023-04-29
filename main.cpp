#include "tournament.h"
#include "visual.h"

using namespace std;

void GlobalTournament()
{
    PlayTournament(vector<shared_ptr<Player> >{
                        make_shared<RandomMove>(),
                        make_shared<CaptureLargest>(),
                        make_shared<CaptureWithLargestDifference>(),
                        make_shared<PositionaryEvaluator>(),
                   },
                   100, "match_results.txt");
}

int main()
{
//    srand(47);
//    VisPlay(make_shared<PositionaryEvaluator>());

//    PlayGame(make_shared<PositionaryEvaluator>(), make_shared<PositionaryEvaluator>(), "game.txt");
//    Visualize("game.txt");

    GlobalTournament();

//    int n = 512;
//    vector<shared_ptr<ParametrizedPlayer> > v;
//    for (int i=0; i<n; i++)
//        v.push_back(make_shared<PositionaryEvaluator>());
//
//    int m = 5 + 6*64;
//    vector<double> init_params(m, 0);
//    {
//        ifstream fin("log.txt");
//        for (int i=0; i<m; i++)
//            fin>>init_params[i];
//    }
//    RunGeneticAlgorithm(v, init_params, n, 128, 1, 1.0);

    return 0;
}
