#ifndef NET_H_INCLUDED
#define NET_H_INCLUDED

#include "dataframe.h"
#include "debugmalloc.h"

//type to store matrices to test on
typedef struct split {
    double** train;
    double** test;
    int no_params;
    int train_instances;
    int test_instances;
} split;

typedef struct model {
    int no_inputs;
    int no_outputs;
    int no_hidden_layers;
    int added_layers;
    struct layer* layers;

    double* outputs;
} model;

typedef struct layer {
    int no_neurons;
    int activation;

    struct neuron* neurons;
}layer;


typedef struct neuron {
    double z;
    double z_prime;

    double bias;
    double bias_prime; //derivative of bias
    double* weights;
    double* weight_primes; //derivatives of weights
} neuron;

struct split train_test_split(dataframe d, int no_params, double ratio, const int* coords);
struct neuron* alloc_neurons(const int no_nodes, const int nops);
double scaled_rand(void);
double sigmoid(double no);
double activation(const int actv, double no);
double activation_prime(const int actv, double no);
struct model* create_model(const int nips, const int nops, const int no_hidden_layers, const int input_activation);
void add(model* model, const int no_neurons, char* activation);
void init(model* m);

#endif // NET_H_INCLUDED
