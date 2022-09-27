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

class Merge {
public:
    Merge(int n) : data(n), gen(std::random_device()()), rnd(0, 1000) {}
    void dataGeneration() {
        for (int i = 0; i < data.n; i++)
            data[i] = rnd(gen);
    }

    void dataShow() {
        for (int i = 0; i < data.n; i++)
            cout << data(i) << ((i + 1) % 15 ? ' ' : '\n');
    }

    void mergeSort(FieldI& data, int front, int end) {
        if(front >= end)
            return;
        int mid = front + (end - front) / 2;
        mergeSort(data, front, mid);
        mergeSort(data, mid + 1, end);
        merge(data, front, mid, end);
    }

    void mergeSort1() {
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

    void merge(FieldI& data, int front, int mid, int end) {
        FieldI tmp(end - front + 1);
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
    Merge merge(num);
    merge.dataGeneration();
    cout << "Before sort:" << endl;
    merge.dataShow();
    
    auto timeS = chrono::high_resolution_clock::now();
    merge.mergeSort(merge.data, 0, merge.data.n - 1);
    auto timeE = chrono::high_resolution_clock::now();
    chrono::duration<double> timeDelta = timeE - timeS;
    cout << endl << "After sort:" << timeDelta.count() <<" s" <<endl;
    merge.dataShow();
    cout << endl;

    cout << "===============================" <<endl;
    merge.dataGeneration();
    cout << endl << "Before sort:" << endl;
    merge.dataShow();
    timeS = chrono::high_resolution_clock::now();
    merge.mergeSort1();
    timeE = chrono::high_resolution_clock::now();
    timeDelta = timeE - timeS;
    cout << endl << "After sort1:" << timeDelta.count() <<" s" <<endl;
    merge.dataShow();
    return 0;
}