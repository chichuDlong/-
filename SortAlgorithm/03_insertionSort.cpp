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

class Insert {
public:
    Insert(int n) : data(n), gen(std::random_device()()), rnd(0, 1000) {}
    void dataGeneration() {
        for (int i = 0; i < data.n; i++)
            data[i] = rnd(gen);
    }

    void dataShow() {
        for (int i = 0; i < data.n; i++)
            cout << data(i) << ((i + 1) % 15 ? ' ' : '\n');
    }

    void insertionSort() {
        for (int i = 1; i < data.n; i++) {
            int tmp = data[i];
            int j = i - 1;
            while(j >= 0 && tmp < data[j]) {
                data[j+1] = data[j];
                j--;
            }
            data[j+1] = tmp;   
        }
    }

    void insertionSort1() {
        for(int i = 1; i < data.n; i++)
            for(int j = i; j > 0 && data[j] < data[j-1]; j--)
                dataExchange(data, j, j - 1);
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
    Insert insert(num);
    insert.dataGeneration();
    cout << "Before sort:" << endl;
    insert.dataShow();
    
    auto timeS = chrono::high_resolution_clock::now();
    insert.insertionSort();
    auto timeE = chrono::high_resolution_clock::now();
    chrono::duration<double> timeDelta = timeE - timeS;
    cout << endl << "After sort:" << timeDelta.count() <<" s" <<endl;
    insert.dataShow();
    cout << "===============================" <<endl;

    insert.dataGeneration();
    cout << endl << "Before sort:" << endl;
    insert.dataShow();
    timeS = chrono::high_resolution_clock::now();
    insert.insertionSort1();
    timeE = chrono::high_resolution_clock::now();
    timeDelta = timeE - timeS;
    cout << endl << "After sort2:" << timeDelta.count() <<" s" <<endl;
    insert.dataShow();

    return 0;
}