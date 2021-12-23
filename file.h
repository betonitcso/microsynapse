#ifndef FILE_H_INCLUDED
#define FILE_H_INCLUDED

#include "net.h"
#include "string.h"
#include <limits.h>

void save_net(model* m, char* weights_file, char* biases_file);
void load_net(model* m, char* weights_file, char* biases_file);

#endif // FILE_H_INCLUDED
