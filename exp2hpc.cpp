#include <iostream>
#include <vector>
#include <omp.h>

using namespace std;


// ================= PARALLEL BUBBLE SORT =================

void parallelBubbleSort(vector<int> &arr){

    bool swapped = true;

    int n = arr.size();

    while(swapped){

        swapped = false;

        // EVEN PHASE
        #pragma omp parallel for shared(arr,swapped)

        for(int i = 0; i < n - 1; i += 2){

            if(arr[i] > arr[i + 1]){

                swap(arr[i], arr[i + 1]);

                swapped = true;
            }
        }

        // ODD PHASE
        #pragma omp parallel for shared(arr,swapped)

        for(int i = 1; i < n - 1; i += 2){

            if(arr[i] > arr[i + 1]){

                swap(arr[i], arr[i + 1]);

                swapped = true;
            }
        }
    }
}



// ================= MERGE FUNCTION =================

void mergeSort(vector<int> &arr,
               int left,
               int right,
               int mid){

    vector<int> temp;

    int i = left;

    int j = mid + 1;

    // Compare both halves
    while(i <= mid && j <= right){

        if(arr[i] <= arr[j]){

            temp.push_back(arr[i]);

            i++;
        }

        else{

            temp.push_back(arr[j]);

            j++;
        }
    }

    // Remaining left half
    while(i <= mid){

        temp.push_back(arr[i]);

        i++;
    }

    // Remaining right half
    while(j <= right){

        temp.push_back(arr[j]);

        j++;
    }

    // Copy sorted data back
    for(int k = 0; k < temp.size(); k++){

        arr[left + k] = temp[k];
    }
}



// ================= PARALLEL MERGE SORT =================

void parallelMS(vector<int>& arr,
                int left,
                int right){

    // Base condition
    if(left < right){

        int mid = (left + right) / 2;

        // Parallel sections
        #pragma omp parallel sections
        {

            // Left half
            #pragma omp section
            {
                parallelMS(arr, left, mid);
            }

            // Right half
            #pragma omp section
            {
                parallelMS(arr, mid + 1, right);
            }
        }

        // Merge sorted halves
        mergeSort(arr, left, right, mid);
    }
}



// ================= MAIN FUNCTION =================

int main() {

    vector<int> arr1 = {
        9, 4, 7, 2, 1, 8, 3, 6
    };

    vector<int> arr2 = arr1;

    omp_set_num_threads(4);


    // Parallel Bubble Sort
    parallelBubbleSort(arr1);

    cout << "Parallel Bubble Sort:\n";

    for(int num : arr1) {

        cout << num << " ";
    }

    cout << endl << endl;


    // Parallel Merge Sort
    parallelMS(arr2, 0, arr2.size() - 1);

    cout << "Parallel Merge Sort:\n";

    for(int num : arr2) {

        cout << num << " ";
    }

    cout << endl;

    return 0;
}