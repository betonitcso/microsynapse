#ifndef DATAFRAME_H_INCLUDED
#define DATAFRAME_H_INCLUDED

#include "matrix.h"
#include "string.h"
#include "debugmalloc.h"

// to store matrices
typedef struct dataframe{
    double** matrix; // dynamically allocated 2d arr
    dim df_dim;
} dataframe;

// gets dimensions of a file, returns it in dim struct
dim get_dim(FILE* file, const char* sep);
void read_err(FILE* fp, char* path);
void alloc_err(double** matrix);
dataframe read_csv(char* path, char *sep);

#endif // DATAFRAME_H_INCLUDED
