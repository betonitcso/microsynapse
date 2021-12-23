#ifndef DATAFRAME_H_INCLUDED
#define DATAFRAME_H_INCLUDED

#include "matrix.h"
#include "string.h"

// to store matrices
typedef struct dataframe{
    double** matrix; // dynamically allocated 2d arr
    dim df_dim;
} dataframe;

dim get_dim(FILE* file, const char* sep);
dataframe read_csv(char* path, char *sep);
void head(dataframe d);
void free_df(dataframe d);

#endif // DATAFRAME_H_INCLUDED
