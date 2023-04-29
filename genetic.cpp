#include "tournament.h"

using namespace std;

void ParametrizedPlayer::SetParameters(std::vector<double> parameters)
{
    params = parameters;
}

void RunGeneticAlgorithm(std::vector<std::shared_ptr<ParametrizedPlayer> > players, std::vector<double> default_params, int population, int victorious, int rounds, double step)
{
    double temp = 1.0;
    int n = population;
    int m = victorious;
    vector<vector<double> > a(n, default_params);
    int iter = 0;
    while (true)
    {
        for (int i=0; i<n; i++)
            players[i]->SetParameters(a[i]);
        if ((iter++)%1 == 0)
        {
            MatchResult mr1 = PlayMatch(players[0], make_shared<CaptureLargest>(), 1000);
            MatchResult mr2 = PlayMatch(players[0], make_shared<CaptureWithLargestDifference>(), 1000);
            cout<<"Iteration. Res: "<<mr1.wins<<"/"<<mr1.draws<<"/"<<mr1.loses<<", "<<mr2.wins<<"/"<<mr2.draws<<"/"<<mr2.loses<<"\n";
            ofstream fout("log.txt");
            fout<<fixed<<setprecision(10);
            for (double d : a[0])
                fout<<d<<"\n";
        }

        vector<int> score(n);
        for (int i=0; i<n; i++)
            for (int j=0; j<n; j++)
                if (i!=j)
                {
                    MatchResult mr = PlayMatch(players[i], players[j], rounds);
                    score[i]+=2*mr.wins;
                    score[i]+=  mr.draws;
                    score[j]+=2*mr.loses;
                    score[j]+=  mr.draws;
                }

        vector<int> sorted(n);
        for (int i=0; i<n; i++)
            sorted[i] = i;
        sort(sorted.begin(), sorted.end(), [&](int x, int y){ return score[x] > score[y]; });

        vector<vector<double> > b(n);
        for (int i=0; i<m; i++)
            b[i] = a[sorted[i]];
        a = b;
        for (int i=m; i<n; i++)
        {
            a[i] = a[rand()%m];
            for (double& d : a[i])
                d += ((rand()*1.0/RAND_MAX) - 0.5) * step * temp;
        }
    }
}

void RunGeneticAlgorithm(std::vector<std::shared_ptr<ParametrizedPlayer> > players, std::vector<double> default_params, int population, int victorious, int rounds, std::shared_ptr<Player> against, double step)
{
    double temp = 1.0;
    int n = population;
    int m = victorious;
    vector<vector<double> > a(n, default_params);
    int iter = 0;
    while (true)
    {
        for (int i=0; i<n; i++)
            players[i]->SetParameters(a[i]);

        ofstream fout("log.txt");
        fout<<fixed<<setprecision(10);
        for (double d : a[0])
            fout<<d<<"\n";

        vector<int> score(n);
        for (int i=0; i<n; i++)
        {
            MatchResult mr1 = PlayMatch(players[i], against, rounds);
            MatchResult mr2 = PlayMatch(against, players[i], rounds);
            score[i]+=2*mr1.wins;
            score[i]+=  mr1.draws;
            score[i]+=2*mr2.loses;
            score[i]+=  mr2.draws;
        }

        vector<int> sorted(n);
        for (int i=0; i<n; i++)
            sorted[i] = i;
        sort(sorted.begin(), sorted.end(), [&](int x, int y){ return score[x] > score[y]; });

        vector<vector<double> > b(n);
        for (int i=0; i<m; i++)
            b[i] = a[sorted[i]];
        cout<<score[sorted[0]]<<"/"<<4*rounds<<"\n";

        a = b;
        for (int i=m; i<n; i++)
        {
            a[i] = a[rand()%m];
            for (double& d : a[i])
                d += ((rand()*1.0/RAND_MAX) - 0.5) * step * temp;
        }
    }
}
