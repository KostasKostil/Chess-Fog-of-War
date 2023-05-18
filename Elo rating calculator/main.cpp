#include <bits/stdc++.h>
#define nmax 100
using namespace std;

struct MatchResult
{
    int wins;
    int draws;
    int loses;
};

int n;
MatchResult mr[nmax][nmax];

bool digit(char c)
{
    return c>='0' and c<='9';
}
void Read(string filename)
{
    string cur;
    vector<string> input;
    ifstream fin(filename);
    while (getline(fin, cur))
        input.push_back(cur), n++;
    for (int i=0; i<n; i++)
    {
        stringstream ss(input[i]);
        for (int j=0; j<n; j++)
            if (i!=j)
        {
            while (!isdigit(ss.peek())) ss.ignore(1);
            ss>>mr[i][j].wins;
            while (!isdigit(ss.peek())) ss.ignore(1);
            ss>>mr[i][j].draws;
            while (!isdigit(ss.peek())) ss.ignore(1);            ss>>mr[i][j].loses;
        }
    }
}

double K =  1.0;
double rating[nmax];
double new_rating[nmax];
double Iteration() // returns total change
{
    for (int i=0; i<n; i++)
        new_rating[i] = rating[i];
    for (int i=0; i<n; i++)
        for (int j=0; j<n; j++)
            if (i!=j)
    {
        double ea = 1.0/(1+pow(10, (rating[j]-rating[i])/400));

        new_rating[i] += K*(mr[i][j].wins*(1-ea) + mr[i][j].draws*(0.5-ea) + mr[i][j].loses*(0-ea));
        new_rating[j] -= K*(mr[i][j].wins*(1-ea) + mr[i][j].draws*(0.5-ea) + mr[i][j].loses*(0-ea));
    }
    double res = 0;
    for (int i=0; i<n; i++)
    {
        res += abs(rating[i] - new_rating[i]);
        rating[i] = new_rating[i];
    }
    return res;
}

int main()
{
    Read("match_results.txt");
    while (Iteration() > 1e-4)
    {
        for (int i=0; i<n; i++)
            cout<<rating[i]<<" ";
        cout<<"\n";
    }
    for (int i=0; i<n; i++)
        cout<<rating[i] - rating[0]<<" ";
    cout<<"\n";

    return 0;
}
