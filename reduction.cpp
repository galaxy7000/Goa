#include <iostream>
#include <vector>
#include <omp.h>

using namespace std;

int main() {
    // int n;
    // cout << "Enter number of elements: ";
    // cin >> n;
    // vector<int> data(n);
    // cout << "Enter elements:\n";
    // for (int i = 0; i < n; ++i) cin >> data[i];

    const int n = 10000;
    int data[n], brr[n];
    for (int i = 0; i < n; i++){
        data[i] = rand() % 100000;
    }

    int minVal = data[0], maxVal = data[0];
    long long sum = 0;

    #pragma omp parallel for reduction(min:minVal) reduction(max:maxVal) reduction(+:sum)
    for (int i = 0; i < n; ++i) {
        minVal = min(minVal, data[i]);
        maxVal = max(maxVal, data[i]);
        sum += data[i];
    }

    double avg = static_cast<double>(sum) / n;
    cout << "Min: " << minVal << "\nMax: " << maxVal << "\nSum: " << sum << "\nAverage: " << avg << endl;

    return 0;
}
