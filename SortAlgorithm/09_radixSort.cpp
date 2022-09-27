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

class Radix {
public:
    Radix(int n) : data(n), gen(std::random_device()()), rnd(0, 10000) {}
    void dataGeneration() {
        for (int i = 0; i < data.n; i++)
            data[i] = rnd(gen);
    }

    void dataShow() {
        for (int i = 0; i < data.n; i++)
            cout << data(i) << ((i + 1) % 15 ? ' ' : '\n');
    }

    void radixSort() {
        int maxNum = maxBitAndNum(data)[0];
        int cntBit = maxBitAndNum(data)[1];        
        cout << endl << "cntBit = " << cntBit << endl;
        cout << "maxNum = " << maxNum << endl;        
        FieldI cnt(10);
        FieldI res(data.n);
        // 进行 cntBit 次排序
        for(int i = 1, radix = 1; i <= cntBit; i++) {
            cnt.clear();            
            //统计每个桶中的记录数
            for(int j = 0; j < data.n; j++) {
                int k = data[j] / radix % 10; 
                cnt[k]++;
            }
            // 将 res 中的位置依次分配给每个桶
            for(int j = 1; j < cnt.n; j++)
                cnt[j] += cnt[j - 1];            
            // 将所有桶中记录依次收集到 res 中
            for(int j = data.n - 1; j >= 0; j--) {
                int k = data[j] / radix % 10;
                res[--cnt[k]] = data[j];
                // res[cnt[k] - 1] = data[j];
                // cnt[k]--;
            }
            //将临时数组的内容复制到data中
            for(int j = 0; j < data.n; j++)
                data[j] = res[j];
            radix *= 10;
        }
    }

    void radixSort1() {
        int maxNum = maxBitAndNum(data)[0];
        int cntBit = maxBitAndNum(data)[1];
        int minNum = maxBitAndNum(data)[2];
        if(minNum < 0) 
            for(int i = 0; i < data.n; i++) data[i] -= minNum;
        cout << endl << "cntBit = " << cntBit << endl;
        cout << "maxNum = " << maxNum << endl;        
        FieldI cnt(10);
        FieldI res(data.n);
        // 进行 cntBit 次排序
        for(int i = 1, radix = 1; i <= cntBit; i++) {
            cnt.clear();            
            //统计每个桶中的记录数
            for(int j = 0; j < data.n; j++) {
                int k = data[j] / radix % 10; 
                cnt[k]++;
            }
            // 将 res 中的位置依次分配给每个桶
            for(int j = 1; j < cnt.n; j++)
                cnt[j] += cnt[j - 1];            
            // 将所有桶中记录依次收集到 res 中
            for(int j = data.n - 1; j >= 0; j--) {
                int k = data[j] / radix % 10;
                res[--cnt[k]] = data[j];
                // res[cnt[k] - 1] = data[j];
                // cnt[k]--;
            }
            //将临时数组的内容复制到data中
            for(int j = 0; j < data.n; j++)
                data[j] = res[j];
            radix *= 10;
        }
        // 排序结束，将减去的最小值加回
        if(minNum < 0) 
            for(int i = 0; i < data.n; i++) data[i] += minNum;
    }

    vector<int> maxBitAndNum(FieldI& data) {
        if(data.n == 0) return {};
        int resMax = data[0], resMin = data[0], cnt = 0;
        for(int i = 1; i < data.n; i++) {
            // if(resMax < data[i]) resMax = data[i];
            resMax = resMax < data[i] ? data[i] : resMax;
            resMin = resMin > data[i] ? data[i] : resMin;
        }            
        int maxNum = resMin < 0 ? resMax - resMin : resMax;
        while (resMax) {
            resMax /= 10;
            cnt++;
        }
        return {maxNum, cnt, resMin};
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
    Radix radix(num);
    radix.dataGeneration();
    cout << "Before sort:" << endl;
    radix.dataShow();
    
    auto timeS = chrono::high_resolution_clock::now();
    radix.radixSort();
    auto timeE = chrono::high_resolution_clock::now();
    chrono::duration<double> timeDelta = timeE - timeS;
    cout << endl << "After sort:" << timeDelta.count() <<" s" <<endl;
    radix.dataShow();
    cout << endl;
    
    cout << "===============================" <<endl;
    radix.dataGeneration();
    cout << endl << "Before sort:" << endl;
    radix.dataShow();
    timeS = chrono::high_resolution_clock::now();
    radix.radixSort1();
    timeE = chrono::high_resolution_clock::now();
    timeDelta = timeE - timeS;
    cout << endl << "After sort1:" << timeDelta.count() <<" s" <<endl;
    radix.dataShow();
    return 0;
}