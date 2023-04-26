#include "tournament.h"

using namespace std;

int main()
{
    PlayTournament(vector<shared_ptr<Player> >{
                        make_shared<RandomMove>(),
                        make_shared<CaptureLargest>(),
                        make_shared<CaptureWithLargestDifference>(),
                   },
                   100, "match_results.txt");
    return 0;
}
