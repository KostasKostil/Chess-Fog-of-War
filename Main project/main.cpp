#include "tournament.h"
#include "visual.h"
#include "utils.h"
#include "mystockfish.h"

using namespace std;

void GlobalTournament()
{
    PlayTournament(vector<shared_ptr<Player> >{
                        make_shared<RandomMove>(),
                        make_shared<CaptureKing>(),
                        make_shared<CaptureLargest>(),
                        make_shared<CaptureWithLargestDifference>(),
                        make_shared<PositionaryEvaluator>(),
                        make_shared<NeuralNetworkPlayer>(),
                        make_shared<SimpleStockfish>(),
                        make_shared<PossibleBoardList>(),
                   },
                   100, "match_results.txt");
}

int main()
{
//    rl_Train();

//    PlayStockfish(3);

//    utils::GetDataset();
//    nn_Train();

//    VisPlay(make_shared<SimpleStockfish>());

//    PlayGame(make_shared<PossibleBoardList>(), make_shared<PossibleBoardList>(), "game.txt");
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

//    int n = 64;
//    vector<shared_ptr<ParametrizedPlayer> > v1, v2;
//    for (int i=0; i<n; i++)
//    {
//        v1.push_back(make_shared<PositionaryEvaluator>());
//        v2.push_back(make_shared<PositionaryEvaluator>());
//    }
//    int m = 5 + 6*64;
//    vector<double> init_params(m, 0);
//    RunGeneticAlgorithmPair(v1, v2, init_params, n, 8, 1.0);



    return 0;
}
