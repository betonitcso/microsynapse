#include "net.h"


struct split train_test_split(dataframe d, int no_params, double ratio, const int* coords) {

    split sp;

    int instances = d.df_dim.rows; //all instances
    int train_no = (int) d.df_dim.rows * ratio; //training instances (will always round them down but whatever)
    int test_no = d.df_dim.rows - train_no; //testing instances

    double** train_out;
    double** test_out;

    //transpose matrix from input df so copying becomes easier
    double** matrix_t = transpose(d.matrix, d.df_dim);
    for(int i = 0; i < d.df_dim.cols; i++) {
        for(int j = 0; j < d.df_dim.rows; j++) {
        }
    }
    assert(matrix_t != NULL);


    //allocating mem for array
    train_out = (double**) malloc(sizeof(double) * no_params);
    test_out = (double**) malloc(sizeof(double) * no_params);
    assert(train_out != NULL && test_out != NULL);

    for(int i = 0; i < no_params; i++) {
        train_out[i] = (double*) malloc(sizeof(double) * train_no);
        assert(train_out[i] != NULL);
        test_out[i] = (double*) malloc(sizeof(double) * test_no);
        assert(test_out[i] != NULL);
    }

    //copying values
    int idx = 0;
    for(int i = 0; i < no_params; i++) {
        for(idx; idx < train_no; idx++) {
            train_out[i][idx] = matrix_t[coords[i]][idx];
            assert(train_out[i][idx] == matrix_t[coords[i]][idx]);
        }

        for(int idt = 0; idt < test_no; idt++) {
            test_out[i][idt] = matrix_t[coords[i]][idt+train_no];
            assert(test_out[i][idt] == matrix_t[coords[i]][idt+train_no]);
        }
        idx = 0;
    }

    //assigning values to sp struct
    sp.train = train_out;
    sp.test = test_out;
    sp.no_params = no_params;
    sp.train_instances = train_no;
    sp.test_instances = test_no;

    return sp;
}


//allocates memory for all the neurons in a layer
struct neuron* alloc_neurons(const int no_nodes, const int nops) {
    neuron* nrs = (neuron*) malloc(sizeof(neuron) * no_nodes);
    for(int i = 0; i < no_nodes; i++) {
        nrs[i].weights = (double*) malloc(sizeof(double) * nops);
        nrs[i].weight_primes = (double*) malloc(sizeof(double) * nops);
    }
    return nrs;
}

double scaled_rand() {
    return ((double) rand()) / ((double) RAND_MAX);
}

double sigmoid(double no) {
    return 1.0 / (1.0 + exp(-no));
}

double activation(const int actv, double no) {
    double sigma;
    switch(actv) {
        case 0: sigma = sigmoid(no); //sigmoid
        case 1: if(no <= 0) {sigma = 0;} else sigma = no; // relu
        case 2: sigma = tanh(no); //tanh
    }
    return sigma;
}

double activation_prime(const int actv, double no) {
    double sigma_prime;
    switch(actv) {
        case 0: sigma_prime = sigmoid(no) * (1-sigmoid(no));
        case 1: sigma_prime = no <= 0 ? 0 : 1;
        case 2: sigma_prime = 1 - pow(tanh(no), 2);
    }
    return sigma_prime;
}


struct model* create_model(const int nips, const int nops, const int no_hidden_layers, const int input_activation) {
    struct model* m = (model*) malloc(sizeof(model*));

    m->no_inputs = nips;
    m->no_outputs = nops;
    m->no_hidden_layers = no_hidden_layers;
    m->added_layers = 0;
    m->layers = (layer*) malloc(sizeof(layer) * (no_hidden_layers+1));
    m->outputs = (double*) malloc(sizeof(double) * nops);

    //initialize first layer
    m->layers[0].no_neurons = nips;
    m->layers[0].activation = input_activation;

    return m;
}

void add(model* model, const int no_neurons, char* activation) {

    int prev_nodes = model->layers[model->added_layers].no_neurons;

    if(model->added_layers > model->no_hidden_layers || model->added_layers > 17) {
        printf("\nCan't add more layers.");
        exit(0);
    }

    model->layers[model->added_layers+1].no_neurons = no_neurons;
    model->layers[model->added_layers].neurons = alloc_neurons(prev_nodes, no_neurons);

    if(strcmp(activation, "sigmoid") == 0) {
        model->layers[model->added_layers+1].activation = 0;
    }
    else if(strcmp(activation, "relu") == 0) {
        model->layers[model->added_layers+1].activation = 1;
    }
    else if(strcmp(activation, "tanh") == 0) {
        model->layers[model->added_layers+1].activation = 2;
    }
    else {
        printf("invalid activation function.");
        exit(2);
    }
    model->added_layers += 1;
}



void init(model* m) {
    int no_layers = m->no_hidden_layers + 1;
    //initialize weights randomly
    for(int i = 0; i < no_layers-1; i++) { //for every layer
        for(int j = 0; j < m->layers[i].no_neurons; j++) { //for each neuron in a layer
            printf("%d: ", i);
            for(int k = 0; k < m->layers[i+1].no_neurons; k++) { //for each weight in a neuron
                m->layers[i].neurons[j].weights[k] = scaled_rand();
                printf("%f ", m->layers[i].neurons[j].weights[k]);
            }
            printf("\n");
            //initialize biases randomly
            m->layers[i].neurons[j].bias = scaled_rand();
        }
    }
};
