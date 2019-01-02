#include "assignment3.h"

#include <stdlib.h>
#include <stdio.h>


// FUNCTIONS THAT I DEFINED //


/**
 * Used as the comparison function for quicksort. Pretty much copied from:
 * https://www.tutorialspoint.com/c_standard_library/c_function_qsort.htm
 * The reason behind me copying was because I wasn't sure how to implement
 * a function that operates on pointers of type void. 
 */
static int compareInts(const void *a, const void *b) {

    return *((int *)a) - *((int *)b);

}


/**
 * Create a SL list given:
 * length   : the length of the list
 * val_list : the values to use
 * 
 * Returns a pointer to the first node of the list
*/
static SLnodePtr generateSLlist(int length, int *val_list) {

    // To make a list of length 0, point to NULL
    if (length == 0) return NULL;

    // Otherwise, make the node
    SLnodePtr current = malloc(sizeof(struct SLnode));

    // Take the assigned number from the array as the key
    current->key = *val_list;

    // Point it to the next element in the SL list
    current->next = generateSLlist(length - 1, val_list + 1);

    // Return the pointer to this node
    return current;

}


/**
 * Generates an HB list given:
 * n : the length of the HB list
 * m : the max value of the length of the bottom lists
 * a : an array of the keys for the HB nodes
 * 
 * Returns a pointer to the first value in the list
*/
static HBnodePtr generateHBlist(int n, int m, int a[]) {

    // If this list is supposed to be zero long, point to NULL
    //  If the previous value was the last node, point
    //  that node to NULL
    if (n == 0) return NULL;

    // Otherwise, make an HB node and get a pointer to it
    HBnodePtr current = malloc(sizeof(struct HBnode));

    // Set the key to the given value
    current->key = *a;

    // Generate a random number in range [0, m] for the length of the SL list
    // Make an array for the keys
    // Declare counter for iterating through the array
    int slLength = (int)((m + 1) * ((double)rand()/RAND_MAX)), val_arr[slLength], i;
    
    // Populate the array with values in [*a, RAND_MAX)
    for (i = 0; i < slLength; i++)
        val_arr[i] = (int)((RAND_MAX - *a) * ((double)rand())/RAND_MAX) + *a;

    // EXPLANATION FOR ABOVE:
    // ((double)rand())/RAND_MAX) --> Generate a random double on [0,1)
    // * (500 - *a)               --> Scale from somewhere between [0,0] or [0,500),
    //                                depending on how big *a (the start of the list) is
    //                                The scaling is done this way so that no matter what,
    //                                the number is <= RAND_MAX.
    //                                This is to avoid integer overflow.
    // + *a                       --> Add the value of the start of the list so that
    //                                every value in the array is at least
    //                                the value of the start of the array

    // Sort it
    qsort(val_arr, slLength, sizeof(int), compareInts);

    // Make an SL list for `current`'s bottom to point at
    //  Pass the created and sorted array of keys
    current->bottom = generateSLlist(slLength, val_arr);

    // Called recursively to generate the next element in the list
    current->next = generateHBlist(n - 1, m, a + 1);

    // Return the pointer to this node
    return current;

}




// FUNCTIONS PART OF SPECIFICATION //


HBnodePtr createHBlist(int n, int m) {

    // Make an array of keys for the HB nodes
    int val_arr[n], i;
    
    // The values generated are on the full range of rand()
    for (i = 0; i < n; i++)
        val_arr[i] = rand();

    // Sort the array so that the keys are in increasing order
    qsort(val_arr, n, sizeof(int), compareInts);

    // Pass the array to generateHBlist, which recursively builds the list
    return generateHBlist(n, m, val_arr);

}


void freeSLlist(SLnodePtr L) {

    // If the pointer points to NULL, we have reached the end
    //  and everything should be freed
    if (L == NULL) return;

    // Free the rest of the SL list
    freeSLlist(L->next);

    // Free the current node
    free(L);

}


void freeHBlist(HBnodePtr L) {

    // If the pointer points to NULL, we have reached the end
    //  and everything should be freed
    if (L == NULL) return;

    // Otherwise, free the attached SL list
    freeSLlist(L->bottom);

    // Free the rest of the HB list
    freeHBlist(L->next);

    // Free the current node
    free(L);

}


void printSLlist(const SLnodePtr L) {

    // If the list is empty, or the entire list has been printed,
    //  print a newline character and return
    if (L == NULL) {

        printf("\n");

    } else {

        // Print the key of the node L is pointing to, followed by two spaces
        printf("%d  ", L->key);

        // Print the next element in the list
        printSLlist(L->next);

    }    

}


void printHBlist(const HBnodePtr L) {

    // If the list is empty there is no need to print anything
    if (L == NULL) return;

    // Otherwise, print the HB node's key
    printf("%d ->  ", L->key);

    // Print the HB node's list
    printSLlist(L->bottom);

    // Print the rest of the list
    printHBlist(L->next);

}


/**
 * Merges two given SLnodePtrs, a and b
*/
static SLnodePtr merge(SLnodePtr a, SLnodePtr b) {

    // If the first list is empty, point to the second, and vice versa
    if (a == NULL) return b;
    if (b == NULL) return a;

    // If the first value in b is smaller than the first value in a
    if (a->key > b->key) {

        //
        SLnodePtr rest = merge(a, b->next);
        b->next = rest;
        return b;
    }

    // Otherwise do the same thing but with the a list
    SLnodePtr rest = merge(a->next, b);
    a->next = rest;
    return a;

}


SLnodePtr flattenList(const HBnodePtr L) {

    // To flatten an empty HB list, return an empty SL list
    if (L == NULL) return NULL;

    // Make pointers used in copying the HB\s bottom list,
    //  as well as a node to store the HB node's key
    SLnodePtr orig, copy, first = malloc(sizeof(struct SLnode));

    // Copy the HB node's key into the SL node
    first -> key = L -> key;

    // Make a pointer to the bottom list to iterate over it
    orig = L->bottom;

    // `copy` points to the end of the copy of the HB's bottom list
    copy = first;

    // Make a copy of the bottom list, stored under first

    // While there are still elements to copy in HB's bottom
    while (orig != NULL) {

        // Make a new node
        copy->next = malloc(sizeof(struct SLnode));

        // Move the end of the list to that node
        copy = copy->next;

        // Copy the key into that node
        copy->key = orig->key;

        // Move down in HB's bottom
        orig = orig->next;

    }

    // Null-terminate the copied list
    copy->next = NULL;

    // Merge this list with the rest of the list flattened
    return merge(first, flattenList(L->next));

}
