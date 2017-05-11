#ifndef UTILIYT_H
#define UTILIYT_H

#include <stdio.h>
#include <math.h>

unsigned long long int computeMean (unsigned long long int[], int);
unsigned long long int computeVarience 
    (unsigned long long int[], int, unsigned long long int);

unsigned long long int computeFilter 
    (unsigned long long int[], int, unsigned long long int, unsigned long long int,
     unsigned long long int*, int*);
unsigned long long int filterByVarience
    (unsigned long long int, int, unsigned long long int *, int *);

#endif
