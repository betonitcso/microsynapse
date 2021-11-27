#include "dataframe.h"

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

void read_err(FILE* fp, char* path) {
    if(fp == NULL) {
        printf("Cannot read %s", path);
        exit(0);
    }
}

void alloc_err(double** matrix) {
    if(matrix == NULL) {
        perror("Failed to allocate memory. ");
        exit(0);
    }
}

// reads csv, turns it into a 2D arr of double type, then returns it in a dataframe
dataframe read_csv(char* path, char *sep) {
    char buff[1024];
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

    int i = 0;
    fclose(fp);

    fp = fopen(path, "r");
    while(fgets(buff, 1024, fp)) {
        char* token;
        token = strtok(buff, ",");
        int j = 0;

        while(token != NULL) {
            matrix[i][j] = atof(token);
            token = strtok(NULL, ",");
            j++;
        }
        i++;
    }
    fclose(fp);
    d.matrix = matrix;

    return d;
}
