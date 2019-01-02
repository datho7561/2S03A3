#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "assignment3.h"

SLnodePtr flatten(const HBnodePtr L);

int main() {

    srand(time(NULL));

    HBnodePtr myList = createHBlist(5, 3);

    printf("\nGENERATED LIST:\n\n");

    printHBlist(myList);

    printf("\n... generating flattened list ...\n");

    SLnodePtr myFlatList = flattenList(myList);

    printf("\nFLATTENED LIST:\n\n");    

    printSLlist(myFlatList);

    freeSLlist(myFlatList);

    freeHBlist(myList);

    return 0;

}