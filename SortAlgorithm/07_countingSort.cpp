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
    void countingSort(FieldI& data) {
        if(data.n < 2) return;
        int maxNum = getMax(data);
        FieldI cnt(maxNum + 1);
        for(int i = 0; i < data.n; i++) cnt[data[i]]++;
        for(int i = 0, j = 0; i < cnt.n; i++) 
            while(cnt[i]-- > 0) data[j++] = i;
    }

    void countingSort1(FieldI& data) {
        if(data.n < 2) return;
        // 第 1 步，找到序列中的最大值
        int maxNum = getMax(data);

        // 第 2 步，创建一个数组，长度至少为 max+1，并初始化为 0
        FieldI cnt(maxNum + 1);
        FieldI res(data.n);

        // 第 3 步，遍历源数组，统计各个元素的出现次数，并存储在相应的位置上
        for(int i = 0; i < data.n; i++) cnt[data[i]]++;
        
        // cout <<endl<< "计数组数组 cnt" << endl;
        // for (int i = 0; i < cnt.n; i++)
        //     cout << cnt(i) << ((i + 1) % 15 ? ' ' : '\n');

        //第 4 步，累加 cnt 数组中的出现次数
        for(int i = 1; i < cnt.n; i++) cnt[i] += cnt[i-1];
        
        // cout <<endl<< "求和之后的累计数组" << endl;
        // for (int i = 0; i < cnt.n; i++)
        //     cout << cnt(i) << ((i + 1) % 15 ? ' ' : '\n');

        // 第 5 步，根据 cnt 数组中的信息，找到各个元素排序后所在位置，存储在 res 数组中
        // 第 6 步，数组相应位置上的值减 1
        for (int i = data.n - 1; i >= 0; i--) {
            res[--cnt[data[i]]] = data[i];
            // res[cnt[data[i]]-1] = data[i];
            // cnt[data[i]]--;
        }
        
        // 将 output 数组中的数据原封不动地拷贝到 list 数组中
        for (int i = 0; i < data.n; i++) data[i] = res[i];
    }

    int getMax(FieldI& data) {
        if(data.n == 0) return 0;
        int res = data[0];
        for(int i = 1; i < data.n; i++)
            if(res < data[i]) res = data[i];
        return res;
    }
};

namespace Functions {
    template<typename T>
    Field_<T>& dataGeneration(Field_<T>& data) {
        std::mt19937 gen(std::random_device{} ());
        std::uniform_int_distribution<T> rnd(0, 1000);
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

        int testTime = 300000;
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
            solver.countingSort1(arr1);
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


/*============================================================*/
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
    solver.countingSort(data);
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
    solver.countingSort1(data);
    timeE = chrono::high_resolution_clock::now();
    timeDelta = timeE - timeS;
    cout << endl << "After sort1:" << timeDelta.count() <<" s" <<endl;
    Functions::dataShow(data);
    cout << endl;
    return 0;
}