#include <cstdlib>
#include <iostream>
#include <random>
#include <chrono>
using namespace std;;

template<typename T>
class Field_ {
public:
    Field_(int n) : n(n) {
        data = new T[n];
        clear();
    }
    ~Field_() {
        if (data == nullptr) return;
        delete[] data;
        data = nullptr;
    }
    Field_& operator=(double s) {
        for (int i = 0; i < n; i++)
            data[i] = s;
        return *this;
    }
    void clear() {
        (*this) = 0;
    }
    T operator()(int i) const {
        return data[i];
    }
    T& operator[](int i) {
        return data[i];
    }
    int n;
    T* data;
};
using FieldI = Field_<int>;
using FieldD = Field_<double>;

class Selection {
public:
    Selection(int n) : data(n), gen(std::random_device()()), rnd(0, 1000) {}
    void dataGeneration() {
        for (int i = 0; i < data.n; i++)
            data[i] = rnd(gen);
    }
    void dataShow() {
        for (int i = 0; i < data.n; i++)
            cout << data(i) << ((i + 1) % 15 ? ' ' : '\n');
    }
    void selectionSort() {
        for (int i = 0; i < data.n - 1; i++) {
            int minIx = i;
            for (int j = i + 1; j < data.n; j++)
                minIx = data[minIx] > data[j] ? j : minIx;
            dataExchange(data, i, minIx);
        }
    }
    void selectionSort2() {
        for(int left = 0, right = data.n - 1; left < right; left++, right--) {
            int minIx = left;
            int maxIx = right;
            for(int i = left; i <= right; i++) {
                minIx = data[minIx] > data[i] ? i : minIx;
                maxIx = data[maxIx] < data[i] ? i : maxIx;
            }
            dataExchange(data, left, minIx);
            if(left == maxIx)
                maxIx = minIx;
            dataExchange(data, right, maxIx);
        }
    }
    void dataExchange(FieldI& data, int& ix, int& jx) {
        int tmp = data[ix];
        data[ix] = data[jx];
        data[jx] = tmp;
    }
    FieldI data;
protected:
    std::mt19937 gen;
    std::uniform_int_distribution<int> rnd;
};

int main(int argc, char* argv[]) {
    int num = 20;
    if(argc > 1)
        num = atoi(argv[1]);
    Selection s(num);
    s.dataGeneration();

    cout << "Before sort:" << endl;
    s.dataShow();
    
    auto timeS = chrono::high_resolution_clock::now();
    s.selectionSort();
    auto timeE = chrono::high_resolution_clock::now();
    chrono::duration<double> timeDelta = timeE - timeS;
    cout << endl << endl << "After sort:" << endl;
    cout << "Selection sort time consumption: " << timeDelta.count() <<" s" <<endl;
    s.dataShow();
    
    s.dataGeneration();
    timeS = chrono::high_resolution_clock::now();
    s.selectionSort2();
    timeE = chrono::high_resolution_clock::now();
    timeDelta = timeE - timeS;
    cout << endl << endl << "After sort2:" << endl;
    cout << "Selection sort2 time consumption: " << timeDelta.count() <<" s" <<endl;
    s.dataShow();
    return 0;
}