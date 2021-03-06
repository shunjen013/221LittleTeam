#include <stdio.h>
#include <math.h>
#include "utility.h"
#define VAR_THRES 1

unsigned long long int computeMean (unsigned long long int arr[], int len) {
    unsigned long long int res = 0;
    int idx;
    for (idx = 0; idx < len; idx ++) res += arr[idx];
    res /= len;
    return res;
}
unsigned long long int computeVarience 
    (unsigned long long int arr[], int len, unsigned long long int mean) {
    unsigned long long int res = 0;
    int idx;
    for (idx = 0; idx < len; idx ++) {
        if (arr[idx] > mean) res += (arr[idx] - mean)*(arr[idx] - mean);
        else res += (mean - arr[idx])*(mean - arr[idx]);
    }
    res /= len;
    return (unsigned long long int)sqrt(res);
}
unsigned long long int computeFilter 
    (unsigned long long int arr[], int len, unsigned long long int mean, unsigned long long int varience,
     unsigned long long int *arr_new, int *len_new) {
    unsigned long long int res = 0;
    unsigned long long int upper, lower;
    int idx;
    *len_new = 0;
    upper = mean + VAR_THRES*varience;
    lower = (mean > VAR_THRES*varience) ? mean - VAR_THRES*varience : 0;
    for (idx = 0; idx < len; idx ++) {
        if (arr[idx] < upper && arr[idx] > lower) {
            arr_new[*len_new] = arr[idx]; 
            res += arr[idx];
            (*len_new) ++;
        }
    }
    /*res /= *len_new;*/
    return res;
}

unsigned long long int filterByVarience 
    (unsigned long long int arr[], int len, unsigned long long int *arr_new, int *len_new) {
    unsigned long long int mean, var, ans;
    mean = computeMean (arr, len);
    var = computeVarience (arr, len, mean);
    ans = computeFilter(arr, len, mean, var, arr_new, len_new);
    return res;
}
