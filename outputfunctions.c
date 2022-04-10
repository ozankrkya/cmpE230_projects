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

//matrix multiplier 

double **matrix_mult(double **m1, double **m2, int m1_r, int m1_c, int m2_c){
    //initializes the output array 
    double **output_matrix = malloc(sizeof(double*)*m1_r);
    for(int i = 0; i< m2_c; i++){
        output_matrix[i] = malloc(sizeof(double)*m2_c);
    }
    //sum of products for output marix's entries
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

//matrix adder 

double **matrix_add(double **m1, double **m2, int m1_r, int m1_c){
    //initializes the output array
    double **output_matrix = malloc(sizeof(double*)*m1_r);
    for(int i = 0; i< m1_c; i++){
        output_matrix[i] = malloc(sizeof(double)*m1_c);
    }
    // adds the entries with one to one correspondence
    for (int i = 0; i<m1_r ; i++){
        for (int j = 0 ; j< m1_c ; j++){
            output_matrix[i][j] = m1[i][j] + m2[i][j];
        }
    } 
    return output_matrix;
}
//matrix subtructor 
double **matrix_sub(double **m1, double **m2, int m1_r, int m1_c){
    //initializes the output array
    double **output_matrix = malloc(sizeof(double*)*m1_r);
    for(int i = 0; i< m1_c; i++){
        output_matrix[i] = malloc(sizeof(double)*m1_c);
    }
    // subtracts the entries with one to one correspondence
    for (int i = 0; i<m1_r ; i++){
        for (int j = 0 ; j< m1_c ; j++){
            output_matrix[i][j] = m1[i][j] - m2[i][j];
        }
    } 
    return output_matrix;
}

// matrix transposer 

double **matrix_transpose(double **matrix, int matrix_r, int matrix_c){
    //initializes the output array
    double **output_matrix = malloc(sizeof(double*)*matrix_r);
    for(int i = 0; i< matrix_c; i++){
        output_matrix[i] = malloc(sizeof(double)*matrix_c);
    }
    // defines the output matrix's [j][i] as input matrix's [i][j]
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


void matrix_printer(double **print_matrix, int row_num, int col_num){
    //defines an epsilon for comparising if number is integer or float
    double epsilon = 1/(1048576);
    for (int i=0 ; i<row_num; i++){
        for (int j=0 ; j<col_num ; j++){
            double value = print_matrix[i][j];
            int floor_val = (int)(value);
            if (value-floor_val>0){
                printf("%f\n", value);
            }else{
                printf("%d\n", floor_val);
            }
        }    
        printf("\n" );
    }   
}
// scalar matrix multiplyer

double ** matrix_scalar(double **m1,int num, int m1_r, int m1_c){
        double **output_matrix = malloc(sizeof(double*)*m1_r);
        for(int i = 0; i< m1_r; i++){
            output_matrix[i] = malloc(sizeof(double)*m1_c);
        }
        double sop = 0 ;
        for (int i = 0; i<m1_r; i++){
            for (int j = 0; j<m1_c ; j++){
                sop += m1[i][j] * num;
                output_matrix[i][j] = sop;
                sop = 0;
            }
        }
        return output_matrix;

    }