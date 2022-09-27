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

class Quick {
public:
    Quick(int n) : data(n), gen(std::random_device()()), rnd(0, 1000) {}
    void dataGeneration() {
        for (int i = 0; i < data.n; i++)
            data[i] = rnd(gen);
    }

    void dataShow() {
        for (int i = 0; i < data.n; i++)
            cout << data(i) << ((i + 1) % 15 ? ' ' : '\n');
    }

    void quickSort(FieldI& data, int low, int high) {
        if(low >= high) return;
        int pivot = partition(data, low, high);
        quickSort(data, low, pivot - 1);
        quickSort(data, pivot + 1, high);
    }

    void dualQuickSort(FieldI& data, int start, int end) {
        if(start > end) return; // 参数不对直接返回
        if(data[start] > data[end]) swap(data, start, end);
        int left = start, right = end, k = start + 1;
        // (start，left]:左侧小于等于pivot1 [right,end)大于pivot2  
        int pivot1 = data[start], pivot2 = data[end]; // 储存最左侧和最右侧的值
        while (k < right) {
            if (data[k] <= pivot1) {
                // 避免left就在k前面时进行元素的自我比较交换
                if (left + 1 == k) left++;
                else swap(data, ++left, k);
                k++;
            }
            // 由于放入了一个新的元素进来，此时的k位置要继续与左侧比较
            else if (data[k] >= pivot2) swap(data, --right, k);
            else k++;
        }
        swap(data, start, left);
        swap(data, end, right);
        dualQuickSort(data, start, left-1);
        dualQuickSort(data, left+1, right-1);
        dualQuickSort(data, right+1, end);
    }

    int partition(FieldI& data, int low, int high) {
        int pivot = data[low];
        while(low < high) {
            while (low < high && pivot <= data[high]) high--;
            data[low] = data[high];
            while (low < high && pivot >= data[low]) low++;
            data[high] = data[low];
        }
        data[low] = pivot;
        return low;
    }

    void swap(FieldI& data, int i, int j) {
        int tmp = data[i];
        data[i] = data[j];
        data[j] = tmp;
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
    Quick quick(num);
    quick.dataGeneration();
    cout << "Before sort:" << endl;
    quick.dataShow();
    
    auto timeS = chrono::high_resolution_clock::now();
    quick.quickSort(quick.data, 0, quick.data.n - 1);
    auto timeE = chrono::high_resolution_clock::now();
    chrono::duration<double> timeDelta = timeE - timeS;
    cout << endl << "After sort:" << timeDelta.count() <<" s" <<endl;
    quick.dataShow();
    cout << endl;

    cout << "===============================" <<endl;
    quick.dataGeneration();
    cout << endl << "Before sort:" << endl;
    quick.dataShow();
    timeS = chrono::high_resolution_clock::now();
    quick.dualQuickSort(quick.data, 0, quick.data.n - 1);
    timeE = chrono::high_resolution_clock::now();
    timeDelta = timeE - timeS;
    cout << endl << "After sort1:" << timeDelta.count() <<" s" <<endl;
    quick.dataShow();
    return 0;
}