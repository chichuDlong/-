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

class Counting {
public:
    Counting(int n) : data(n), gen(std::random_device()()), rnd(0, 10000) {}
    void dataGeneration() {
        for (int i = 0; i < data.n; i++)
            data[i] = rnd(gen);
    }

    void dataShow() {
        for (int i = 0; i < data.n; i++)
            cout << data(i) << ((i + 1) % 15 ? ' ' : '\n');
    }

    void countingSort() {
        int maxNum = getMax(data);
        FieldI cnt(maxNum + 1);
        for(int i = 0; i < data.n; i++) cnt[data[i]]++;
        for(int i = 0, j = 0; i < cnt.n; i++) 
            while(cnt[i]-- > 0) data[j++] = i;
    }

    void countingSort1() {
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

    FieldI data;
protected:
    std::mt19937 gen;
    std::uniform_int_distribution<int> rnd;
};

int main(int argc, char* argv[]) {
    int num = 20;
    if(argc > 1)
        num = atoi(argv[1]);
    Counting counting(num);
    counting.dataGeneration();
    cout << "Before sort:" << endl;
    counting.dataShow();
    
    auto timeS = chrono::high_resolution_clock::now();
    counting.countingSort();
    auto timeE = chrono::high_resolution_clock::now();
    chrono::duration<double> timeDelta = timeE - timeS;
    cout << endl << "After sort:" << timeDelta.count() <<" s" <<endl;
    counting.dataShow();
    cout << endl;
    
    cout << "===============================" <<endl;
    counting.dataGeneration();
    cout << endl << "Before sort:" << endl;
    counting.dataShow();
    timeS = chrono::high_resolution_clock::now();
    counting.countingSort1();
    timeE = chrono::high_resolution_clock::now();
    timeDelta = timeE - timeS;
    cout << endl << "After sort1:" << timeDelta.count() <<" s" <<endl;
    counting.dataShow();
    return 0;
}