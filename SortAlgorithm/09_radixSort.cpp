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
    void radixSort(FieldI& data) {
        if(data.n < 2) return;
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

    void radixSort1(FieldI& data) {
        if(data.n < 2) return;
        int maxNum = maxBitAndNum(data)[0];
        int cntBit = maxBitAndNum(data)[1];
        int minNum = maxBitAndNum(data)[2];
        if(minNum < 0) 
            for(int i = 0; i < data.n; i++) data[i] -= minNum;
        // cout << endl << "cntBit = " << cntBit << endl;
        // cout << "maxNum = " << maxNum << endl;        
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
            solver.radixSort1(arr1);
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

/*=====================================================================*/
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
    solver.radixSort(data);
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
    solver.radixSort1(data);
    timeE = chrono::high_resolution_clock::now();
    timeDelta = timeE - timeS;
    cout << endl << "After sort1:" << timeDelta.count() <<" s" <<endl;
    Functions::dataShow(data);
    cout << endl;
    return 0;
}