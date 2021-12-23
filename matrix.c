#include "matrix.h"
#include "debugmalloc.h"

double find_max(double** matrix, dim d, int is_unsigned) {

    if(matrix == NULL) {
        perror("[ERR] matrix can't be NULL.");
        exit("01031");
    }

    double max;
    if(is_unsigned) {
       max = matrix[0][0];
        for(int i = 0; i < d.rows; i++) {
            for (int j = 0; j < d.cols; j++) {
                if(matrix[i][j] > max) {
                    max = matrix[i][j];
                }
            }
        }
    }
    else {
        max = fabs(matrix[0][0]);
        for(int i = 0; i < d.rows; i++) {
            for (int j = 0; j < d.cols; j++) {
                if(matrix[i][j] > max) {
                    max = matrix[i][j];
                }
            }
        }
    }
    return max;
}

// standard scaler for df
void s_scale(double** matrix, dim d, double max) {

    if(matrix == NULL) {
        perror("[ERR] matrix can't be NULL.");
        exit("01031");
    }

    for(int i = 0; i < d.rows; i++) {
        for (int j = 0; j < d.cols; j++) {
            matrix[i][j] = matrix[i][j] / max;
        }
    }
}


//transposing a matrix (rows -> cols & cols -> rows)
double** transpose(double** matrix, dim d) {

    if(matrix == NULL) {
        perror("[ERR] matrix can't be NULL.");
        exit("01031");
    }

    double** matrix_t;
    matrix_t = (double**) malloc(sizeof(double)*d.cols);
    for(int i = 0; i < d.cols; i++) {
        matrix_t[i] = (double*) malloc(sizeof(double) * d.rows);
    }


    for(int j = 0; j < d.rows; j++) {
        for(int k = 0; k < d.cols; k++) {
            matrix_t[k][j] = matrix[j][k];
        }
    }

    return matrix_t;
}

void free_matrix(double** matrix, dim d) {

    if(matrix == NULL) return;

    for(int i = 0; i < d.rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

void free_transpose(double** matrix, dim d) {

    if(matrix == NULL) return;

    for(int i = 0; i < d.cols; i++) {
        free(matrix[i]);
    }
    free(matrix);
}
