#include <cstdlib>
#include <iostream>
#include <random>
#include <chrono>
#include <sys/select.h>
#include <algorithm>
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
    Field_(const Field_& other):Field_{other.n} {
        for(int i = 0; i < n; i++)
            data[i] = other(i);
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

template<typename T>
class sortAlgorithm {
public:
    void selectionSort(Field_<T>& data) {
        if(data.n < 2) return;
        for (int i = 0; i < data.n - 1; i++) {
            int minIx = i;
            for (int j = i + 1; j < data.n; j++)
                minIx = data[minIx] > data[j] ? j : minIx;
            swap(data, i, minIx);
        }
    }
    void selectionSort2(Field_<T>& data) {
        if(data.n < 2) return;
        for(int left = 0, right = data.n - 1; left < right; left++, right--) {
            int minIx = left;
            int maxIx = right;
            for(int i = left; i <= right; i++) {
                minIx = data[minIx] > data[i] ? i : minIx;
                maxIx = data[maxIx] < data[i] ? i : maxIx;
            }
            swap(data, left, minIx);
            if(left == maxIx)
                maxIx = minIx;
            swap(data, right, maxIx);
        }
    }
    void swap(Field_<T>& data, int& ix, int& jx) {
        T tmp = data[ix];
        data[ix] = data[jx];
        data[jx] = tmp;
    }
};

namespace Functions {
    template<typename T>
    Field_<T>& dataGeneration(Field_<T>& data) {
        std::mt19937 gen(std::random_device{} ());
        std::uniform_int_distribution<T> rnd(-1000, 1000);
        for (int i = 0; i < data.n; i++)
                data[i] = rnd(gen);
        return data;
    }

    template<typename T>
    void dataShow(Field_<T>& data) {
        for (int i = 0; i < data.n; i++)
            cout << data(i) << ((i + 1) % 15 ? ' ' : '\n');
    }

    template<typename T>
    bool isEqual(Field_<T>& arr1, Field_<T>& arr2) {
        bool equal = true;
        for(int i = 0; i < arr1.n; i++)
            if(arr1[i] != arr2[i]) {
                equal = false;
                break;
            }
        return equal;
    }

    template<typename T>
    void testCorrect(sortAlgorithm<T>& solver, int num) {
        Field_<T> data(num);

        int testTime = 30000;
        bool succed = true;
        for(int it = 0; it < testTime; it++) {
            FieldI arr1 = dataGeneration(data);
            FieldI arr2 = arr1, arr3 = arr1;
            // before sort
            // cout << "Before Sort" << endl;
            // cout << "【arr1】" << endl;
            // dataShow(arr1);
            // cout << endl << "【arr2】" << endl;
            // dataShow(arr2);
            // cout << endl << "【arr3】" << endl;
            // dataShow(arr3);
            
            // sort
            solver.selectionSort(arr1);
            sort(arr2.data, arr2.data + arr2.n);
            if(!isEqual(arr1, arr2)) {
                succed = false;
                break;
            }
            
            // after sort
            // cout << endl << endl << "After Sort" << endl;
            // cout << "【arr1】" << endl;
            // dataShow(arr1);
            // cout << endl << "【arr2】" << endl;
            // dataShow(arr2);
            // cout << endl << "【arr3】" << endl;
            // dataShow(arr3);
        }
        cout << endl << (succed ? "Sort algorithm correct!!" : "So sorry, you failed!!") << endl;
    }
};


//================================================================================//
int main(int argc, char* argv[]) {
    int num = 20;
    if(argc > 1)
        num = atoi(argv[1]);
    
    sortAlgorithm<int> solver;
    Functions::testCorrect<int>(solver, num);

    FieldI data(num);
    data = Functions::dataGeneration<int>(data);
    cout << "Before sort:" << endl;
    Functions::dataShow(data);
    
    auto timeS = chrono::high_resolution_clock::now();
    solver.selectionSort(data);
    auto timeE = chrono::high_resolution_clock::now();
    chrono::duration<double> timeDelta = timeE - timeS;
    cout << endl << endl << "After sort:" << endl;
    cout << "Selection sort time consumption: " << timeDelta.count() <<" s" <<endl;
    Functions::dataShow(data);
    
    cout << "===============================" <<endl;
    data = Functions::dataGeneration(data);
    timeS = chrono::high_resolution_clock::now();
    solver.selectionSort2(data);
    timeE = chrono::high_resolution_clock::now();
    timeDelta = timeE - timeS;
    cout << endl << endl << "After sort2:" << endl;
    cout << "Selection sort2 time consumption: " << timeDelta.count() <<" s" <<endl;
    Functions::dataShow(data);
    return 0;
}