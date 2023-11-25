#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <numeric>
using namespace std;

// 두 정렬된 벡터를 하나의 정렬된 벡터로 병합하는 함수
vector<int> merge(vector<int>& left, vector<int>& right) {
    vector<int> result;
    size_t i = 0, j = 0;
    // 왼쪽과 오른쪽 벡터에서 정렬된 순서대로 요소를 병합
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
    // 왼쪽 벡터의 나머지 요소를 추가
    while (i < left.size()) {
        result.push_back(left[i]);
        i++;
    }
    // 오른쪽 벡터의 나머지 요소를 추가
    while (j < right.size()) {
        result.push_back(right[j]);
        j++;
    }

    return result;
}
// 벡터에 대해 병합 정렬을 수행하는 함수
vector<int> mergesort(vector<int>& arr) {
    if (arr.size() <= 1) {
        return arr;
    }
    // 벡터의 중간 인덱스를 찾음
    size_t mid = arr.size() / 2;
    vector<int> left(arr.begin(), arr.begin() + mid);
    vector<int> right(arr.begin() + mid, arr.end());
    // 각 부분에 재귀적으로 병합 정렬을 적용
    left = mergesort(left);
    right = mergesort(right);

    return merge(left, right);
}
// 퀵정렬 알고리즘을 위해 벡터를 분할하는 함수
int partition(vector<int>& arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    // 피벗보다 작은 요소를 왼쪽으로 이동
    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    // 피벗을 올바른 위치로 이동
    swap(arr[i + 1], arr[high]);
    return i + 1;
}
// 벡터에 대해 퀵정렬을 수행하는 함수
void quicksort(vector<int>& arr, int low, int high) {
    if (low < high) {
        // 분할 인덱스를 찾음
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
    // 퀵정렬과 병합정렬의 실행 시간을 저장하는 벡터
    vector<double> quicksort_times;
    vector<double> mergesort_times;

    for (int i = 0; i < 10; i++) {
        // 통계적 정확성을 위해 비교를 여러 번 반복
        vector<int> data;
        for (size_t j = 0; j < n; j++) {
            data.push_back(rand() % 1000 + 1);
        }
        // 퀵정렬과 병합정렬을 위해 랜덤 벡터 복제
        vector<int> quicksort_data = data;
        vector<int> mergesort_data = data;

        // 퀵정렬 측정
        double quicksort_time = measure_time(mergesort, quicksort_data);
        quicksort_times.push_back(quicksort_time);

        // 합병정렬 측정
        double mergesort_time = measure_time(mergesort, mergesort_data);
        mergesort_times.push_back(mergesort_time);
    }
    // 퀵정렬과 병합정렬의 평균 실행 시간 계산
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
    for (size_t n : {10,100,200,1000,2000, 5000, 10000,15000,20000,50000,100000}) {
        compare_algorithms(n);
    }

    return 0;
}