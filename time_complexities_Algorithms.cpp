#include <iostream>
#include <algorithm>
#include <string>
#include <fstream>
#include <vector>
#include <chrono>
#include"functions.h"
using namespace std;
using namespace std::chrono;

// ---------- Utility Functions ----------
int getMax(const vector<int>& arr) {
    int mx = arr[0];
    for (int val : arr)
        if (val > mx) mx = val;
    return mx;
}

int getMin(const vector<int>& arr) {
    int mn = arr[0];
    for (int val : arr)
        if (val < mn) mn = val;
    return mn;
}

// ---------- Sorting Algorithms ----------
void insertionSort(vector<int>& arr) {
    for (int i = 1; i < arr.size(); i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void bubbleSort(vector<int>& arr) {
    for (int i = 0; i < arr.size() - 1; i++) {
        for (int j = 0; j < arr.size() - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

void merge(vector<int>& arr, int l, int m, int r) {
    int n1 = m - l + 1, n2 = r - m;
    vector<int> L(n1), R(n2);
    for (int i = 0; i < n1; i++) L[i] = arr[l + i];
    for (int j = 0; j < n2; j++) R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) arr[k++] = L[i++];
        else arr[k++] = R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

void mergeSort(vector<int>& arr, int l, int r) {
    if (l < r) {
        int m = (l + r) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

void heapify(vector<int>& arr, int n, int i) {
    int largest = i;
    int l = 2 * i + 1, r = 2 * i + 2;
    if (l < n && arr[l] > arr[largest]) largest = l;
    if (r < n && arr[r] > arr[largest]) largest = r;
    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);
    for (int i = n - 1; i >= 0; i--) {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}

int partition(vector<int>& arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSort(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

void countingSort(vector<int>& arr) {
    if (arr.empty()) return;
    int maxVal = getMax(arr);
    int minVal = getMin(arr);
    int range = maxVal - minVal + 1;

    vector<int> count(range, 0), output(arr.size());
    for (int num : arr) count[num - minVal]++;
    for (int i = 1; i < range; i++) count[i] += count[i - 1];
    for (int i = arr.size() - 1; i >= 0; i--) {
        output[count[arr[i] - minVal] - 1] = arr[i];
        count[arr[i] - minVal]--;
    }
    arr = output;
}

void countingSortForRadix(vector<int>& arr, int exp) {
    vector<int> output(arr.size());
    int count[10] = { 0 };
    for (int num : arr) count[(num / exp) % 10]++;
    for (int i = 1; i < 10; i++) count[i] += count[i - 1];
    for (int i = arr.size() - 1; i >= 0; i--) {
        output[count[(arr[i] / exp) % 10] - 1] = arr[i];
        count[(arr[i] / exp) % 10]--;
    }
    arr = output;
}

void radixSort(vector<int>& arr) {
    if (arr.empty()) return;

    // Handle negatives: shift all numbers by minVal
    int minVal = getMin(arr);
    if (minVal < 0) {
        for (int& num : arr) num -= minVal;
    }

    int m = getMax(arr);
    for (int exp = 1; m / exp > 0; exp *= 10)
        countingSortForRadix(arr, exp);

    // Shift back if needed
    if (minVal < 0) {
        for (int& num : arr) num += minVal;
    }
}

void bucketSort(vector<int>& arr) {
    if (arr.empty()) return;
    int n = arr.size();
    int maxVal = getMax(arr);
    int minVal = getMin(arr);

    // Shift if negatives exist
    int offset = 0;
    if (minVal < 0) {
        offset = -minVal;
        for (int& num : arr) num += offset;
        maxVal = getMax(arr);
    }

    vector<vector<int>> buckets(n);

    for (int i = 0; i < n; i++) {
        int idx = (long long)arr[i] * n / (maxVal + 1);
        if (idx >= n) idx = n - 1; // clamp
        buckets[idx].push_back(arr[i]);
    }

    arr.clear();
    for (int i = 0; i < n; i++) {
        sort(buckets[i].begin(), buckets[i].end());
        for (int val : buckets[i]) arr.push_back(val);
    }

    // Shift back
    if (offset > 0) {
        for (int& num : arr) num -= offset;
    }
}


// ---------- Main ----------
int main() {
    ifstream file("data.csv");
    if (!file.is_open()) {
        cout << "Error: Could not open data.csv" << endl;
        return 1;
    }

    vector<int> data;
    int num;

    string skipLine;
    file >> skipLine;

    while (file >> num) {
        data.push_back(num);
    }

    if (data.empty()) {
        cout << "Error: No data found in file." << endl;
        return 1;
    }

    cout << "Total numbers read: " << data.size() << endl << endl;

    int n2size = max(1, (int)(data.size() * 0.05));   // 5% for n^2 algos
    int nlognsize = max(1, (int)(data.size() * 0.10)); // 10% for nlogn algos

    cout << "Algorithm\t|\tTime Complexity\t|\tExecution Time (ms)" << endl;
    cout << "-------------------------------------------------------------" << endl;

    // Insertion Sort
    {
        vector<int> arr(data.begin(), data.begin() + n2size);
        auto start = high_resolution_clock::now();
        insertionSort(arr);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(stop - start);
        cout << "Insertion Sort\t|\tO(n^2)\t\t|\t" << duration.count() * 20 << endl;
    }

    // Bubble Sort
    {
        vector<int> arr(data.begin(), data.begin() + n2size);
        auto start = high_resolution_clock::now();
        bubbleSort(arr);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(stop - start);
        cout << "Bubble Sort\t|\tO(n^2)\t\t|\t" << duration.count() * 20 << endl;
    }

    // Merge Sort
    {
        vector<int> arr(data.begin(), data.begin() + nlognsize);
        auto start = high_resolution_clock::now();
        mergeSort(arr, 0, arr.size() - 1);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(stop - start);
        cout << "Merge Sort\t|\tO(n log n)\t|\t" << duration.count() * 10 << endl;
    }

    // Heap Sort
    {
        vector<int> arr(data.begin(), data.begin() + nlognsize);
        auto start = high_resolution_clock::now();
        heapSort(arr);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(stop - start);
        cout << "Heap Sort\t|\tO(n log n)\t|\t" << duration.count() * 10 << endl;
    }

    // Quick Sort
    {
        vector<int> arr(data.begin(), data.begin() + nlognsize);
        auto start = high_resolution_clock::now();
        quickSort(arr, 0, arr.size() - 1);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(stop - start);
        cout << "Quick Sort\t|\tO(n log n)\t|\t" << duration.count() * 10 << endl;
    }

    // Counting Sort
    {
        vector<int> arr = data;
        auto start = high_resolution_clock::now();
        countingSort(arr);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(stop - start);
        cout << "Counting Sort\t|\tO(n + k)\t|\t" << duration.count() << endl;
    }

    // Radix Sort
    {
        vector<int> arr = data;
        auto start = high_resolution_clock::now();
        radixSort(arr);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(stop - start);
        cout << "Radix Sort\t|\tO(nk)\t\t|\t" << duration.count() << endl;
    }

    // Bucket Sort
    {
        vector<int> arr = data;
        auto start = high_resolution_clock::now();
        bucketSort(arr);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(stop - start);
        cout << "Bucket Sort\t|\tO(n + k)\t|\t" << duration.count() << endl;
    }

    return 0;
}
