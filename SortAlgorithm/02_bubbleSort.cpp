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
    void bubbleSort(Field_<T>& data) {
        if(data.n < 2) return;
        for (int i = 0; i < data.n - 1; i++) {
            for (int j = 1; j < data.n - i; j++)
                if(data[j-1] > data[j])
                    swap(data, j, j - 1);
        }
    }

    void bubbleSort1(Field_<T>& data) {
        if(data.n < 2) return;
        for(int i = 0; i < data.n - 1; i++) {
            bool exchanged = false;
            for(int j = 0; j < data.n - 1 - i; j++) {
                if(data[j] > data[j+1]) {
                    swap(data, j, j + 1);
                    exchanged = true;
                }
            }
            if(!exchanged)
                return;
        }
    }

    void bubbleSort2(Field_<T>& data) {
        if(data.n < 2) return;
        int lastLoc = data.n;
        int tmpPos = 0;
        for(int i = 0; i < data.n - 1; i++) {
            bool exchanged = false;
            for(int j = 0; j < lastLoc - 1; j++) {
                if(data[j] > data[j+1]) {
                    swap(data, j, j + 1);
                    tmpPos = j + 1;
                    exchanged = true;
                }
            }
            lastLoc = tmpPos;
            if(!exchanged)
                return;
        }
    }

    void bubbleSort3(Field_<T>& data) {
        if(data.n < 2) return;
        int leftLasChangeIx = 0;
        int rightLastChangIx = data.n - 1;
        for (int i = 0; i < data.n / 2; i++) {
            int tmpIx = -1;
            for (int j = leftLasChangeIx; j < rightLastChangIx; j++) {
                if (data[j] > data[j + 1]) {
                    swap(data, j, j + 1);
                    tmpIx = j + 1;
                }
            }
            if(tmpIx == -1)
                break;
            else
                rightLastChangIx = tmpIx;
            tmpIx = -1;
            for(int k = rightLastChangIx; k >leftLasChangeIx; k--) {
                if(data[k] < data[k-1]){
                    swap(data, k, k - 1);
                    tmpIx = k - 1;
                }
            }
            if (tmpIx == -1)
                break;
            else
                leftLasChangeIx = tmpIx;
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
            solver.bubbleSort3(arr1);
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
    solver.bubbleSort(data);
    auto timeE = chrono::high_resolution_clock::now();
    chrono::duration<double> timeDelta = timeE - timeS;
    cout << endl << "After sort: " << timeDelta.count() <<" s" <<endl;
    Functions::dataShow(data);
    cout << endl << "===============================" <<endl;

    data = Functions::dataGeneration<int>(data);
    cout << endl << "Before sort:" << endl;
    Functions::dataShow(data);
    timeS = chrono::high_resolution_clock::now();
    solver.bubbleSort1(data);
    timeE = chrono::high_resolution_clock::now();
    timeDelta = timeE - timeS;
    cout << endl << "After sort2: " << timeDelta.count() <<" s" <<endl;
    Functions::dataShow(data);
    cout << endl << "===============================" <<endl;

    data = Functions::dataGeneration<int>(data);
    cout << endl << "Before sort:" << endl;
    Functions::dataShow(data);
    timeS = chrono::high_resolution_clock::now();
    solver.bubbleSort2(data);
    timeE = chrono::high_resolution_clock::now();
    timeDelta = timeE - timeS;
    cout << endl << "After sort3: " << timeDelta.count() <<" s" <<endl;
    Functions::dataShow(data);
    cout << endl << "===============================" <<endl;

    data = Functions::dataGeneration<int>(data);
    cout << endl << "Before sort:" << endl;
    Functions::dataShow(data);
    timeS = chrono::high_resolution_clock::now();
    solver.bubbleSort3(data);
    timeE = chrono::high_resolution_clock::now();
    timeDelta = timeE - timeS;
    cout << endl << "After sort4: " << timeDelta.count() <<" s" <<endl;
    Functions::dataShow(data);
    return 0;
}