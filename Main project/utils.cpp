#include "utils.h"
#include "board.h"
#include <bits/stdc++.h>

using namespace std;

namespace utils
{
Board Invert(Board b)
{
    Board b2;
    for (int i=0; i<8; i++)
        for (int j=0; j<8; j++)
    {
        b2[i][j] = b[7-i][j];
        if (b2[i][j].color == Color::White)
            b2[i][j].color = Color::Black;
        else
            b2[i][j].color = Color::White;
    }
    return b2;
}
Move Invert(Move m)
{
    m.from.x = 7 - m.from.x;
    m.to.x = 7 - m.to.x;
    return m;
}

Move Parse(string s)
{
    if (s.size() < 4)
    {
        exit(0);
    }
    int en = s.size() - 1;
    if (s[en] == 'Q')
        en-=2;

    Move m;
    m.from = Square{8-(s[en-2]-'0'), (s[en-3]-'a')};
    m.to = Square{8-(s[en]-'0'), (s[en-1]-'a')};
    return m;
}

void GetDataset()
{
    vector<Test> res;
    map<string, int> stats;
    int wlog = 0, blog = 0, n = 100;
    for (int i=0; i<n; i++)
    {
        Board b = InitialBoard();

        stringstream fname; fname<<"games/"<<i<<".pgn";
        ifstream fin(fname.str());

        bool white = false;
        bool black = false;

        string s;
        for (int j=0; j<14; j++)
        {
            getline(fin, s);

            if (j == 6 && s == "[Black \"oferwald\"]")
                black = true;
            if (j == 4 && s == "[White \"oferwald\"]")
                white = true;
        }
        if (white == black)
            cout<<"Warning: oferwald was not white xor black.\n";
        wlog += white;
        blog += black;
        while (getline(fin, s))
        {
            stringstream ss;
            ss<<s;
            string no, wh, bl;
            ss>>no>>wh>>bl;
            if (wh == "1-0" || wh == "0-1" || wh.size() < 4)
                break;
            Move m = Parse(wh);
            if (white)
                res.push_back(Test{b, m});
            b = ApplyMove(b, m);
            stats[wh]++;

            if (bl == "1-0" || bl == "0-1" || bl.size() < 4)
                break;
            m = Parse(bl);
            if (black)
                res.push_back(Test{Invert(b), Invert(m)});
            b = ApplyMove(b, m);
            stats[bl]++;
        }
    }

    cout<<n<<" games loaded. "<<wlog<<" with white, "<<blog<<" with black.\n";
    cout<<"Most popular moves:\n";
    vector<pair<int, string> > v;
    for (auto pa : stats)
        v.push_back({pa.second, pa.first});
    sort(v.begin(), v.end());
    reverse(v.begin(), v.end());
    for (auto pa : v)
        if (pa.first > 50)
            cout<<pa.first<<" "<<pa.second<<"\n";
    dataset = res;
}

}
