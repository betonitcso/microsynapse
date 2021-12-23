#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>


typedef struct dim {
    int rows;
    int cols;
} dim;

double find_max(double** matrix, dim d, int is_unsigned);
void s_scale(double** matrix, dim d, double max);
double** transpose(double** matrix, dim d);
void free_matrix(double** matrix, dim d);
void free_transpose(double** matrix, dim d);


#endif // MATRIX_H_INCLUDED
