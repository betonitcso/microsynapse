#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <time.h>
// #include <stdbool.h>


// to store df dimensions
typedef struct dim {
    int rows;
    int cols;
} dim;

// to store matrices
typedef struct dataframe {
    double** matrix; // dynamically allocated 2d arr
    dim df_dim;
} dataframe;

typedef struct layer {
    char activation[10];
    char type[10];
    int n_neurons;
} layer;


// gets dimensions of a file, returns it in dim struct
dim get_dim(FILE* file, const char* sep) {
    dim d;char buff[1024];
    int c = 1, r = 1; // cols & rows

    // iterates through the 1st row, increments c when character is a comma
    fgets(buff, 1024, file);
    for(int i = 0; buff[i] != '\n'; i++) {
        if(buff[i] == *sep) {
            c++;
        }
    }

    // iterates through rows and increments r
    while(fgets(buff, 1024, file) != NULL) {
        r++;
    }

    // assigns values to output dim
    d.rows = r;
    d.cols = c;
    return d;
}

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

void read_err(FILE* fp, char* path) {
    if(fp == NULL) {
        printf("Cannot read %s", path);
        exit(0);
    }
}

void alloc_err(double** matrix) {
    if(matrix == NULL) {
        printf("Failed to allocate memory. ");
        exit(0);
    }
}

// random initialization of weights
double rand_weight() {
    srand((int) time(NULL));
    return ((double) rand()) / ((double) RAND_MAX);
}

// reads csv, turns it into a 2D arr of double type, then returns it in a dataframe
dataframe read_csv(char* path, char *sep) {

    FILE* fp;
    fp = fopen(path, "r");

    // throws an error if file is not readable
    read_err(fp, path);

    dataframe d;
    double** matrix;
    d.df_dim = get_dim(fp, sep);

    // allocates memory for the matrix that holds the csv's values
    matrix = (double**) malloc(sizeof(double) * d.df_dim.rows);
    for(int i = 0; i < d.df_dim.rows; i++) {
        for(int j = 0; j < d.df_dim.cols; j++) {
            matrix[i] = (double*) malloc(sizeof(double) * d.df_dim.cols);
        }
    }

    // throws error if memory allocation fails
    alloc_err(matrix);

    //DUMMY LOOP - REPLACE WITH AN ACTUAL FUNCTION THAT READS FILES
    for(int k = 0; k < d.df_dim.rows; k++) {
        for(int l = 0; l < d.df_dim.cols; l++) {
            matrix[k][l] = k + l;
        }
    }

    d.matrix = matrix;
    fclose(fp);
    return d;
}

dataframe fast_csv(char* path, char *sep, dim d) {
    FILE* fp;
    fp = fopen(path, "r");

    // throws an error if file is not readable
    read_err(fp, path);

    dataframe df;
    double** matrix;
    df.df_dim = d;

    matrix = (double**) malloc(sizeof(double) * df.df_dim.rows);
    for(int i = 0; i < df.df_dim.rows; i++) {
        for(int j = 0; j < df.df_dim.cols; j++) {
            matrix[i] = (double*) malloc(sizeof(double) * df.df_dim.cols);
        }
    }

    /* READS & TOKENIZES FILE
     *
     *
     *
     *
     *
     *
     *
     *
     */

    // throws error if memory allocation fails
    alloc_err(matrix);

    df.matrix = matrix;
    fclose(fp);
    return df;
}


int main() {
    char p[] = "C:\\infoc\\exec\\test.csv";
    dataframe df = read_csv(p, ",");
    double max = find_max(df.matrix, df.df_dim, 1);
    s_scale(df.matrix, df.df_dim, max);
    for(int i = 0; i < df.df_dim.rows; i++) {
        for(int j = 0; j < df.df_dim.cols; j++) {
            printf("%.3f ", df.matrix[i][j]);
        }
    }
    return 0;
}