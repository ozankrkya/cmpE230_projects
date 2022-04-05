#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "functions.c"
#include <stdbool.h>




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
    char tokens[11] = {'[' , ']' , ',' , '(' , ')' , '=', ':', '{', '}', '*', '+'};
    int reserved[11];
    
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
            if(line[i] == '#'){
                break;
            }
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
        //printf("%s\n", newSentence);
        

        // asd sad f sad
        
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
        if(strcmp(splittedLines[0],"matrix")==0 ){
            if(strcmp(splittedLines[2],"[")==0 && strcmp(splittedLines[6],"]")==0&& isNumber(splittedLines[3])&& isNumber(splittedLines[5])){
                memcpy(varTypes[varNumber], splittedLines[0], 100);
                memcpy(varNames[varNumber], splittedLines[1], 100);
                memcpy(row_dimensions[varNumber], splittedLines[3], 100);
                memcpy(col_dimensions[varNumber], splittedLines[5], 100);
                varNumber++;
            }else{
                error(lineId);
                break;
            }

        }else if(strcmp(splittedLines[0],"vector")==0){
            if(strcmp(splittedLines[2],"[")==0 && strcmp(splittedLines[4],"]")==0 && isNumber(splittedLines[3])){
                memcpy(varTypes[varNumber], splittedLines[0], 100);
                memcpy(varNames[varNumber], splittedLines[1], 100);
                memcpy(row_dimensions[varNumber], splittedLines[3], 100);
                char col = '1';
                memcpy(col_dimensions[varNumber], &col, 100);
                varNumber++;
            }else{
                error(lineId);
                break;
            }

        }else if(strcmp(splittedLines[0],"scalar")==0){
            memcpy(varTypes[varNumber], splittedLines[0], 100);
            memcpy(varNames[varNumber], splittedLines[1], 100);
            row_dimensions[varNumber] = NULL;
            col_dimensions[varNumber] = NULL;          
            varNumber++;
        }else if(strcmp(splittedLines[1],"=") == 0){
            char *as_var_name = calloc(24, sizeof(char));
            memcpy(as_var_name, splittedLines[0],1);
            //looks if variable is declared
            
            if(isVariable(as_var_name,varNames)){
                int as_var_index = find(as_var_name, varNames);
                //assignment for matrices
                
                if(strcmp(varTypes[as_var_index],"matrix") == 0 || strcmp(varTypes[as_var_index],"vector") == 0){
                    int row_num_as_matrix = atoi(row_dimensions[as_var_index]);
                    int col_num_as_matrix = atoi(col_dimensions[as_var_index]);

                    // looks for matrix assignment which is done with curly braces
                    
                    if(strcmp(splittedLines[2],"{") == 0 && strcmp(splittedLines[3+(row_num_as_matrix*col_num_as_matrix)], "}") == 0){
                        int entry_number = row_num_as_matrix*col_num_as_matrix;
                        double entries[entry_number];
                        for(int i=0;i<row_num_as_matrix;i++){
                            for(int j=0;j<col_num_as_matrix;j++){
                                int entry_char_size = strlen(splittedLines[3+(i*col_num_as_matrix+j)]);
                                char *entry_value_in_char = calloc(entry_char_size,sizeof(double));
                                memcpy(entry_value_in_char,splittedLines[3+(i*col_num_as_matrix+j)],entry_char_size);
                                double entry_value = atof(entry_value_in_char);
                                entries[i*col_num_as_matrix+j] = entry_value;
                            }
                        }
                        for (int i=0;i<entry_number;i++){
                        
                        }
                        // at the end of above block, we have the double array (entries) to give as an input to matrix_assign function.
                        // now print "varName[as_var_index] = matrix_assigner(entries, row_num_as_matrix, col_num_as_matrix);\n" to the output file
                    }
                    else{
                        error(lineId);
                        //where are my curly braces or there are not right number of entries for variable matrix
                        break;
                    }
                    
                }             
            }
            else{
                error(lineId);
                //there is no such variable declared to assign
                break;
            }    
            
        }

        
        
        
        // empty the array
        //printf("%s \n",splittedLines[0]);
        memset(newSentence,0,512);

    }
    fclose(fp);
    
    

    //Assign statements



return(0);
}
