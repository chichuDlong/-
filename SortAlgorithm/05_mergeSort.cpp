#include <cstdlib>
#include <iostream>
#include <random>
#include <chrono>
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
    void mergeSort(Field_<T>& data, int front, int end) {
        if(front >= end)
            return;
        int mid = front + (end - front) / 2;
        mergeSort(data, front, mid);
        mergeSort(data, mid + 1, end);
        merge(data, front, mid, end);
    }

    void mergeSort1(Field_<T>& data) {
        if(data.n < 2) return;
        // int size = 1, low = 0, mid = 0, high = 0;
        // while(size < data.n) {
        //     low = 0;
        //     while (low + size < data.n) {
        //         mid = low + size - 1;
        //         high = mid + size;
        //         if(high > data.n - 1)
        //             high = data.n - 1;
        //         merge(data, low, mid, high);
        //         low = high + 1;
        //     }
        //     size *= 2;
        // }
        for(int size = 1; size < data.n; size *= 2) {
            for(int low = 0, mid = 0, high = 0; low + size < data.n; low = high + 1) {
                mid = low + size - 1;
                high = mid + size;
                // if(high > data.n - 1) high = data.n - 1;
                high = high > data.n - 1 ? data.n - 1 : high;
                merge(data, low, mid, high);
            }
        }
    }

    void merge(Field_<T>& data, int front, int mid, int end) {
        Field_<T> tmp(end - front + 1);
        int i = front, j = mid + 1, k = 0;
        while(i <= mid && j <= end)
            tmp[k++] = data[i] <= data[j] ? data[i++] : data[j++];
        while(i <= mid)
            tmp[k++] = data[i++];
        while(j <= end)
            tmp[k++] = data[j++];
        for(int ix = 0; ix < tmp.n; ix++)
            data[front+ix] = tmp[ix];
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
            solver.mergeSort(arr1, 0, arr1.n - 1);
            // solver.mergeSort1(arr1);
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

/*====================================================================*/
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
    solver.mergeSort(data, 0, data.n - 1);
    auto timeE = chrono::high_resolution_clock::now();
    chrono::duration<double> timeDelta = timeE - timeS;
    cout << endl << "After sort:" << timeDelta.count() <<" s" <<endl;
    Functions::dataShow(data);
    cout << endl;

    cout << "===============================" <<endl;
    data = Functions::dataGeneration<int>(data);
    cout << endl << "Before sort:" << endl;
    Functions::dataShow(data);
    timeS = chrono::high_resolution_clock::now();
    solver.mergeSort1(data);
    timeE = chrono::high_resolution_clock::now();
    timeDelta = timeE - timeS;
    cout << endl << "After sort1:" << timeDelta.count() <<" s" <<endl;
    Functions::dataShow(data);
    cout << endl;
    return 0;
}