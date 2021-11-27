#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "debugmalloc.h"

typedef struct dim {
    int rows;
    int cols;
} dim;

double find_max(double** matrix, dim d, int is_unsigned);
void s_scale(double** matrix, dim d, double max);
double dot(const double* a_vec, const double* b_vec, int height);
double** transpose(double** matrix, dim d);


#endif // MATRIX_H_INCLUDED
