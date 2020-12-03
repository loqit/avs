#include <iostream>
#include <omp.h>
#include <chrono>

void matrixVector(int n, int m, const double* arr, bool flag)
{
    std::cout << "Enter vector" << '\n';
    auto * v = new double [m];
    for (int i = 0;i < m;i++)
    {
        std::cin >> v[i];
    }
    double res[n];
#pragma omp parallel shared(n, m, arr, v, flag) reduction(+:tmp) private(i, j)
    {
#pragma omp for sheldue(static)
        for (int i = 0;i < n;i++)
        {
            double tmp = 0;
            for (int j = 0;j < m;j++)
            {
                int x = i, y = j;
                if (flag) { std::swap(x, y); }
                tmp += arr[x * m + y] * v[j];
            }
            res[i] = tmp;
        }
    }

    std::string s = flag ? " " : "\n";
    for (int i = 0; i < n; ++i) {
        std::cout << res[i] << s;
    }
}


int main() {
    int n, m;
    std::cout << "Enter matrix size:";
    std::cin >> n >> m;
    double a[n][m];
    std::cout << "Enter matrix" << '\n';
    for (int i = 0;i < n;i++)
    {
        for (int j = 0;j < m;j++)
        {
            std::cin >> a[i][j];
        }
    }

    auto begin = std::chrono::high_resolution_clock::now();
    matrixVector(n, m, &a[0][0], false);
    auto end = std::chrono::high_resolution_clock::now();
    auto time = std::chrono::duration<double, std::milli>(end - begin).count();
    std::cout << "Time : " << time << std::endl;
    std::cout << '\n';

    begin = std::chrono::high_resolution_clock::now();
    matrixVector(n, m, &a[0][0], true);
    end = std::chrono::high_resolution_clock::now();
    time = std::chrono::duration<double, std::milli>(end - begin).count();
    std::cout << "Time : " << time << std::endl;
    return 0;
}
