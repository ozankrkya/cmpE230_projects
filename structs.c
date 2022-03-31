#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// struct for matrices
// data type is 1 for vectors and matrices
// all values are stored as floats in 2D array.
// row number and column number are stored in two distinct variables.

struct matrix{
    char *name[24];
    int data_type;
    int row_number;
    int column_number;
    float matrix[];
};

// struct for scalars
// data type is 0 for scalars when initializing

struct scalar {
    char *name[24];
    int data_type;
    float actual_value;
    int int_value;
};
