#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <numeric>
using namespace std;

vector<int> merge(vector<int>& left, vector<int>& right) {
    vector<int> result;
    size_t i = 0, j = 0;

    while (i < left.size() && j < right.size()) {
        if (left[i] < right[j]) {
            result.push_back(left[i]);
            i++;
        }
        else {
            result.push_back(right[j]);
            j++;
        }
    }

    while (i < left.size()) {
        result.push_back(left[i]);
        i++;
    }

    while (j < right.size()) {
        result.push_back(right[j]);
        j++;
    }

    return result;
}

vector<int> mergesort(vector<int>& arr) {
    if (arr.size() <= 1) {
        return arr;
    }

    size_t mid = arr.size() / 2;
    vector<int> left(arr.begin(), arr.begin() + mid);
    vector<int> right(arr.begin() + mid, arr.end());

    left = mergesort(left);
    right = mergesort(right);

    return merge(left, right);
}

int partition(vector<int>& arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }

    swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quicksort(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);

        quicksort(arr, low, pi - 1);
        quicksort(arr, pi + 1, high);
    }
}

double measure_time(vector<int>(*algorithm)(vector<int>&), vector<int>& data) {
    clock_t start_time = clock();
    algorithm(data);
    clock_t end_time = clock();

    return static_cast<double>(end_time - start_time) / CLOCKS_PER_SEC;
}

void compare_algorithms(size_t n) {
    vector<double> quicksort_times;
    vector<double> mergesort_times;

    for (int i = 0; i < 10; i++) {
        vector<int> data;
        for (size_t j = 0; j < n; j++) {
            data.push_back(rand() % 1000 + 1);
        }

        vector<int> quicksort_data = data;
        vector<int> mergesort_data = data;

        // 퀵정렬 측정
        double quicksort_time = measure_time(mergesort, quicksort_data);
        quicksort_times.push_back(quicksort_time);

        // 합병정렬 측정
        double mergesort_time = measure_time(mergesort, mergesort_data);
        mergesort_times.push_back(mergesort_time);
    }

    double avg_quicksort_time = accumulate(quicksort_times.begin(), quicksort_times.end(), 0.0) / quicksort_times.size();
    double avg_mergesort_time = accumulate(mergesort_times.begin(), mergesort_times.end(), 0.0) / mergesort_times.size();

    cout << "Data size: " << n << endl;
    cout << "Average QuickSort Time: " << avg_quicksort_time << " seconds" << endl;
    cout << "Average MergeSort Time: " << avg_mergesort_time << " seconds" << endl;
    cout << "------" << endl;
}

int main() {
    srand(static_cast<unsigned>(time(0)));

    // 데이터 크기에 따른 성능 측정
    for (size_t n : {1000, 5000, 10000}) {
        compare_algorithms(n);
    }

    return 0;
}
