#include "net.h"
#include "debugmalloc.h"


struct split train_test_split(dataframe d, int no_params, double ratio, int* coords) {

    if(d.matrix == NULL) {
        perror("[ERR] dataframe must not be NULL.");
        exit(030);
    }
    if(d.df_dim.rows < 1 || d.df_dim.cols < 1) {
        perror("[ERR] dataframe's rows and cols must not be zero.");
        exit(031);
    }
    if(no_params < 1) {
        perror("[ERR] the number of parameters must be greater than zero.");
        exit(032);
    }

    split sp;
    dataframe train;
    dataframe test;
    int train_no = (int) d.df_dim.rows * ratio; //training instances (will always round them down but whatever)
    int test_no = d.df_dim.rows - train_no; //testing instances

    train.df_dim.rows = train_no;
    test.df_dim.rows = test_no;
    train.df_dim.cols = no_params;
    test.df_dim.cols = no_params;


    double** train_out;
    double** test_out;

    train_out = (double**) malloc(sizeof(double) * train_no);
    test_out = (double**) malloc(sizeof(double) * test_no);

    /*
    if(train_out == NULL || test_out == NULL) {
        perror("[ERR] can't allocate memory for split.");
        exit(033);
    }
    debugmalloc kills this
    */

    int idx = 0;
    for(idx; idx < train_no; idx++) {
        train_out[idx] = (double*) malloc(sizeof(double) * no_params);

        if(train_out[idx] == NULL) {
            perror("[ERR] can't allocate memory for train split.");
            exit(033);
        }

        for(int j = 0; j < no_params; j++) {
            train_out[idx][j] = d.matrix[idx][coords[j]];
        }
    }

    for(int i = 1; i < test_no; i++) {
        test_out[i] = (double*) malloc(sizeof(double) * no_params);

        if(test_out[i] == NULL) {
            perror("[ERR] can't allocate memory for test split.");
            exit(033);
        }

        for(int j = 0; j < no_params; j++) {
            test_out[i-1][j] = d.matrix[i+idx][coords[j]];
        }
    }

    train.matrix = train_out;
    test.matrix = test_out;
    sp.train = train;
    sp.test = test;

    return sp;
}


double scaled_rand() {
    return ((double) rand()) / ((double) RAND_MAX);
}

double sigmoid(double no) {
    return 1.0 / (1.0 + exp(-no));
}

double relu(double no) {
    if(no >= 0.0) return no;
    else return 0.0;
}

double actv(bool is_prime, activation type, double no) {
    if(!is_prime) {
        switch(type) {
            case SIGMOID: return sigmoid(no);
            case RELU: return relu(no);
            case TANH: return tanh(no);
        }
    }

    else {
        switch(type) {
            case SIGMOID: return ( sigmoid(no) * (1 - sigmoid(no)));
            case RELU: return (no > 0 ? 1 : 0);
            case TANH: return (1.0 - pow(tanh(no),2));
        }
    }
    perror("[ERR] Wrong activation type.");
    exit(0);
}

struct model* create_model(const int nips, const int nops, const int no_hidden_layers, activation output_activation) {

    if(nips < 1 || nops < 1 ) {
        perror("[ERR] the number of inputs or outputs can not be zero.");
        exit(034);
    }

    struct model* m = (model*) malloc(sizeof(model));

    if(m == NULL) {
        perror("[ERR] can't allocate memory for model.");
        exit(035);
    }

    m->no_inputs = nips;
    m->no_outputs = nops;
    m->no_hidden_layers = no_hidden_layers;
    m->added_layers = 0;
    m->layers = (layer*) malloc(sizeof(layer) * (no_hidden_layers+2));

    if(m->layers == NULL && no_hidden_layers != 0) {
        perror("[ERR] can't allocate memory for layers.");
        exit(036);
    }

    m->layers[0].no_neurons = nips;
    m->layers[no_hidden_layers+1].no_neurons = nops;
    m->layers[no_hidden_layers+1].activation = output_activation;

    return m;
}

//allocates memory for all the neurons in a layer
struct neuron* alloc_neurons(const int no_neurons, const int outputs) {
    neuron* nrs = (neuron*) malloc(sizeof(neuron) * no_neurons);

    if(nrs == NULL) {
        perror("[ERR] can't allocate neurons.");
        exit(011);
    }

    for(int i = 0; i < no_neurons; i++) {
        nrs[i].weights = (double*) malloc(sizeof(double) * outputs);
        nrs[i].weight_deltas = (double*) malloc(sizeof(double) * outputs);
    }
    return nrs;
}

void add(model* model, const int no_neurons, activation activation) {
    int prev_neurons = model->layers[model->added_layers].no_neurons;
    if(model->added_layers > model->no_hidden_layers || model->added_layers > 17) {
        perror("Can't add more layers.");
        exit(1);
    }

    model->layers[model->added_layers+1].no_neurons = no_neurons;
    model->layers[model->added_layers].neurons = alloc_neurons(prev_neurons, no_neurons);

    model->layers[model->added_layers+1].activation = activation;

    model->added_layers += 1;
}


//fills network's weights and biases randomly
void fwrand(model* m) {

    if(m == NULL) {
        perror("[ERR] model can't be NULL.");
        exit(0310);
    }

    int no_layers = m->no_hidden_layers;

    for(int i = 0; i < no_layers+1; i++) {
        for(int j = 0; j < m->layers[i].no_neurons; j++) {
            for(int k = 0; k < m->layers[i+1].no_neurons; k++) {
                m->layers[i].neurons[j].weights[k] = scaled_rand();
            }
        }
    }

    //initialize biases the same way (input layer doesn't have those)
    for(int i = 1; i < no_layers+2; i++) {
        for(int j = 0; j < m->layers[i].no_neurons; j++) {
            m->layers[i].neurons[j].bias = scaled_rand();
        }
    }
}



void init(model* m) {

    if(m == NULL) {
        perror("[ERR] model can't be NULL.");
        exit(0310);
    }

    int no_layers = m->no_hidden_layers;

    if(no_layers != m->added_layers) {
        perror("[ERR] The number of created layers and added layers do not match.");
        exit(012);
    }

    srand((int)time(NULL) % 10);

   //second to last layer & output layer neuron allocation
   m->layers[no_layers].neurons = alloc_neurons(m->layers[no_layers].no_neurons, m->no_outputs);
   m->layers[no_layers+1].neurons = alloc_neurons(m->no_outputs, 0);

    //initialize weights randomly
    fwrand(m);

}

void feed_input(model* m, const double* inputs) {

    if(m == NULL) {
        perror("[ERR] model can't be NULL.");
        exit(031);
    }

    for(int i = 0; i < m->no_inputs; i++) {
        m->layers[0].neurons[i].z = inputs[i];
        m->layers[0].neurons[i].output = inputs[i];
    }
}

void fwd_pass(model* m, double* inputs) {

    if(m == NULL) {
        perror("[ERR] model can't be NULL.");
        exit(0310);
    }

    if(inputs == NULL) {
        perror("[ERR] input field can't be NULL.");
        exit(0312);
    }

    feed_input(m, inputs);

    for(int i = 1; i < m->no_hidden_layers + 2; i++) {
        int a = m->layers[i].activation;
        for(int n = 0; n < m->layers[i].no_neurons; n++) {
            double z = m->layers[i].neurons[n].bias;

            for(int pz = 0; pz < m->layers[i-1].no_neurons; pz++) {
                z += m->layers[i-1].neurons[pz].output * m->layers[i-1].neurons[pz].weights[n];
            }

            m->layers[i].neurons[n].z = z;
            m->layers[i].neurons[n].output = actv(NOT_PRIME, a, z);
        }
    }

}

void out_prop(model* m, double* outs) {

    if(m == NULL || outs == NULL) {
        perror("[ERR] params can't be null.");
        exit(0313);
    }

    for(int j=0; j < m->no_outputs;j++) {
        double n_out = m->layers[m->no_hidden_layers+1].neurons[j].output;
        m->layers[m->no_hidden_layers+1].neurons[j].z_delta = (n_out - outs[j]) * n_out * (1- n_out);
        double dz = m->layers[m->no_hidden_layers+1].neurons[j].z_delta;

        for(int k = 0; k < m->layers[m->no_hidden_layers].no_neurons; k++)
        {
            m->layers[m->no_hidden_layers].neurons[k].weight_deltas[j] = (dz * m->layers[m->no_hidden_layers].neurons[k].output);
            m->layers[m->no_hidden_layers].neurons[k].output_delta = m->layers[m->no_hidden_layers].neurons[k].weights[j] * dz;
        }

        m->layers[m->no_hidden_layers + 1].neurons[j].bias_delta = m->layers[m->no_hidden_layers + 1].neurons[j].z_delta;
    }
}


void bwd_pass(model* m, double* outs){

    if(m == NULL || outs == NULL) {
        perror("[ERR] params can't be null.");
        exit(034);
    }

    // Output Layer
    out_prop(m, outs);

    // Hidden Layers
    for(int i = m->no_hidden_layers; i > 0; i--)
    {
        for(int j=0; j < m->layers[i].no_neurons; j++)
        {


            double zd = m->layers[i].neurons[j].z_delta;
            if(m->layers[i].activation != ATYPE_RELU) {
                m->layers[i].neurons[j].z_delta = m->layers[i].neurons[j].output_delta * actv(IS_PRIME, m->layers[i].activation, zd);
            }

            else if(m->layers[i].neurons[j].z >= 0) {
                m->layers[i].neurons[j].z_delta = m->layers[i].neurons[j].output_delta;
            }
            else {
                m->layers[i].neurons[j].z_delta = 0;
            }


            for(int k=0; k < m->layers[i-1].no_neurons; k++)
            {
                m->layers[i-1].neurons[k].weight_deltas[j] = m->layers[i].neurons[j].z_delta * m->layers[i-1].neurons[k].output;

                if(i>1)
                {
                    m->layers[i-1].neurons[k].output_delta = m->layers[i-1].neurons[k].weights[j] * m->layers[i].neurons[j].z_delta;
                }
            }

            m->layers[i].neurons[j].bias_delta = m->layers[i].neurons[j].z_delta;
        }
    }
}


void update_weights(model* m, double alpha) {

    if(m == NULL) {
        perror("[ERR] params can't be null.");
        exit(031);
    }

    if(alpha <= 0.0) {
        perror("[ERR] learning rate can't be zero or less.");
        exit(02);
    }

    for(int i = 0; i < m->no_hidden_layers + 1; i++) {
        for(int k = 0; k < m->layers[i].no_neurons; k++) {
            for(int j = 0; j < m->layers[i+1].no_neurons; j++) {
                double wd = m->layers[i].neurons[k].weight_deltas[j];
                m->layers[i].neurons[k].weights[j] = m->layers[i].neurons[k].weights[j] - alpha * wd;
            }
        }
    }

    //update biases (with same delta as z's)
    for(int i = 1; i < m->no_hidden_layers + 2; i++) {
        for(int j = 0; j < m->layers[i].no_neurons; j++) {
            double b_delta = m->layers[i].neurons[j].z_delta;
            m->layers[i].neurons[j].bias = m->layers[i].neurons[j].bias - b_delta;
        }
    }
}


int shuffle(long instances) {

    if(instances <= 0) {
        perror("[ERR] shuffle instances can't be zero or less.");
        exit(0314);
    }

    if (instances < 2)  return 0;
    int s = (int) round(log10(instances));
    long shf = LONG_MAX;
    while(shf > instances-1) {
        shf = (int) round(scaled_rand() * pow(10, s));
    }
    return shf;
}


double train(model* m, dataframe xt, dataframe y, double alpha, int epochs) {

    if(xt.matrix == NULL || y.matrix == NULL) {
        perror("[ERR] matrices of dataframes can't be NULL.");
        exit(030);
    }

    if(m == NULL) {
        perror("[ERR] model can't be NULL.");
        exit(005);
    }

    if(epochs <= 0) {
        perror("[ERR] number of epochs must be greater than zero.");
        exit(010);
    }

    if(alpha <= 0.0) {
        perror("[ERR] learning rate must be greater than zero.");
        exit(012);
    }

    double global_error = 0.0;
    for(int i = 0; i < epochs; i++) {
        for(int j = 0; j < xt.df_dim.rows; j++) {
            int shf = shuffle(xt.df_dim.rows);

            fwd_pass(m, xt.matrix[shf]);
            global_error += fabs(m->layers[m->no_hidden_layers+1].neurons[0].output - y.matrix[shf][0]);
            bwd_pass(m, y.matrix[shf]);
            update_weights(m, 0.2);
        }

        if(i % 100 == 0 && i > 0) {
                global_error = 0.0;
        }
    }
    return global_error;
}

void test(model* m, dataframe xt, dataframe y) {

    if(xt.matrix == NULL || y.matrix == NULL) {
        perror("[ERR] matrices of dataframes can't be NULL.");
        exit(0314);
    }

    if(m == NULL) {
        perror("[ERR] model can't be NULL.");
        exit(0315);
    }

    for(int i = 0; i < xt.df_dim.rows; i++) {
        fwd_pass(m, xt.matrix[i]);
        double err = 0.0;
        for(int j = 0; j < m->no_outputs; j++) {
            err += fabs(m->layers[m->no_hidden_layers+1].neurons[j].output - y.matrix[i][j]);
        }
        printf("[TEST] [ INDEX: %d ] [ ", i);
        for(int j = 0; j < m->no_outputs; j++) {
            printf("Y-HAT: %f Y: %f ", m->layers[m->no_hidden_layers+1].neurons[j].output, y.matrix[i][j]);
        }
        printf("] [ TEST-ERR: %f ]\n", err);
    }
}

void free_model(model* m) {
    if(m == NULL) return;
    for(int i = 0; i < m->no_hidden_layers + 2; i++) {
            for(int j = 0; j < m->layers[i].no_neurons; j++) {
                free(m->layers[i].neurons[j].weights);
                free(m->layers[i].neurons[j].weight_deltas);
            }
        free(m->layers[i].neurons);
    }
    free(m->layers);
    free(m);
}

void free_split(split s) {

    if(s.test.matrix == NULL && s.train.matrix == NULL) return;

    for(int i = 0; i < s.train.df_dim.rows; i++) {
        free(s.train.matrix[i]);
    }
    for(int i = 0; i < s.test.df_dim.rows; i++) {
        free(s.test.matrix[i]);
    }
    free(s.test.matrix); free(s.train.matrix);
}

void pred(model* m, double* inputs, double* preds) {

    if(inputs == NULL || preds == NULL) {
        perror("[ERR] input and pred parameters can't be NULL.");
        exit(0316);
    }

    if(m == NULL) {
        perror("[ERR] model can't be NULL.");
        exit(0315);
    }

    fwd_pass(m, inputs);
    for(int i = 0; i < m->no_outputs; i++) {
        preds[i] = m->layers[m->no_hidden_layers+1].neurons[i].output;
    }
}

