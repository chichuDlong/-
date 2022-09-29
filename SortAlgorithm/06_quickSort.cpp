#include <cstdlib>
#include <ios>
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
    void quickSort(Field_<T>& data, int low, int high) {
        if(low >= high) return;
        int pivot = partition(data, low, high);
        quickSort(data, low, pivot - 1);
        quickSort(data, pivot + 1, high);
    }

    void dualQuickSort(Field_<T>& data, int start, int end) {
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

    int partition(Field_<T>& data, int low, int high) {
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

    void swap(Field_<T>& data, int ix, int jx) {
        T tmp = data[ix];
        data[ix] = data[jx];
        data[jx] = tmp;
    }

    void quickSort1(Field_<T>& data, int L, int R) {
        if (L < R) {
            int index = rand() % (R - L + 1) + L;
            swap(data, index, R);
            vector<int> p = partition1(data, L, R);
            quickSort1(data, L, p[0] - 1);
            quickSort1(data, p[1] + 1, R);
        }
    }

    vector<int> partition1(Field_<T>& data, int L, int R) {
        int less =L - 1; // < 区右边界
        int more = R;    // > 区左边界
        while(L < more) {
            if (data[L] < data[R]) swap(data, ++less, L++);
            else if (data[L] > data[R]) swap(data, --more, L);
            else L++;
        }
        swap(data, more, R);
        return {less + 1, more};
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

        int testTime = 200000;
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
            solver.quickSort1(arr1, 0, arr1.n - 1);
            //solver.dualQuickSort(arr1, 0, arr1.n - 1);
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
    solver.quickSort(data, 0, data.n - 1);
    auto timeE = chrono::high_resolution_clock::now();
    chrono::duration<double> timeDelta = timeE - timeS;
    cout << endl << "After sort: " << timeDelta.count() <<" s" <<endl;
    Functions::dataShow(data);

    cout << endl << "===============================" <<endl;
    data = Functions::dataGeneration<int>(data);
    cout << endl << "Before sort:" << endl;
    Functions::dataShow(data);
    timeS = chrono::high_resolution_clock::now();
    solver.dualQuickSort(data, 0, data.n - 1);
    timeE = chrono::high_resolution_clock::now();
    timeDelta = timeE - timeS;
    cout << endl << "After sort1: " << timeDelta.count() <<" s" <<endl;
    Functions::dataShow(data);

    cout << endl << "===============================" <<endl;
    data = Functions::dataGeneration<int>(data);
    cout << endl << "Before sort:" << endl;
    Functions::dataShow(data);
    timeS = chrono::high_resolution_clock::now();
    solver.quickSort1(data, 0, data.n - 1);
    timeE = chrono::high_resolution_clock::now();
    timeDelta = timeE - timeS;
    cout << endl << "After sort1: " << timeDelta.count() <<" s" <<endl;
    Functions::dataShow(data);
    cout << endl;
    return 0;
}