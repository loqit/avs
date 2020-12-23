#include <iostream>
#include <omp.h>
#include <chrono>
#include <ctime>
#include <random>



double fRand(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}
void matrixVectorV2(const std::vector<std::vector<double>>& a, const std::vector<double>& v, std::vector<double>& res)
{
    size_t n = a.size(), m = a[0].size();

    for (int i = 0;i < n;i++)
    {

        for (int j = 0;j < m;j++)
        {
            res[i] += a[i][j] * v[j];
        }

    }

/*
    std::string s = flag ? " " : "\n";
    for (int i = 0; i < n; ++i) {
        std::cout << res[i] << s;
    }*/
}

void matrixVector(const std::vector<std::vector<double>>& a, const std::vector<double>& v, std::vector<double>& res)
{
    size_t n = a.size(), m = a[0].size();
    // double tmp = 0;
    int i, j;
#pragma omp parallel for private(i, j) schedule(static)

    for (i = 0;i < n;i++)
    {
        double tmp = 0;
        for (j = 0;j < m;j++)
        {
            tmp += a[i][j] * v[j];
        }

        res[i] = tmp;

    }

/*
    std::string s = flag ? " " : "\n";
    for (int i = 0; i < n; ++i) {
        std::cout << res[i] << s;
    }
    */
}


int main() {
    int n, m;
    std::cout << "Enter matrix size:";
    std::cin >> n >> m;
    std::vector<std::vector<double>> a(n, std::vector<double>(m));
    std::vector<double> v(m), res(n);

    srand(time(NULL));

//#pragma omp parallel for schedule(static)
    for (int i = 0;i < n ;i++)
    {
        for (int j = 0;j < m;j++)
        {
            a[i][j] = fRand(1,100);

        }
    }

    for (int i = 0;i < m;i++)
    {
        v[i] = fRand(1,100);
    }

    auto begin = std::chrono::steady_clock::now();
    matrixVector(a, v, res);
    auto end = std::chrono::steady_clock::now();
    auto time = std::chrono::duration<double, std::milli>(end - begin).count();
    std::cout << '\n' << "Time : " << time << std::endl;
    auto x = v;
    std::cout << '\n' << "Without OpenMP"  << '\n' << std::endl;

    begin = std::chrono::steady_clock::now();
    matrixVectorV2(a, v, res);
    end = std::chrono::steady_clock::now();
    time = std::chrono::duration<double, std::milli>(end - begin).count();
    std::cout << '\n' << "Time : " << time << std::endl;
    auto y = v;
    if(x == y)
    {
        std::cout << "true" << std::endl;
    }
    return 0;
}
