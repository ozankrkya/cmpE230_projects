#include <stdio.h>

//sqrt is already implemented in c standart library

//transpose scalar
double transpose_s(float s){
    return s;
};

// choose function
double choose(double expr1, double expr2, double expr3, double expr4){
    if(expr1 == 0){
        return expr2;
    }
    else if (expr1>0){
        return expr3;
    }
    else if (expr1<0){
        return expr4;
    }
}

//matrix multiplier -passed

double **matrix_mult(double **m1, double **m2, int m1_r, int m1_c, int m2_c){
    double **output_matrix = malloc(sizeof(double*)*m1_r);
    for(int i = 0; i< m2_c; i++){
        output_matrix[i] = malloc(sizeof(double)*m2_c);
    }
    double sop = 0 ;
    for (int i = 0; i<m1_r; i++){
        for (int j = 0; j<m2_c ; j++){
            for(int k = 0; k< m1_c; k++){
                sop += m1[i][k] * m2[k][j];
            }
            output_matrix[i][j] = sop;
            sop = 0;
        }
    }
    return output_matrix;
}

//matrix adder -passed

double **matrix_add(double **m1, double **m2, int m1_r, int m1_c){
    double **output_matrix = malloc(sizeof(double*)*m1_r);
    for(int i = 0; i< m1_c; i++){
        output_matrix[i] = malloc(sizeof(double)*m1_c);
    }
    for (int i = 0; i<m1_r ; i++){
        for (int j = 0 ; j< m1_c ; j++){
            output_matrix[i][j] = m1[i][j] + m2[i][j];
        }
    } 
    return output_matrix;
}
//matrix subtructor -passed

double **matrix_sub(double **m1, double **m2, int m1_r, int m1_c){
    double **output_matrix = malloc(sizeof(double*)*m1_r);
    for(int i = 0; i< m1_c; i++){
        output_matrix[i] = malloc(sizeof(double)*m1_c);
    }
    for (int i = 0; i<m1_r ; i++){
        for (int j = 0 ; j< m1_c ; j++){
            output_matrix[i][j] = m1[i][j] - m2[i][j];
        }
    } 
    return output_matrix;
}

// matrix transposer -passed

double **matrix_transpose(double **matrix, int matrix_r, int matrix_c){
    double **output_matrix = malloc(sizeof(double*)*matrix_r);
    for(int i = 0; i< matrix_c; i++){
        output_matrix[i] = malloc(sizeof(double)*matrix_c);
    }
    for(int i = 0; i< matrix_r; i++){
        for (int j = 0; j<matrix_c; j++){
            output_matrix[j][i] = matrix[i][j]; 
        }
    }
    return output_matrix;
}

// Print statements 

// printsep
void printsep(){
    printf("----------");
}

// matrix printer / i think there is no need for implementing a printer for scalars
double epsilon = 1/(1048576);
#include <math.h>
matrix_printer(double **print_matrix, int row_num, int col_num){
    for (int i=0 ; i++ ; i<row_num){
        for (int j=0 ; j++ ; j<col_num){
            double value = print_matrix[i][j];
            double floor_val = floor(value);
            if (floor_val > value){
                if ((floor_val - value) > epsilon){
                    printf("%f\n",value);
                }
                else {
                    printf("%d\n",(int) floor_val);
                }
            }
            else{
                if((value-floor_val) > epsilon){
                    printf("%f\n",value);
                }
                else{
                    printf("%d\n", (int) floor_val);
                }
            }    
        }   
    }
}

double **matrix_assigner(int row_number, int column_number){
    double **output_matrix = calloc(row_number,sizeof(double*));
    for(int i = 0; i< column_number; i++){
        output_matrix[i] = calloc(column_number,sizeof(double));
    } 
    return output_matrix;
}



