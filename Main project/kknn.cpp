#include "kknn.h"
#include <bits/stdc++.h>

using namespace std;

default_random_engine generator;
normal_distribution<double> distribution(0.0, 1.0);

double random_weight(double r)
{
    return distribution(generator)* r;
}
double sigmoid(double x)
{
    return 1/(1+exp(-x));
}
double sigmoid_derivative(double x)
{
    return x*(1-x);
}
double relu(double x)
{
    return (x > 0) ? x : 0;
}
double relu_derivative(double x)
{
    return(x > 0) ? 1 : 0;
}

/**
 * CONSTRUCTORS
 */

KKNN::KKNN(int total_n)
{
    setN(total_n);
}
KKNN::KKNN(std::vector<int> layers)
{
    int s = 0;
    for (int i : layers) s+=i;
    setN(s);
    s = 0;
    for (size_t i=0; i+1<layers.size(); i++)
    {
        int x = s + layers[i];
        int y = x + layers[i+1];
        AddDenseLayerTransition(s, x, y);
        s = x;
    }
}

/**
 * CREATION
 */

void KKNN::AddEdge(int u, int v)
{
    g[v].push_back(BackwardEdge{u, NewWeight(0.1)});
}
void KKNN::AddEdge(int u, int v, int wid)
{
    g[v].push_back(BackwardEdge{u, wid});
}
void KKNN::AddDenseLayerTransition(int l1, int r1, int l2, int r2)
{
    for (int i=l1; i<r1; i++)
        for (int j=l2; j<r2; j++)
            AddEdge(i, j, NewWeight(1.0/sqrt(r1-l1)) );
}
void KKNN::AddDenseLayerTransition(int l, int m, int r)
{
    AddDenseLayerTransition(l, m, m, r);
}
std::vector<std::vector<int> > KKNN::AddEmbeddingMatrix(int w, int h)
{
    vector<vector<int> > v(w, vector<int>(h));
    for (int i=0; i<w; i++)
        for (int j=0; j<h; j++)
            v[i][j] = NewWeight(1.0/sqrt(w*h));
    return v;
}

int KKNN::NewWeight(double r)
{
    w.push_back(Weight{random_weight(r), 0});
    return w.size() - 1;
}
void KKNN::SetActivation(int i, Activation act)
{
    a[i] = act;
}
void KKNN::SetEducationCoeff(double coeff)
{
    educationCoeff = coeff;
}


/**
 * RUNNING
 */

std::vector<double> KKNN::Run(std::vector<double> input)
{
    int m = input.size();
    for (int i=0; i<m; i++)
        a[i] = Activation::None;
    input.resize(n);
    for (int i=m; i<n; i++)
    {
        if (a[i] == Activation::MaxPooling)
        {
            double x = -1e18;
            for (auto e : g[i])
                x = max(x, input[e.from]);
            input[i] = x;
        }
        else
        {
            double x = 0;
            for (auto e : g[i])
                x += w[e.wid].weight * input[e.from];

            if (a[i] == Activation::None)    input[i] = x;
            if (a[i] == Activation::Sigmoid) input[i] = sigmoid(x+b[i].weight);
            if (a[i] == Activation::Relu)    input[i] = relu(x+b[i].weight);
        }
    }
    reverse(input.begin(), input.end());
    return input;
}
double KKNN::SingleTestcase(std::vector<double> input, std::vector<double> target_output, double case_w)
{
    double err = 0;
    auto output = Run(input);
    int m = target_output.size();
    target_output.resize(n);
    reverse(output.begin(), output.end());
    reverse(target_output.begin(), target_output.end());

    vector<double> delta(n);
    for (int i=n-1; i>=0; i--)
    {
    // calculating delta
        if (i >= n-m)
        {
            delta[i] = target_output[i] - output[i];
            err += delta[i] * delta[i];
        }
        if (a[i] == Activation::Sigmoid) delta[i] *= sigmoid_derivative(output[i]);
        if (a[i] == Activation::Relu)    delta[i] *= relu_derivative(output[i]);

    // backpropagation delta
        if (a[i] == Activation::MaxPooling)
        {
            double x = -1e18;
            for (auto e : g[i])
                x = max(x, output[e.from]);
            for (auto e : g[i])
                if (output[e.from] == x)
                    delta[e.from] += delta[i];
        }
        else
        {
            for (auto e : g[i])
                delta[e.from] += delta[i] * w[e.wid].weight;
        }
    }
    for (int i=0; i<n; i++)
    {
        b[i].cumulative += delta[i] * case_w;
        for (auto e : g[i])
            w[e.wid].cumulative += delta[i] * output[e.from] * case_w;
    }
    return err;
}
void KKNN::InitializeTestset()
{
    for (auto& bi : b)
        bi.cumulative = 0;
    for (auto& wi : w)
        wi.cumulative = 0;
}
void KKNN::FinishTestset(int cnt)
{
    for (auto& bi : b)
        bi.weight += bi.cumulative * educationCoeff;
    for (auto& wi : w)
        wi.weight += wi.cumulative * educationCoeff;
}
double KKNN::RunTestset(std::vector<std::vector<double> > input, std::vector<std::vector<double> > target_output, std::vector<double> case_w)
{
    double sum = 0;
    int m = min(input.size(), target_output.size());
    if (m == 0)
        throw 404;
    while (int(case_w.size()) < m)
        case_w.push_back(1);

    InitializeTestset();
    for (int i=0; i<m; i++)
        sum += SingleTestcase(input[i], target_output[i], case_w[i]);
    FinishTestset(m);

    return sum / m;
}
void KKNN::Save(std::string file_name)
{
    ofstream out(file_name);
    out<<fixed<<setprecision(20);
    out<<n<<" "<<w.size()<<"\n";
    for(int i=0; i<n; i++)
    {
        out<<g[i].size();
        for (auto e : g[i])
            out<<" "<<e.from<<" "<<e.wid;
        out<<"\n";
    }
    for (int i=0; i<n; i++)
    {
        if (a[i] == Activation::None)       out<<0<<"\n";
        if (a[i] == Activation::Sigmoid)    out<<1<<"\n";
        if (a[i] == Activation::Relu)       out<<2<<"\n";
        if (a[i] == Activation::MaxPooling) out<<3<<"\n";
    }
    for (int i=0; i<n; i++)
        out<<b[i].weight<<"\n";
    for (int i=0; i<int(w.size()); i++)
        out<<w[i].weight<<"\n";
}
void KKNN::Load(std::string file_name)
{
    ifstream in(file_name);
    int m;
    in>>n>>m;
    setN(n);
    for (int i=0; i<n; i++)
    {
        int q;
        in>>q;
        g[i].resize(q);
        for (auto& e : g[i])
            in>>e.from>>e.wid;
    }
    int x;
    for (int i=0; i<n; i++)
    {
        in>>x;
        if (x == 0) a[i] = Activation::None;
        if (x == 1) a[i] = Activation::Sigmoid;
        if (x == 2) a[i] = Activation::Relu;
        if (x == 3) a[i] = Activation::MaxPooling;
    }
    for (int i=0; i<n; i++)
        in>>b[i].weight;
    w.resize(m);
    for (int i=0; i<m; i++)
        in>>w[i].weight;
}

/**
 * PRIVATE
 */

void KKNN::setN(int n_)
{
    n = n_;
    a.assign(n, Activation::Sigmoid);
    g.clear();
    g.resize(n);
    b.clear();
    b.resize(n);
    for (int i=0; i<n; i++)
        b[i].weight = random_weight(1);
}
