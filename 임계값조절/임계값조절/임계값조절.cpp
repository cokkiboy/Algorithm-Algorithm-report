#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <numeric>

using namespace std;

vector<int> insertion_sort(vector<int>& arr) {
    for (size_t i = 1; i < arr.size(); i++) {
        int key = arr[i];
        int j = i - 1;

        while (j >= 0 && key < arr[j]) {
            arr[j + 1] = arr[j];
            j--;
        }

        arr[j + 1] = key;
    }

    return arr;
}

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

vector<int> merge_vectors(vector<int>& left, vector<int>& middle, vector<int>& right) {
    vector<int> result;
    result.reserve(left.size() + middle.size() + right.size());

    result.insert(result.end(), left.begin(), left.end());
    result.insert(result.end(), middle.begin(), middle.end());
    result.insert(result.end(), right.begin(), right.end());

    return result;
}

vector<int> quicksort_with_threshold(vector<int>& arr, size_t threshold) {
    if (arr.size() <= threshold) {
        return insertion_sort(arr);
    }
    else {
        int pivot = arr[arr.size() / 2];
        vector<int> left, middle, right;

        for (int num : arr) {
            if (num < pivot) {
                left.push_back(num);
            }
            else if (num == pivot) {
                middle.push_back(num);
            }
            else {
                right.push_back(num);
            }
        }

        left = quicksort_with_threshold(left, threshold);
        right = quicksort_with_threshold(right, threshold);

        return merge_vectors(left, middle, right);
    }
}

double measure_time_with_threshold(size_t threshold, vector<int>& data) {
    clock_t start_time = clock();
    quicksort_with_threshold(data, threshold);
    clock_t end_time = clock();

    return static_cast<double>(end_time - start_time) / CLOCKS_PER_SEC;
}

void analyze_threshold_performance(size_t n) {
    vector<size_t> thresholds(n + 1);
    iota(thresholds.begin(), thresholds.end(), 0);

    vector<double> avg_times;

    for (size_t threshold : thresholds) {
        vector<double> threshold_times;

        for (int i = 0; i < 10; i++) {
            vector<int> data;

            for (size_t j = 0; j < n; j++) {
                data.push_back(rand() % 1000 + 1);
            }

            // 퀵정렬 (임계값 조정) 측정
            double threshold_time = measure_time_with_threshold(threshold, data);
            threshold_times.push_back(threshold_time);
        }

        double avg_threshold_time = accumulate(threshold_times.begin(), threshold_times.end(), 0.0) / threshold_times.size();
        avg_times.push_back(avg_threshold_time);
    }

    // 최적의 임계값 찾기
    auto min_avg = min_element(avg_times.begin(), avg_times.end());

    // 최적의 임계값과 해당 임계값에서의 실행 시간 출력
    size_t optimal_threshold = distance(avg_times.begin(), min_avg);
    cout << "Optimal Threshold: " << optimal_threshold << endl;
    cout << "Average Time with Optimal Threshold: " << *min_avg << " seconds" << endl;
}

int main() {
    srand(static_cast<unsigned>(time(0)));

    // 데이터 크기에 따른 성능 측정
    for (size_t n : {1000, 5000, 10000}) {
        analyze_threshold_performance(n);
    }

    return 0;
}
