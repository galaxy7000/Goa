#include <iostream>
#include <omp.h>
#include <chrono>
using namespace std;
using namespace std::chrono;

void merge(int arr[], int low, int mid, int high) {
    // Create arrays of left and right partititons
    int n1 = mid - low + 1;
    int n2 = high - mid;

    int left[n1];
    int right[n2];
    
    // Copy all left elements
    for (int i = 0; i < n1; i++) left[i] = arr[low + i];
    
    // Copy all right elements
    for (int j = 0; j < n2; j++) right[j] = arr[mid + 1 + j];
    
    // Compare and place elements
    int i = 0, j = 0, k = low;

    while (i < n1 && j < n2) {
        if (left[i] <= right[j]){
            arr[k] = left[i];
            i++;
        } 
        else{
            arr[k] = right[j];
            j++;
        }
        k++;
    }

    // If any elements are left out
    while (i < n1) {
        arr[k] = left[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = right[j];
        j++;
        k++;
    }
}

void parallelMergeSort(int arr[], int low, int high) {
    if (low < high) {
        int mid = (low + high) / 2;

        #pragma omp parallel sections
        {
            #pragma omp section
            {
                parallelMergeSort(arr, low, mid);
            }

            #pragma omp section
            {
                parallelMergeSort(arr, mid + 1, high);
            }
        }
        merge(arr, low, mid, high);
    }
}

void mergeSort(int arr[], int low, int high) {
    if (low < high) {
        int mid = (low + high) / 2;
        mergeSort(arr, low, mid);
        mergeSort(arr, mid + 1, high);
        merge(arr, low, mid, high);
    }
}

void printArray(int arr[], int n){
    for(int i = 0; i < n; i++) cout << arr[i] << " ";
    cout << "\n";
}

int main() {
    // --- Option 1: Random elements
    const int n = 10000;
    int arr[n], brr[n];
    for (int i = 0; i < n; i++){
        arr[i] = rand() % 100000;
        brr[i] = arr[i];
    }

    // (you can uncomment this block to use it instead of user input)
    // int n = 20;
    // int arr[] = {45, 23, 89, 12, 78, 67, 90, 34, 21, 56, 11, 44, 39, 72, 30, 19, 99, 85, 63, 7};
    // int brr[20];
    // for(int i = 0; i < n; i++) brr[i] = arr[i];
    

    // User input block
    // int n;
    // cout << "Enter number of elements: ";
    // cin >> n;

    // int* arr = new int[n];
    // int* brr = new int[n];

    // cout << "Enter " << n << " elements:\n";
    // for(int i = 0; i < n; i++) {
    //     cin >> arr[i];
    //     brr[i] = arr[i];  // copy for parallel sort
    // }
    
    
    auto start = high_resolution_clock::now();
    mergeSort(arr, 0, n);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Sequential Bubble Sort took: " << duration.count() / 1e6 << " seconds.\n";
    // printArray(arr, n);

    // Parallel Bubble Sort
    start = high_resolution_clock::now();
    parallelMergeSort(brr, 0, n);
    stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start);
    cout << "Parallel Bubble Sort took: " << duration.count() / 1e6 << " seconds.\n";
    // printArray(brr, n);
    
    return 0;
}