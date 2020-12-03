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

void matrixVector(int n, int m, const double* arr, bool flag)
{
    //std::cout << "Enter vector" << '\n';
    auto * v = new double [m];
    //#pragma omp for
    for (int i = 0;i < m;i++)
    {
        v[i] = fRand(1,100);
    }


    double res[n];
#pragma omp parallel // shared(n, m, arr, v) reduction(+:res)
    {
        /*
        int nthreads = omp_get_num_threads();
    int threadid = omp_get_thread_num();
    int items_per_thread = m / nthreads;
    int lb = threadid * items_per_thread;
    int ub = (threadid == nthreads - 1) ? (m - 1) : (lb + items_per_thread - 1);
*/
#pragma omp for
        for (int i = 0;i < n;i++)
        {
            res[i] = 0;
            for (int j = 0;j < m;j++)
            {
                int x = i, y = j;
                if (flag) { std::swap(x, y); }
#pragma omp atomic
                res[i] += arr[x * m + y] * v[j];
            }
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



#pragma omp for
    for (int i = 0;i < n ;i++)
    {
        for (int j = 0;j < m;j++)
        {
            a[i][j] = fRand(1,100);
            // std::cout << a[i][j] << " " ;
        }
        //std::cout << '\n';
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
    std::cout << '\n' << "Time : " << time << std::endl;
    return 0;
}
