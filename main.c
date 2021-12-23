#include <stdio.h>
#include "microsynapse.h"
#include "debugmalloc.h"

int main() {

    //modell létrehozása és inicializálása
    model* m;
    m = create_model(2, 1, 1, SIGMOID);
    add(m, 2, RELU);
    init(m);


    //adatok beolvasása és szegmentálása
    dataframe df = read_csv("C:\\infoc\\xor.csv", ",");
    int x_coords[] = {0, 1};
    int y_coords[] = {2};

    split x_split = train_test_split(df, 2, 1.0, x_coords);
    split y_split = train_test_split(df, 1, 1.0, y_coords);


    //háló tanítása
    double GL_ERR = train(m, x_split.train, y_split.train, 0.2, 50000);
    while(GL_ERR > 2) {
        fwrand(m);
        GL_ERR = train(m, x_split.train, y_split.train, 0.2, 50000);
        printf("[TRAINING] global error: %f\n", GL_ERR);
    }

    //teljesítmény kiírása
    printf("[TRAINING] Finished training with accumulated error: %f\n", GL_ERR);
    test(m, x_split.train, y_split.train);

    //háló kipróbálása tetszõleges inputtal
    double proba[] = {1.0, 0.0};
    double ertekek[1];
    pred(m, proba, ertekek);
    for(int i = 0; i < sizeof(ertekek)/sizeof(double); i++) {
        printf("predicted: %f\n", ertekek[i]);
    }

    //háló mentése
    save_net(m, "C:\\infoc\\xorws.csv", "C:\\infoc\\xorbiases.csv");

    //felszabadítunk mindent az égvilágon
    free_model(m);
    free_split(x_split);
    free_split(y_split);
    free_df(df);

    return 0;
}
