#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "functions.c"
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


int main (int argc,char *argv[]) {

    FILE *fp;
    char line[256];
    // array to store splitted line
    char *splittedLines[256];
    for (int i = 0; i < 256; i++) splittedLines[i] = calloc(256, sizeof(char*));
    char *varNames[256];
    for (int i = 0; i < 256; i++) varNames[i] = calloc(256, sizeof(char));
    char *varTypes[256];
    for (int i = 0; i < 256; i++) varTypes[i] = calloc(256, sizeof(char));
    char *row_dimensions[256];
    for (int i = 0; i < 256; i++) row_dimensions[i] = calloc(256, sizeof(char*));
    char *col_dimensions[256];
     for (int i = 0; i < 256; i++) col_dimensions[i] = calloc(256, sizeof(char*));
    int varNumber = 0;
    /* Open file for reading Filename is given on the command line */

    if (argc != 2) {
        printf("Give filename as command line argument\n") ;
        return(1);
    }

    fp = fopen(argv[1], "r");
    // detects file cannot open error
    if(fp == NULL) {
        printf("Cannot open %s\n",argv[1]);
        return(1);
    }
    

    //reserved tokens
    char tokens[7] = {'[' , ']' , ',' , '(' , ')' , '=', ':'};
    int reserved[7];
    
    for(int i = 0; i < sizeof(tokens)/sizeof(char); i++){
        int temp = (int) tokens[i];
        reserved[i] = temp;
    }
    char space = ' ';

    int lineId = 0;

    //reads input from file
    while( fgets(line,256,fp) != NULL ) {
        int tokenId = 0;
        int j = 0;
        char newSentence[512];
        int lineLength = strlen(&line[0]);
        
        for(int i = 0; i < strlen(&line[0]); i++){
            //checks tokens if its reserved or alphanumeric
            if(isReserved(line[i], reserved)) {
                newSentence[j] = space;
                j++;
                newSentence[j] = line[i];
                j++;
                newSentence[j] = space;
                j++;
            }else if (isalnum(line[i]) || (int) line[i] == (int)space){
                newSentence[j] = line[i];
                j++;
            }
        }
        
        
        // splits the tokens that includes space between them
        char *token = strtok(newSentence, " ");
        while( token != NULL ) {
            //stores each token in array
            memcpy(splittedLines[tokenId], token, 256); 
            tokenId += 1;
            token = strtok(NULL, " ");
        }
        lineId += 1;
        //checks 0 index and choose what to do
        if(strcmp(splittedLines[0],"matrix")==0){
            memcpy(varTypes[varNumber], splittedLines[0], 100);
            memcpy(varNames[varNumber], splittedLines[1], 100);
            memcpy(row_dimensions[varNumber], splittedLines[3], 100);
            memcpy(col_dimensions[varNumber], splittedLines[5], 100);
            varNumber++;


        }else if(strcmp(splittedLines[0],"vector")==0){
            memcpy(varTypes[varNumber], splittedLines[0], 100);
            memcpy(varNames[varNumber], splittedLines[1], 100);
            memcpy(row_dimensions[varNumber], splittedLines[3], 100);
            char col = '1';
            memcpy(col_dimensions[varNumber], &col, 100);
            varNumber++;

        }else if(strcmp(splittedLines[0],"scalar")==0){
            memcpy(varTypes[varNumber], splittedLines[0], 100);
            memcpy(varNames[varNumber], splittedLines[1], 100);
            row_dimensions[varNumber] = NULL;
            col_dimensions[varNumber] = NULL;          
            varNumber++;
        }
        
         
        // empty the array
        //printf("%s \n",splittedLines[0]);
        memset(newSentence,0,512);
        
    }
    fclose(fp);
    
        struct matrix matrixArray[256];
    struct scalar scalarArray[256];
    int scalarVarCount = 0;
    int matrixVarCount = 0;

    for (int i=0;i<256;i++){
        if(strcmp(varTypes[i],"scalar")==0){
            struct scalar s;
            s.name[0] = varNames[i];
            s.data_type = 0;
            scalarArray[scalarVarCount] = s;
            scalarVarCount += 1;
        }
        else if(strcmp(varTypes[i],"vector")==0){
            struct matrix v;
            v.name[0] = varNames[i];
            v.data_type = 1;
            v.row_number = atoi(row_dimensions[i]);
            v.column_number = atoi(col_dimensions[i]);
            matrixArray[matrixVarCount] = v;
            matrixVarCount += 1;
        }
        else if (strcmp(varTypes[i],"matrix")==0){
            struct matrix m;
            m.name[0] = varNames[i];
            m.data_type = 1;
            m.row_number = atoi(row_dimensions[i]);
            m.column_number = atoi(col_dimensions[i]);
            matrixArray[matrixVarCount] = m;
            matrixVarCount += 1;
        }
    }

    printf("%d\n",matrixArray[2].row_number);

return(0);

}
