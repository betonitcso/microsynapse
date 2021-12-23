#ifndef NET_H_INCLUDED
#define NET_H_INCLUDED

#include "dataframe.h"
#include "stdbool.h"
#include <limits.h>
#include <time.h>


#define IS_PRIME true
#define NOT_PRIME false
#define ATYPE_SIGMOID 0
#define ATYPE_RELU 1
#define ATYPE_TANH 2



typedef enum activation{
    SIGMOID,
    RELU,
    TANH
} activation;

//type to store matrices to test on
typedef struct split {
    dataframe train;
    dataframe test;
} split;



typedef struct model {
    int no_inputs;
    int no_outputs;
    int no_hidden_layers;
    int added_layers;
    struct layer* layers;
} model;

typedef struct layer {
    int no_neurons;
    activation activation;

    struct neuron* neurons;
}layer;

typedef struct neuron {
    double output;
    double output_delta;
    double z;
    double z_delta;

    double bias;
    double bias_delta; //derivative of bias
    double* weights;
    double* weight_deltas; //derivatives of weights
} neuron;

struct split train_test_split(dataframe d, int no_params, double ratio, int* coords);

double scaled_rand();
double sigmoid(double no);
double relu(double no);
double actv(bool is_prime, activation type, double no);

struct neuron* alloc_neurons(const int no_neurons, const int outputs);
struct model* create_model(const int nips, const int nops, const int no_hidden_layers, activation output_activation);
void init(model* m);
void add(model* model, const int no_neurons, activation activation);

void feed_input(model* m, const double* inputs);
void fwd_pass(model* m, double* inputs);
void out_prop(model* m, double* outs);
void bwd_pass(model* m, double* outs);
void update_weights(model* m, double alpha);
int shuffle(long instances);
void fwrand(model* m);

double train(model* m, dataframe xt, dataframe y, double alpha, int epochs);
void test(model* m, dataframe xt, dataframe y);
void pred(model* m, double* inputs, double* preds);

//fns to free net.h structures
void free_model(model* m);
void free_split(split s);


#endif // NET_H_INCLUDED
