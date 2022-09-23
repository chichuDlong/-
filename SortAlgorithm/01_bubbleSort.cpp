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

class Bubble {
public:
    Bubble(int n) : data(n), gen(std::random_device()()), rnd(0, 1000) {}
    void dataGeneration() {
        for (int i = 0; i < data.n; i++)
            data[i] = rnd(gen);
    }

    void dataShow() {
        for (int i = 0; i < data.n; i++)
            cout << data(i) << ((i + 1) % 15 ? ' ' : '\n');
    }

    void bubbleSort() {
        for (int i = 0; i < data.n - 1; i++) {
            for (int j = 1; j < data.n - i; j++)
                if(data[j-1] > data[j])
                    dataExchange(data, j, j - 1);
        }
    }

    void bubbleSort1() {
        for(int i = 0; i < data.n - 1; i++) {
            bool exchanged = false;
            for(int j = 0; j < data.n - 1 - i; j++) {
                if(data[j] > data[j+1]) {
                    dataExchange(data, j, j + 1);
                    exchanged = true;
                }
            }
            if(!exchanged)
                return;
        }
    }

    void bubbleSort2() {
        int lastLoc = data.n;
        int tmpPos = 0;
        for(int i = 0; i < data.n - 1; i++) {
            bool exchanged = false;
            for(int j = 0; j < lastLoc - 1; j++) {
                if(data[j] > data[j+1]) {
                    dataExchange(data, j, j + 1);
                    tmpPos = j + 1;
                    exchanged = true;
                }
            }
            lastLoc = tmpPos;
            if(!exchanged)
                return;
        }
    }

    void bubbleSort3() {
        int leftLasChangeIx = 0;
        int rightLastChangIx = data.n - 1;
        for (int i = 0; i < data.n / 2; i++) {
            int tmpIx = -1;
            for (int j = leftLasChangeIx; j < rightLastChangIx; j++) {
                if (data[j] > data[j + 1]) {
                    dataExchange(data, j, j + 1);
                    tmpIx = j + 1;
                }
            }
            if(tmpIx == -1)
                break;
            else
                rightLastChangIx = tmpIx;
            tmpIx = -1;
            for(int k = rightLastChangIx; k >leftLasChangeIx; k--) {
                if(data[k] < data[k-1]){
                    dataExchange(data, k, k - 1);
                    tmpIx = k - 1;
                }
            }
            if (tmpIx == -1)
                break;
            else
                leftLasChangeIx = tmpIx;
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
    Bubble bubble(num);
    bubble.dataGeneration();
    cout << "Before sort:" << endl;
    bubble.dataShow();
    
    auto timeS = chrono::high_resolution_clock::now();
    bubble.bubbleSort();
    auto timeE = chrono::high_resolution_clock::now();
    chrono::duration<double> timeDelta = timeE - timeS;
    cout << endl << "After sort:" << timeDelta.count() <<" s" <<endl;
    bubble.dataShow();
    cout << "===============================" <<endl;

    bubble.dataGeneration();
    cout << endl << "Before sort:" << endl;
    bubble.dataShow();
    timeS = chrono::high_resolution_clock::now();
    bubble.bubbleSort1();
    timeE = chrono::high_resolution_clock::now();
    timeDelta = timeE - timeS;
    cout << endl << "After sort2:" << timeDelta.count() <<" s" <<endl;
    bubble.dataShow();
    cout << "===============================" <<endl;

    bubble.dataGeneration();
    cout << endl << "Before sort:" << endl;
    bubble.dataShow();
    timeS = chrono::high_resolution_clock::now();
    bubble.bubbleSort2();
    timeE = chrono::high_resolution_clock::now();
    timeDelta = timeE - timeS;
    cout << endl << "After sort3:" << timeDelta.count() <<" s" <<endl;
    bubble.dataShow();
    cout << "===============================" <<endl;

    bubble.dataGeneration();
    cout << endl << "Before sort:" << endl;
    bubble.dataShow();
    timeS = chrono::high_resolution_clock::now();
    bubble.bubbleSort3();
    timeE = chrono::high_resolution_clock::now();
    timeDelta = timeE - timeS;
    cout << endl << "After sort4:" << timeDelta.count() <<" s" <<endl;
    bubble.dataShow();
    return 0;
}