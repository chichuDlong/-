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

class Shell {
public:
    Shell(int n) : data(n), gen(std::random_device()()), rnd(0, 1000) {}
    void dataGeneration() {
        for (int i = 0; i < data.n; i++)
            data[i] = rnd(gen);
    }

    void dataShow() {
        for (int i = 0; i < data.n; i++)
            cout << data(i) << ((i + 1) % 15 ? ' ' : '\n');
    }

    void shellSort(){
        int h = 1;
        while(h <= data.n / 3)
            h = 3 * h + 1;
        for(int gap = h; gap >= 1; gap /= 3) {
            for(int i = gap; i < data.n; i++){
                int tmp = data[i];
                int j = i - gap;
                while(j >= 0 && tmp < data[j]) {
                    data[j+gap] = data[j];
                    j -= gap;
                }
                data[j+gap] = tmp;
            }
        }
    }

    void shellSort1() {
        int h = 1;
        while(h <= data.n / 3)
            h = 3 * h + 1;
        for(int gap = h; gap > 0; gap = (gap - 1) / 3) {
            for(int i = gap; i < data.n; i++)
                for(int j = i; j > gap - 1; j -= gap)
                    if(data[j] < data[j-gap])
                        dataExchange(data, j, j - gap);
        }
    }

    void dataExchange(FieldI& data, int ix, int jx) {
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
    Shell shell(num);
    shell.dataGeneration();
    cout << "Before sort:" << endl;
    shell.dataShow();
    
    auto timeS = chrono::high_resolution_clock::now();
    shell.shellSort();
    auto timeE = chrono::high_resolution_clock::now();
    chrono::duration<double> timeDelta = timeE - timeS;
    cout << endl << "After sort:" << timeDelta.count() <<" s" <<endl;
    shell.dataShow();
    cout << endl;

    cout << "===============================" <<endl;
    shell.dataGeneration();
    cout << endl << "Before sort:" << endl;
    shell.dataShow();
    timeS = chrono::high_resolution_clock::now();
    shell.shellSort1();
    timeE = chrono::high_resolution_clock::now();
    timeDelta = timeE - timeS;
    cout << endl << "After sort2:" << timeDelta.count() <<" s" <<endl;
    shell.dataShow();
    return 0;
}