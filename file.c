#include "file.h"
#include "debugmalloc.h"

#define NO_DIGITS 64

void save_net(model* m, char* weights_file, char* biases_file) {

    if(m == NULL) {
        perror("[ERR] model can't be NULL.");
        exit(0315);
    }

    if(biases_file == NULL || weights_file == NULL) {
        perror("[ERR] file paths can't be NULL.");
        exit(0111);
    }

    FILE* fp;
    fp = fopen(weights_file, "w+");

    if(fp == NULL) {
        perror("[ERR] can't open file.");
        exit(01221);
    }
    printf("[SAVE] Saving file...\n");

    for(int i = 0; i < m->no_hidden_layers+1; i++) {
        for(int j = 0; j < m->layers[i].no_neurons; j++) {
            for(int k = 0; k < m->layers[i+1].no_neurons; k++) {
                char buff[CHAR_MAX];
                if(j+k > 0) fprintf(fp, ",");
                gcvt(m->layers[i].neurons[j].weights[k], NO_DIGITS, buff);
                fprintf(fp, buff);
            }
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
    printf("[SAVE] Weights saved to %s\n", weights_file);

    //biases go to a different file
    fp = fopen(biases_file, "w+");
    if(fp == NULL) {
        perror("[ERR] can't open file.");
        exit(01221);
    }

    for(int i = 1; i < m->no_hidden_layers+2; i++) {
        for(int j = 0; j < m->layers[i].no_neurons; j++) {
            char buff[CHAR_MAX];
            gcvt(m->layers[i].neurons[j].bias, NO_DIGITS, buff);
            fprintf(fp, buff);
            fprintf(fp, ",");
        }
    }
    fclose(fp);
    printf("[SAVE] Biases saved to %s\n", biases_file);
}

void load_net(model* m, char* weights_file, char* biases_file) {

    if(m == NULL) {
        perror("[ERR] model can't be NULL.");
        exit(0315);
    }

    if(biases_file == NULL || weights_file == NULL) {
        perror("[ERR] file paths can't be NULL.");
        exit(0111);
    }

    printf("[LOAD] Loading model...\n");

    FILE* fp;
    fp = fopen(biases_file, "r");
    char buff[CHAR_MAX];

    //reading the biases
    fgets(buff, CHAR_MAX, fp);
    char *token;
    token = strtok(buff, ",");

    while(token != NULL) {
        for(int i = 1; i < m->no_hidden_layers+2; i++) {
            for(int j = 0; j < m->layers[i].no_neurons; j++) {
                m->layers[i].neurons[j].bias = atof(token);
                token = strtok(NULL, ",");
            }
        }
    }
    fclose(fp);
    printf("[LOAD] Biases loaded from%s\n", biases_file);

    //reading the weights
    fp = fopen(weights_file, "r");
    for(int i = 0; i < m->no_hidden_layers+1; i++) {
        fgets(buff, CHAR_MAX, fp);
        for(int j = 0; j < m->layers[i].no_neurons; j++) {
            for(int k = 0; k < m->layers[i+1].no_neurons; k++) {
                if(j+k == 0) token = strtok(buff, ",");
                else token = strtok(NULL, ",");
                m->layers[i].neurons[j].weights[k] = atof(token);
            }
        }
    }
    fclose(fp);
    printf("[LOAD] Weights loaded from%s\n", weights_file);
}

