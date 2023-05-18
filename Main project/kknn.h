#include <vector>
#include <string>

enum Activation
{
    None,
    Sigmoid,
    Relu,
    MaxPooling
};

double sigmoid(double x);
double sigmoid_derivative(double y);
double relu(double x);
double relu_derivative(double x);

class KKNN
{
public:
    KKNN() = default;
    KKNN(int total_n);
    KKNN(std::vector<int> layers);

public:
    void AddEdge(int u, int v);
    void AddEdge(int u, int v, int wid);
    void AddDenseLayerTransition(int l1, int r1, int l2, int r2);
    void AddDenseLayerTransition(int l, int m, int r);
    std::vector<std::vector<int> > AddEmbeddingMatrix(int w, int h);

    int NewWeight(double r);
    void SetActivation(int i, Activation act);
    void SetEducationCoeff(double coeff);
public:
    std::vector<double> Run(std::vector<double> input);
    double SingleTestcase(std::vector<double> input, std::vector<double> target_output, double case_w);
    void InitializeTestset();
    void FinishTestset(int cnt);
    double RunTestset(std::vector<std::vector<double> > input, std::vector<std::vector<double> > target_output, std::vector<double> case_w = {});
    void Save(std::string file_name);
    void Load(std::string file_name);
private:
    void setN(int n_);

    struct BackwardEdge
    {
        int from;
        int wid;
    };
    struct Weight
    {
        double weight;
        double cumulative;
    };
    int n;
    std::vector<std::vector<BackwardEdge> > g;
    std::vector<Activation> a;
    std::vector<Weight> w;
    std::vector<Weight> b;
    double educationCoeff = 0.01;
};

