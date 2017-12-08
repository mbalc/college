// derived from http://www.geeksforgeeks.org/merge-sort/

#include "../l2/log.h"
#include <algorithm>
#include <future>
#include <iostream>
#include <thread>
#include <vector>

const int sze = 10000000, thres = 1000;
const bool disableConcurrent = true;

#define DB if (false)

std::vector<int> arr;
std::vector<std::future<void>> futures;

/* C program for Merge Sort */

// Merges two subarrays of arr[].
// First subarray is arr[l..m]
// Second subarray is arr[m+1..r]
void merge(int l, int m, int r) {
  int i, j, k;
  int n1 = m - l + 1;
  int n2 = r - m;

  /* create temp arrays */
  std::vector<int> L(n1), R(n2);

  /* Copy data to temp arrays L[] and R[] */
  for (i = 0; i < n1; i++)
    L[i] = arr[l + i];
  for (j = 0; j < n2; j++)
    R[j] = arr[m + 1 + j];

  /* Merge the temp arrays back into arr[l..r]*/
  i = 0; // Initial index of first subarray
  j = 0; // Initial index of second subarray
  k = l; // Initial index of merged subarray
  while (i < n1 && j < n2) {
    if (L[i] <= R[j]) {
      arr[k] = L[i];
      i++;
    } else {
      arr[k] = R[j];
      j++;
    }
    k++;
  }

  /* Copy the remaining elements of L[], if there
  are any */
  while (i < n1) {
    arr[k] = L[i];
    i++;
    k++;
  }

  /* Copy the remaining elements of R[], if there
  are any */
  while (j < n2) {
    arr[k] = R[j];
    j++;
    k++;
  }
}

/* l is for left index and r is right index of the
sub-array of arr to be sorted */
void mergeSort(int l, int r) {
  if (l < r) {
    // Same as (l+r)/2, but avoids overflow for
    // large l and h
    int m = l + (r - l) / 2;

    std::future<void> f1;
    // Sort first and second halves
    DB std::cout<<"launch for " << l << "; "<< m << "\n";
    if (r - l < thres || disableConcurrent) {
      mergeSort(l, m);
    }
    else {
      f1 = std::async(mergeSort, l, m);
    }
    DB std::cout<<"launch for " << m + 1 << "; "<< r << "\n";
    mergeSort(m + 1, r);

    if (r - l >= thres && !disableConcurrent) f1.get();

    merge(l, m, r);
  }
}

/* UTILITY FUNCTIONS */
/* Function to print an array */
void printArray(int size) {
  int i;
  for (i = 0; i < size; i++)
    printf("%d ", arr[i]);
  printf("\n");
}

/* Driver program to test above functions */
int main() {

  for (int i = 0; i < sze; ++i) {
    int a;
    std::cin >> a;
    arr.push_back(a);
  }
  int arr_size = arr.size();

  DB printf("Given array is \n");
  DB printArray(arr_size);

  DB std::cout<<"launch for " << 0 << "; "<< arr_size << "\n";
  mergeSort(0, arr_size - 1);

  DB printf("\nSorted array is \n");
  DB printArray(arr_size);
  return 0;
}
