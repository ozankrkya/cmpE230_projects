#include <stdio.h>
#include <stdlib.h>
int main (int argc,char *argv[]) {

    double **B = malloc(sizeof(double*)*6 );for(int i = 0; i < 6 ; i++){B[i] = malloc(sizeof(double)*6);}
    B[0][0]=1;B[0][1]=0;B[1][0]=0;B[1][1]=1;B[2][0]=2;B[2][1]=7;
    
    

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

    double ** output_matrix2 = malloc(sizeof(double*)*6);
    for(int i = 0; i< 6; i++){
        output_matrix2[i] = malloc(sizeof(double)*6);
    }
    output_matrix2  = matrix_scalar(B,4,3,2);


    printf("%f, %f ", output_matrix2[0][0],output_matrix2[2][1]);


}