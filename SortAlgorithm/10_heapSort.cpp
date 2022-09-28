#include <cstdlib>
#include <iostream>
#include <random>
#include <chrono>
#include <vector>
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
    void heapSort(Field_<T>& data) {
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

    void maxHeapify(Field_<T>& data, int start, int end) {
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

    void heapSort1(Field_<T>& data) {
        buildHeap(data); 
        for (int i = data.n - 1; i >= 0; i--) {
            //堆顶与末尾结点值交换
            swap(data, i, 0);
            //i不断在砍断
            heapify(data, 0, i);
        }
    }

    void buildHeap(Field_<T>& data) {
        int lastNode = data.n - 1;
        //最后一个结点的父节点 下标，即最后一个非叶子结点
        int parent = (lastNode - 1) / 2;       
        //针对最后一个父节点的 及其前面的父节点进行建堆
        for (int i = parent; i >= 0; i--) 
            heapify(data, i,  data.n);
    }

    void heapify(Field_<T>& data, int i, int n) {
        if (i >= n) return;
        //假设最大值坐标是根结点，获取左右子结点的最大值
        int max = i;
        //左子结点
        int cl = i * 2 + 1;
        //右子结点
        int cr = i * 2 + 2;        
        if (cl < n && data[cl] > data[max]) max = cl;
        if (cr < n && data[cr] > data[max]) max = cr;
        if (max != i) {
            //将左右子树的最大值赋给父结点
            swap(data, max, i);
            //较小的值，被赋给左子树或右子树，则左子树或右子树 需要重新建堆 
            heapify(data, max, n);
        }
    }

    void swap(Field_<T>& data, int ix, int jx) {
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
            solver.heapSort(arr1);
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

/*=======================================================*/
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
    solver.heapSort(data);
    auto timeE = chrono::high_resolution_clock::now();
    chrono::duration<double> timeDelta = timeE - timeS;
    cout << endl << "After sort: " << timeDelta.count() <<" s" <<endl;
    Functions::dataShow(data);
    cout << endl;
    
    cout << "===============================" <<endl;
    data = Functions::dataGeneration<int>(data);
    cout << endl << "Before sort:" << endl;
    Functions::dataShow(data);
    timeS = chrono::high_resolution_clock::now();
    solver.heapSort1(data);
    timeE = chrono::high_resolution_clock::now();
    timeDelta = timeE - timeS;
    cout << endl << "After sort1: " << timeDelta.count() <<" s" <<endl;
    Functions::dataShow(data);
    cout << endl;
    return 0;
}