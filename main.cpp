#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <chrono>
#include <fstream>

void bubble_sort(std::vector<int>& v){

    int n = v.size();

    int i = 1;
    bool swapped;
    do{
        swapped = false;
        for(int j = 0; j < n - i; ++j){
            if(v[j] > v[j+1]){
                std::swap(v[j], v[j+1]);
                swapped = true;
            }
        }
        ++i;
    }while(swapped);

}

void comb_sort(std::vector<int>& v){

    int n = v.size();
    int step = n - 1;
    double fact = 1.247330950103979;

    while(step > 1){

        for(int i = 0; i + step < n; ++i){
            if (v[i] > v[i + step]){
                std::swap(v[i], v[i + step]);
            }
        }
        step /= fact;
    }
    bubble_sort(v);
}

std::vector<int> getRandomVector(int n, int MIN, int MAX){

    std::mt19937 psRandom(rand());
    std::uniform_int_distribution<int> distr(MIN, MAX);

    std::vector<int> v(n);

    for(int i = 0; i < n; ++i){
        v[i] = distr(psRandom);
    }
    return v;
}

int compare(const void * x1, const void * x2){
    return *(int*)x1 > *(int*)x2? 1: *(int*)x1 < *(int*)x2? -1: 0;
}

int main(){


    std::srand(time(0));


    int initialSize = 1000, finishSize = 100000, step = 1000;

    std::vector<int> v1(initialSize);

    int a = std::numeric_limits<int>::min();
    int b = std::numeric_limits<int>::max();

    std::string logFileName;

    for(int sort_code = 0; sort_code < 4; ++sort_code){

        switch(sort_code){
            case 0:
                std::cout << "comb sort\n";
                logFileName = "comb.txt";
                break;
            case 1:
                std::cout << "qsort\n";
                logFileName = "qsort.txt";
                break;
            case 2:
                std::cout << "stable_sort\n";
                logFileName = "stable_sort.txt";
                break;
            case 3:
                std::cout << "sort\n";
                logFileName = "sort.txt";
        }

        std::ofstream fout(logFileName);

        v1 = getRandomVector(initialSize, a, b);
        int current_size = v1.size();
        while(current_size <= finishSize){

            int time = 0;
            int N = 7;
            for(int i = 0; i < N; ++i){

                std::vector<int> v2 = getRandomVector(current_size, a, b);

                auto startTime = std::chrono::steady_clock::now();

                switch(sort_code){
                    case 0:
                        comb_sort(v2);
                        break;
                    case 1:
                        qsort(&v2.front(), v2.size(), sizeof(int), compare);
                        break;
                    case 2:
                        std::stable_sort(v2.begin(), v2.end());
                        break;
                    case 3:
                        std::sort(v2.begin(), v2.end());
                }

                auto stopTime = std::chrono::steady_clock::now();

                time += std::chrono::duration_cast<std::chrono::milliseconds>(stopTime - startTime).count();
            }

            fout << time / N << '\n';

            std::vector<int> joiningVector = getRandomVector(step, a, b);
            v1.insert(v1.end(), joiningVector.begin(), joiningVector.end());
            current_size = v1.size();
            random_shuffle(v1.begin(), v1.end());

            std::cout << (100 * v1.size() / finishSize) << "%\r" << std::flush;
        }

        std::cout << "Done. Please check " << logFileName << ".\n";

        fout.close();
    }

    return 0;
}