/*
Madhur Jaripatke
Roll No. 50
BE A Computer
RMDSSOE, Warje, Pune

Problem Statement: Write a program to implement Parallel Bubble Sort and Merge sort using OpenMP. Use 
existing algorithms and measure the performance of sequential and parallel algorithms.
*/

#include <iostream>
#include <vector>
#include <omp.h>
#include <chrono>
#include <random>

void sequentialBubbleSort(std::vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if (arr[j] > arr[j+1]) {
                std::swap(arr[j], arr[j+1]);
            }
        }
    }
}

void parallelBubbleSort(std::vector<int>& arr) {
    int n = arr.size();
    bool swapped = true;
    
    while (swapped) {
        swapped = false;
        #pragma omp parallel for shared(arr, swapped)
        for (int i = 0; i < n-1; i += 2) {
            if (arr[i] > arr[i+1]) {
                std::swap(arr[i], arr[i+1]);
                swapped = true;
            }
        }
        
        #pragma omp parallel for shared(arr, swapped)
        for (int i = 1; i < n-1; i += 2) {
            if (arr[i] > arr[i+1]) {
                std::swap(arr[i], arr[i+1]);
                swapped = true;
            }
        }
    }
}

void merge(std::vector<int>& arr, int left, int mid, int right) {
    std::vector<int> temp(right - left + 1);
    int i = left, j = mid + 1, k = 0;
    
    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j]) temp[k++] = arr[i++];
        else temp[k++] = arr[j++];
    }
    
    while (i <= mid) temp[k++] = arr[i++];
    while (j <= right) temp[k++] = arr[j++];
    
    for (i = 0; i < k; i++) {
        arr[left + i] = temp[i];
    }
}

void sequentialMergeSort(std::vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        sequentialMergeSort(arr, left, mid);
        sequentialMergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

void parallelMergeSort(std::vector<int>& arr, int left, int right, int depth = 0) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        
        if (depth < 3) {  // Limit parallel recursion depth
            #pragma omp parallel sections
            {
                #pragma omp section
                parallelMergeSort(arr, left, mid, depth + 1);
                #pragma omp section
                parallelMergeSort(arr, mid + 1, right, depth + 1);
            }
        } else {
            sequentialMergeSort(arr, left, mid);
            sequentialMergeSort(arr, mid + 1, right);
        }
        
        merge(arr, left, mid, right);
    }
}

int main() {
    const int SIZE = 50000;
    std::vector<int> arr(SIZE);
    
    // Initialize with random numbers
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 10000);
    for (int i = 0; i < SIZE; i++) {
        arr[i] = dis(gen);
    }

    std::vector<int> arr_copy = arr;
    
    // Set number of threads
    omp_set_num_threads(4);
    
    // Test Bubble Sort
    auto start = std::chrono::high_resolution_clock::now();
    sequentialBubbleSort(arr);
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Sequential Bubble Sort Time: " 
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() 
              << " ms\n";

    arr = arr_copy;
    start = std::chrono::high_resolution_clock::now();
    parallelBubbleSort(arr);
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Parallel Bubble Sort Time: " 
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() 
              << " ms\n";

    // Test Merge Sort
    arr = arr_copy;
    start = std::chrono::high_resolution_clock::now();
    sequentialMergeSort(arr, 0, arr.size() - 1);
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Sequential Merge Sort Time: " 
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() 
              << " ms\n";

    arr = arr_copy;
    start = std::chrono::high_resolution_clock::now();
    parallelMergeSort(arr, 0, arr.size() - 1);
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Parallel Merge Sort Time: " 
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() 
              << " ms\n";

    return 0;
}
