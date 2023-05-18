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
//    int iter = 0;
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

void RunGeneticAlgorithmPair(std::vector<std::shared_ptr<ParametrizedPlayer> > players1, std::vector<std::shared_ptr<ParametrizedPlayer> > players2,
                             std::vector<double> default_params, int population, int rounds, double step)
{
    int n = population;
    vector<vector<double> > a1(n, default_params);
    vector<vector<double> > a2(n, default_params);
    int iter = 0;
    while (true)
    {
        for (int i=0; i<n; i++)
        {
            players1[i]->SetParameters(a1[i]);
            players2[i]->SetParameters(a2[i]);
        }
        if ((iter++)%100 == 0)
        {
            cout<<"Iteration.\n";
            {
                ofstream fout("log1.txt");
                fout<<fixed<<setprecision(10);
                for (double d : a1[0])
                    fout<<d<<"\n";
            }
            {
                ofstream fout("log2.txt");
                fout<<fixed<<setprecision(10);
                for (double d : a2[0])
                    fout<<d<<"\n";
            }
            {
                MatchResult mr1 = PlayMatch(players1[0], players2[0], 1000);
                cout<<"Res1: "<<mr1.wins<<"/"<<mr1.draws<<"/"<<mr1.loses<<"\n";
            }
            {
                MatchResult mr1 = PlayMatch(players1[0], make_shared<CaptureLargest>(), 1000);
                MatchResult mr2 = PlayMatch(players1[0], make_shared<CaptureWithLargestDifference>(), 1000);
                cout<<"Res1: "<<mr1.wins<<"/"<<mr1.draws<<"/"<<mr1.loses<<", "<<mr2.wins<<"/"<<mr2.draws<<"/"<<mr2.loses<<"\n";
            }
            {
                MatchResult mr1 = PlayMatch(players2[0], make_shared<CaptureLargest>(), 1000);
                MatchResult mr2 = PlayMatch(players2[0], make_shared<CaptureWithLargestDifference>(), 1000);
                cout<<"Res2: "<<mr1.wins<<"/"<<mr1.draws<<"/"<<mr1.loses<<", "<<mr2.wins<<"/"<<mr2.draws<<"/"<<mr2.loses<<"\n";
            }
        }

        // genetic algo itself
        vector<int> score1(n), score2(n);
        for (int i=0; i<n; i++)
        {
            MatchResult mr1 = PlayMatch(players1[i], players2[0], rounds);
            MatchResult mr2 = PlayMatch(players2[0], players1[i], rounds);
            score1[i]+=2*mr1.wins;
            score1[i]+=  mr1.draws;
            score1[i]+=2*mr2.loses;
            score1[i]+=  mr2.draws;
        }
        for (int i=0; i<n; i++)
        {
            MatchResult mr1 = PlayMatch(players2[i], players1[0], rounds);
            MatchResult mr2 = PlayMatch(players1[0], players2[i], rounds);
            score2[i]+=2*mr1.wins;
            score2[i]+=  mr1.draws;
            score2[i]+=2*mr2.loses;
            score2[i]+=  mr2.draws;
        }

        vector<int> sorted1(n);
        vector<int> sorted2(n);
        for (int i=0; i<n; i++)
            sorted1[i] = sorted2[i] = i;
        sort(sorted1.begin(), sorted1.end(), [&](int x, int y){ return score1[x] > score1[y]; });
        sort(sorted2.begin(), sorted2.end(), [&](int x, int y){ return score2[x] > score2[y]; });

        a1[0] = a1[sorted1[0]];
        a2[0] = a2[sorted2[0]];
        for (int i=1; i<n; i++)
        {
            a1[i] = a1[0];
            for (double& d : a1[i])
                d += ((rand()*1.0/RAND_MAX) - 0.5) * step;

            a2[i] = a2[0];
            for (double& d : a2[i])
                d += ((rand()*1.0/RAND_MAX) - 0.5) * step;
        }
    }
}
