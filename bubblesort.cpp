#include <iostream>
#include <omp.h>
#include <chrono>
using namespace std;
using namespace std::chrono;


void bubble(int array[], int n){
    for (int i = 0; i < n - 1; i++){
        for (int j = 0; j < n - i - 1; j++){
            if (array[j] > array[j + 1])
                swap(array[j], array[j + 1]);
        }
    }
}

void pBubble(int array[], int n){
    for(int i = 0; i < n; ++i){
        // Odd phase
        #pragma omp parallel for
        for (int j = 1; j < n - 1; j += 2){
            if (array[j] > array[j + 1])
                swap(array[j], array[j + 1]);
        }
        #pragma omp barrier

        // Even phase
        #pragma omp parallel for
        for (int j = 0; j < n - 1; j += 2){
            if (array[j] > array[j + 1])
                swap(array[j], array[j + 1]);
        }
        #pragma omp barrier
    }
}

void printArray(int arr[], int n){
    for(int i = 0; i < n; i++) cout << arr[i] << " ";
    cout << "\n";
}

int main(){
    
    // --- Option 1: Random elements
    const int n = 1000;
    int arr[n], brr[n];
    for (int i = 0; i < n; i++){
        arr[i] = rand() % 10000;
        brr[i] = rand() % 10000;
    }

    // (you can uncomment this block to use it instead of user input)
    // n = 20;
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
    bubble(arr, n);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Sequential Bubble Sort took: " << duration.count() / 1e6 << " seconds.\n";
    printArray(arr, n);

    // Parallel Bubble Sort
    start = high_resolution_clock::now();
    pBubble(brr, n);
    stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start);
    cout << "Parallel Bubble Sort took: " << duration.count() / 1e6 << " seconds.\n";
    printArray(brr, n);

    return 0;
}
