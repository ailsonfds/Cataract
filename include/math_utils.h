#ifndef __MATH_UTILS_H__
#define __MATH_UTILS_H__

#include <math.h>

#define PI 3.14159
#define E 2.71828

double mean(int *range, int n);
double std_deviation(int *range, int n);
int max_in_range(int *range, int n);
int max(int a, int b);
int min(int a, int b);

#endif /** __MATH_UTILS_H__ */