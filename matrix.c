#include "matrix.h"

double find_max(double** matrix, dim d, int is_unsigned) {
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
    for(int i = 0; i < d.rows; i++) {
        for (int j = 0; j < d.cols; j++) {
            matrix[i][j] = matrix[i][j] / max;
        }
    }
}

double dot(const double* a_vec, const double* b_vec, int height) {
    double sum = 0;
    for(int i = 0; i < height; i++) {
        sum += a_vec[i] * b_vec[i];
    }
    return sum;
}

//transposing a matrix (rows -> cols & cols -> rows)
double** transpose(double** matrix, dim d) {
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
