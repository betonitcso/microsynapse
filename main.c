#include <stdio.h>
#include "debugmalloc.h"
#include "net.h"

int main() {
    model* m;
    m = create_model(2, 2, 3, 0);

    add(m, 3, "sigmoid");
    add(m, 2, "relu");
    add(m, 2, "sigmoid");

    init(m);
    return 0;
}
