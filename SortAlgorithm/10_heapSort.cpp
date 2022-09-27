#include <cstdlib>
#include <iostream>
#include <random>
#include <chrono>
#include <vector>
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

class Heap {
public:
    Heap(int n) : data(n), gen(std::random_device()()), rnd(-100, 10000) {}
    void dataGeneration() {
        for (int i = 0; i < data.n; i++)
            data[i] = rnd(gen);
    }

    void dataShow() {
        for (int i = 0; i < data.n; i++)
            cout << data(i) << ((i + 1) % 15 ? ' ' : '\n');
    }

    void heapSort() {
        if(data.n <= 1) return;
        // 初始化，i从最后一个父节点开始调整
        for (int i = data.n / 2 - 1; i >= 0; i--)
            maxHeapify(data, i, data.n - 1);
        // 先将第一个元素和已经排好的元素的前一位交换，再重新调整(刚调整的元素之前的元素)，直到排序完毕
        for (int i = data.n - 1; i > 0; i--) {
            swap(data, 0, i);
            maxHeapify(data, 0, i - 1);
        }
    }

    void maxHeapify(FieldI& data, int start, int end) {
        // 建立父节点索引和子节点索引
        int dad = start;
        int son = dad * 2 + 1;
        // 若子节点索引在范围内才做比较
        while (son <= end) {
            // 比较连个子节点大小，选择大的
            if (son + 1 <= end && data[son] < data[son + 1]) son++;
            // 如果父节点大于子节点说明调整完毕，直接跳出
            if (data[dad] > data[son]) return;
            // 否则交换父子内容，再继续比较父子节点
            else { 
                swap(data, dad, son);
                dad = son;
                son = dad * 2 + 1;
            }
        }
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
    Heap heap(num);
    heap.dataGeneration();
    cout << "Before sort:" << endl;
    heap.dataShow();
    
    auto timeS = chrono::high_resolution_clock::now();
    heap.heapSort();
    auto timeE = chrono::high_resolution_clock::now();
    chrono::duration<double> timeDelta = timeE - timeS;
    cout << endl << "After sort:" << timeDelta.count() <<" s" <<endl;
    heap.dataShow();
    cout << endl;
    
    return 0;
}