#include "dataframe.h"
#include "debugmalloc.h"

// gets dimensions of a file, returns it in dim struct
dim get_dim(FILE* file, const char* sep) {
    if(file == NULL) {
        perror("[ERR] file is NULL.");
        exit(103);
    }
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

static double min(double x, double y) {
    return(x > y ? y : x);
}

// reads csv, turns it into a 2D arr of double type, then returns it in a dataframe
dataframe read_csv(char* path, char *sep) {
    char buff[1024];
    FILE* fp;
    fp = fopen(path, "r");
    if(fp == NULL) {
        perror("[ERR] file is NULL.");
        exit(2132);
    }

    dataframe d;
    double** matrix;
    d.df_dim = get_dim(fp, sep);

    // allocates memory for the matrix that holds the csv's values
    matrix = (double**) malloc(sizeof(double) * d.df_dim.rows);
    for(int i = 0; i < d.df_dim.rows; i++) {
            matrix[i] = (double*) malloc(sizeof(double) * d.df_dim.cols);
    }


    int i = 0;
    fclose(fp);

    fp = fopen(path, "r");
    if(fp == NULL) {
        perror("[ERR] file is NULL.");
        exit(2132);
    }
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

void head(dataframe d) {
    if(d.matrix == NULL) {
        printf("Dataframe does not contain any elements.\n");
    }
    int el = min(d.df_dim.rows, 5);
    printf("\n\n");
    for(int i = 0; i < el; i++) {
        for(int j = 0; j < d.df_dim.cols; j++) {
            printf("%f\t", d.matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n\n");
}

void free_df(dataframe d) {
    if(d.matrix == NULL) return;
    for(int i = 0; i < d.df_dim.rows; i++) {
        free(d.matrix[i]);
    }
    free(d.matrix);
}
