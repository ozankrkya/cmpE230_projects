#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "expressions2.c"

#define constant 256




int main (int argc,char *argv[]) {

    FILE *fp;
    FILE *out;
    FILE *outfunc;
    char line[256];
    // array to store splitted line
    char *splittedLines[256];
    for (int i = 0; i < 256; i++) splittedLines[i] = calloc(256, sizeof(char*));
    char *varNames[512];
    for (int i = 0; i < 512; i++) varNames[i] = calloc(256, sizeof(char));
    char *varTypes[512];
    for (int i = 0; i < 512; i++) varTypes[i] = calloc(256, sizeof(char));
    char *row_dimensions[512];
    for (int i = 0; i < 512; i++) row_dimensions[i] = calloc(256, sizeof(char*));
    char *col_dimensions[512];
    for (int i = 0; i < 512; i++) col_dimensions[i] = calloc(256, sizeof(char*));
    char *resultArr[256];
    for (int i = 0; i < 256; i++) resultArr[i] = calloc(256, sizeof(char*));
    char *vectorArr[256];
    for (int i = 0; i < 256; i++) vectorArr[i] = calloc(256, sizeof(char*));
    int varNumber = 0;
    int vectorNum = 0;
    bool * err = false;
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
    char tokens[12] = {'[' , ']' , ',' , '(' , ')' , '=', ':', '{', '}', '*', '+', '-'};
    int reserved[12];
    
    for(int i = 0; i < 12; i++){
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
        char printline[256];
        printline[0] = '\0';
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
        
        // splits the tokens that includes space between them
        char *token = strtok(newSentence, " ");
        while( token != NULL ) {
            //stores each token in array
            memcpy(splittedLines[tokenId], token, 256); 
            tokenId += 1;
            token = strtok(NULL, " ");
        }
        memcpy(splittedLines[tokenId],"exit",256);
        lineId += 1;
        
        //checks 0 index and choose what to do
        if(strcmp(splittedLines[0],"matrix")==0 ){
            //matrix declaration
            if(strcmp(splittedLines[2],"[")==0 && strcmp(splittedLines[6],"]")==0&& isInteger(splittedLines[3])&& isInteger(splittedLines[5])&&\
             strcmp(splittedLines[4],",")==0){
                memcpy(varTypes[varNumber], splittedLines[0], 100);
                if(isValidName(splittedLines[1])){
                    memcpy(varNames[varNumber], splittedLines[1], 100);
                    memcpy(row_dimensions[varNumber], splittedLines[3], 100);
                    memcpy(col_dimensions[varNumber], splittedLines[5], 100);
                }else{
                    error(lineId);
                    *err = true;
                    break;
                }
                //print to c file
                strcat(printline, "double **") ; 
                strcat(printline, varNames[varNumber]) ;
                strcat(printline, " = malloc(sizeof(double*)*") ;
                int entryNum = atoi(row_dimensions[varNumber])*atoi(col_dimensions[varNumber]);
                char entryNumber[50];
                sprintf(entryNumber, "%d", entryNum);
                strcat(printline, entryNumber) ;
                strcat(printline, " );for(int i = 0; i < ") ;
                strcat(printline, entryNumber) ;
                strcat(printline, " ; i++){") ;
                strcat(printline, varNames[varNumber]) ;
                strcat(printline, "[i] = malloc(sizeof(double)*") ;
                strcat(printline, entryNumber) ;
                strcat(printline, ");}") ;
                int len = strlen(printline);
                printline[len] = '\0';
                memcpy(resultArr[lineId], printline, 256);
                
                varNumber++;
            }else{
                error(lineId);
                *err = true;
                break;
            }
        //statements for declarations
        }else if(strcmp(splittedLines[0],"vector")==0){
            //if first token is equal to vector
            memcpy(vectorArr[vectorNum],splittedLines[1],24);
            vectorNum++;
            if(strcmp(splittedLines[2],"[")==0 && strcmp(splittedLines[4],"]")==0 && isInteger(splittedLines[3])){
                memcpy(varTypes[varNumber], "matrix", 100);
                if(isValidName(splittedLines[1])){
                    memcpy(varNames[varNumber], splittedLines[1], 100);
                    memcpy(row_dimensions[varNumber], splittedLines[3], 100);
                }else{
                    error(lineId);
                    *err = true;
                    break;
                }
                char col = '1';
                memcpy(col_dimensions[varNumber], &col, 100);

                // printing to c file
                strcat(printline, "double **") ; 
                strcat(printline, varNames[varNumber]) ;
                strcat(printline, " = malloc(sizeof(double*)*") ;
                strcat(printline, row_dimensions[varNumber]) ;
                strcat(printline, " );for(int i = 0; i < ") ;
                strcat(printline, row_dimensions[varNumber]) ;
                strcat(printline, " ; i++){") ;
                strcat(printline, varNames[varNumber]) ;
                strcat(printline, "[i] = malloc(sizeof(double)*") ;
                strcat(printline, row_dimensions[varNumber]) ;
                strcat(printline, ");}") ;
                int len = strlen(printline);
                printline[len] = '\0';
                memcpy(resultArr[lineId], printline, 256);
                varNumber++;
            }else{
                error(lineId);
                *err = true;
                break;
            }

        }else if(strcmp(splittedLines[0],"scalar")==0){
            //scalar declaration
            memcpy(varTypes[varNumber], splittedLines[0], 100);
            //checks if varname is valid
            if(isValidName(splittedLines[1])){
                memcpy(varNames[varNumber], splittedLines[1], 100);
            }else{
                error(lineId);
                *err = true;
                break;
            }
            row_dimensions[varNumber] = NULL;
            col_dimensions[varNumber] = NULL;
            // printing to c file 
            strcat(printline, "double ") ;    
            strcat(printline, splittedLines[1]) ;
            strcat(printline, "; ") ;  
            int len = strlen(printline);
            printline[len] = '\0';
            memcpy(resultArr[lineId], printline, 100);
            varNumber++;
        //Statements for assignment statements
        }else if(strcmp(splittedLines[1],"=") == 0){
            char *as_var_name = calloc(24, sizeof(char));
            memcpy(as_var_name, splittedLines[0],8);
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
                        for(int i=0;i<row_num_as_matrix;i++){
                            for(int j=0;j<col_num_as_matrix;j++){
                                int entry_char_size = strlen(splittedLines[3+(i*col_num_as_matrix+j)]);
                                char *entry_value_in_char = calloc(entry_char_size,sizeof(double));
                                memcpy(entry_value_in_char,splittedLines[3+(i*col_num_as_matrix+j)],entry_char_size);
                                //print to c file
                                char str_i[50];
                                char str_j[50];
                                sprintf(str_i, "%d", i);
                                sprintf(str_j, "%d", j);
                                strcat(printline, as_var_name) ;
                                strcat(printline, "[") ;
                                strcat(printline, str_i) ;
                                strcat(printline, "]") ;
                                strcat(printline, "[") ;
                                strcat(printline, str_j) ;
                                strcat(printline, "]") ;
                                strcat(printline, "=") ;
                                strcat(printline, entry_value_in_char) ;
                                strcat(printline, ";") ;
                                int len = strlen(printline);
                                printline[len] = '\0';
                            }
                        }
                        memcpy(resultArr[lineId], printline, 100);
                    }else if(true){

                        //Matrix can assigned by expression assign matrix here

                        char *expr1[256];
                        for (int i = 0; i < 256; i++) expr1[i] = calloc(256, sizeof(char*));
                        for(int i = 2; i <256; i++){
                            if (strcmp(splittedLines[i], "exit")==0){
                                memcpy(expr1[i-2], splittedLines[i],10);
                                break;
                            }
                            expr1[i-2] = splittedLines[i];
                        }
                        char result[256];
                        char *result2[256];
                        for (int i = 0; i < 256; i++) result2[i] = calloc(256, sizeof(char*));
                        int tmpId = 0;
                        expr( result , expr1, varNames,lineId,err);
                        
                        char *tmp = strtok(result, " ");
                        while( tmp != NULL ) {
                            //stores each token in array
                            memcpy(result2[tmpId], tmp, 256); 
                            tmpId += 1;
                            tmp = strtok(NULL, " ");
                        }
                        memcpy(result2[tmpId], "exit", 10); 
                        char result3[256];
                        result3[0] = '\0';
                        int* pvarNum;
                        pvarNum = &varNumber;
                        
                        postFixToInfix(result2, result3, varNames,varTypes, lineId, pvarNum, col_dimensions, row_dimensions, vectorArr);
                        int index = find(result3,varNames);
                        if(strcmp(varTypes[index],"matrix") != 0 ){
                            error(lineId);
                            *err = true;
                            break;
                        }
                        //print to c file
                        strcat(printline, as_var_name) ;
                        strcat(printline, "=") ;
                        strcat(printline, result3);
                        strcat(printline, ";");
                        int len = strlen(printline);
                        printline[len] = '\0';
                        memcpy(resultArr[lineId], printline, 100);
                        memset(result3,0,256);
                    }
                    else{                       
                        error(lineId);
                        *err = true;
                        //where are my curly braces or there are not right number of entries for variable matrix
                        //break;
                    }
                //assignments of scalar type variables    
                }else if(strcmp(varTypes[as_var_index],"scalar") == 0 ){
                    
                    char *expr1[256];
                    for (int i = 0; i < 256; i++) expr1[i] = calloc(256, sizeof(char*));
                    for(int i = 2; i <256; i++){
                        if (strcmp(splittedLines[i], "exit")==0){
                            memcpy(expr1[i-2], splittedLines[i],10);
                            break;
                        }
                        expr1[i-2] = splittedLines[i];
                    }
                    char result[256];
                    char *result2[256];
                    for (int i = 0; i < 256; i++) result2[i] = calloc(256, sizeof(char*));
                    int tmpId = 0;
                    expr( result , expr1, varNames, lineId,err);
                    
                    
                    char *tmp = strtok(result, " ");
                    while( tmp != NULL ) {
                        //stores each token in array
                        memcpy(result2[tmpId], tmp, 256); 
                        tmpId += 1;
                        tmp = strtok(NULL, " ");
                    }
                    memcpy(result2[tmpId], "exit", 10); 
                    char result3[256];
                    result3[0] = '\0';
                    int* pvarNum;
                    pvarNum = &varNumber;
                    //returns the expression the left
                    postFixToInfix(result2, result3, varNames,varTypes, lineId, pvarNum, col_dimensions, row_dimensions, vectorArr);
                    int index = find(result3,varNames);
                    if(index != -1){
                        if(strcmp(varTypes[index],"scalar") != 0 && !isNumber(result3)){
                            error(lineId);
                            *err = true;
                            break;
                        }
                    }
                    //print to c file
                    strcat(printline, as_var_name) ;
                    strcat(printline, "=") ;
                    strcat(printline, result3);
                    strcat(printline, ";");
                    int len = strlen(printline);
                    
                    printline[len] = '\0';
                    
                    memcpy(resultArr[lineId], printline, 100);
                    memset(result3,0,256);
                }            
            }
            else{
                error(lineId);
                *err = true;
                //there is no such variable declared to assign
                break;
            }    
        // statements of for loops if first token is equal to for    
        }else if(strcmp(splittedLines[0],"for") == 0){
            
            //single for loop with one loop variable
            if(strcmp(splittedLines[3],"in") == 0 && strcmp(splittedLines[1],"(") == 0){
                varNames[varNumber] = splittedLines[2];
                varNumber++;
                //for loop expressions
                char *expr1[256];
                for (int i = 0; i < 256; i++) expr1[i] = calloc(256, sizeof(char*));
                char *expr2[256];
                for (int i = 0; i < 256; i++) expr2[i] = calloc(256, sizeof(char*));
                char *expr3[256];
                for (int i = 0; i < 256; i++) expr3[i] = calloc(256, sizeof(char*));
                int cur = 4;
                int temp = cur;
                char postexpr1[256];
                char postexpr2[256];
                char postexpr3[256];
                postexpr1[0] = '\0';
                postexpr2[0] = '\0';
                postexpr3[0] = '\0';
                
                for (int i = 4; i < 256; i++){
                    cur++;
                    if(strcmp(splittedLines[i], ":")==0){
                        break;
                    }
                    memcpy(expr1[i-4],splittedLines[i],10);
                    
                }
                memcpy(expr1[cur-temp-1], "exit", 10); 
                temp = cur;
                for (int i = cur; i < 256; i++){
                    cur++;
                    if(strcmp(splittedLines[i], ":")==0){
                        break;
                    }
                    memcpy(expr2[i-temp],splittedLines[i],10);
                }
                memcpy(expr2[cur-temp-1], "exit", 10); 
                temp = cur;
                for (int i = cur; i < 256; i++){
                    cur++;
                    if(strcmp(splittedLines[i], ")")==0){
                        break;
                    }
                    memcpy(expr3[i-temp],splittedLines[i],10);
                }
                memcpy(expr3[cur-temp-1], "exit", 10); 
                if(!strcmp(splittedLines[cur-1], ")")==0){
                    error(lineId);
                    *err = true;
                    break;
                }

                
                char * arr[4] = {"22", "+", "3", "exit"};
                //converts expressions into the postfix notation
                expr( postexpr1 , expr1, varNames, lineId,err);
                expr( postexpr2 , expr2, varNames, lineId,err);
                expr( postexpr3 , expr3, varNames, lineId,err);
                
                
                int* pvarNum;
                pvarNum = &varNumber;
                char *postArr1[256];
                for (int i = 0; i < 256; i++) postArr1[i] = calloc(256, sizeof(char*));
                char *postArr2[256];
                for (int i = 0; i < 256; i++) postArr2[i] = calloc(256, sizeof(char*));
                char *postArr3[256];
                for (int i = 0; i < 256; i++) postArr3[i] = calloc(256, sizeof(char*));

                char *tmp = strtok(postexpr1, " ");
                
                int tmpId = 0;
                while( tmp != NULL ) {
                    //stores each token in array
                    memcpy(postArr1[tmpId], tmp, 256); 
                    tmpId += 1;
                    tmp = strtok(NULL, " ");
                }
                memcpy(postArr1[tmpId], "exit", 100); 
                tmpId = 0;
                char *tmp2 = strtok(postexpr2, " ");
                while( tmp2 != NULL ) {
                    //stores each token in array
                    memcpy(postArr2[tmpId], tmp2, 256); 
                    tmpId += 1;
                    tmp2 = strtok(NULL, " ");
                }
                memcpy(postArr2[tmpId], "exit", 100);
                tmpId = 0; 
                char *tmp3 = strtok(postexpr3, " ");
                while( tmp3 != NULL ) {
                    //stores each token in array
                    memcpy(postArr3[tmpId], tmp3, 256); 
                    tmpId += 1;
                    tmp3 = strtok(NULL, " ");
                }
                memcpy(postArr3[tmpId], "exit", 100); 
                tmpId = 0;
                char res1[256];
                res1[0] = '\0';
                char res2[256];
                res2[0] = '\0';
                char res3[256];
                res3[0] = '\0';
                
                // convertion of postfix forms into the infix form for assignments in the file.c 
                postFixToInfix(postArr1, res1, varNames,varTypes, lineId, pvarNum, col_dimensions, row_dimensions, vectorArr);
                
                postFixToInfix(postArr2, res2, varNames,varTypes, lineId, pvarNum, col_dimensions, row_dimensions, vectorArr);
                
                postFixToInfix(postArr3, res3, varNames,varTypes, lineId, pvarNum, col_dimensions, row_dimensions, vectorArr);

                //print to c file
                strcat(printline, "for( int ");
                strcat(printline, splittedLines[2]);
                strcat(printline, " = ");
                strcat(printline, res1);
                strcat(printline, ";");
                strcat(printline, splittedLines[2]);
                strcat(printline, " < ");
                strcat(printline, res2);
                strcat(printline, ";");
                strcat(printline, splittedLines[2]);
                strcat(printline, " = ");
                strcat(printline, splittedLines[2]);
                strcat(printline, "+");
                strcat(printline, res3);
                strcat(printline, ") {");
                int len = strlen(printline);
                printline[len] = '\0';
                memcpy(resultArr[lineId], printline, 100);
            //nested for
            }else if(strcmp(splittedLines[3],",") == 0 && strcmp(splittedLines[1],"(") == 0 && strcmp(splittedLines[5],"in") == 0){
                char *expr1[256];
                for (int i = 0; i < 256; i++) expr1[i] = calloc(256, sizeof(char*));
                char *expr2[256];
                for (int i = 0; i < 256; i++) expr2[i] = calloc(256, sizeof(char*));
                char *expr3[256];
                for (int i = 0; i < 256; i++) expr3[i] = calloc(256, sizeof(char*));
                char *expr4[256];
                for (int i = 0; i < 256; i++) expr1[i] = calloc(256, sizeof(char*));
                char *expr5[256];
                for (int i = 0; i < 256; i++) expr2[i] = calloc(256, sizeof(char*));
                char *expr6[256];
                for (int i = 0; i < 256; i++) expr3[i] = calloc(256, sizeof(char*));
                int cur = 6;
               
            }else{
                error(lineId);
                *err = true;
            }
        //change matrix index (cannot be finished)
        /*
        }else if(strcmp(splittedLines[1],"[") == 0){
            
            printf("%s \n", splittedLines[0]);
            char *expr1[256];
            for (int i = 0; i < 256; i++) expr1[i] = calloc(256, sizeof(char*));
            char *expr2[256];
            for (int i = 0; i < 256; i++) expr2[i] = calloc(256, sizeof(char*));
            char *as_var_name = calloc(24, sizeof(char));
            int cur  = 2;
            
            memcpy(as_var_name, splittedLines[0],8);
            int as_var_index = find(as_var_name, varNames);
            if(strcmp(varTypes[as_var_index],"matrix") == 0 && !isVector(splittedLines[0],varTypes)){
                for (int i = cur; i < 256; i++){
                    cur++;
                    if(strcmp(splittedLines[i], ",")==0 ){
                        break;
                    }
                    expr1[i-2] = splittedLines[i];
                }
                int temp = cur;
                
                if(strcmp(splittedLines[temp-1],",") == 0){
                    printf("bu %s", varTypes[as_var_index]);
                    for (int i = cur; i < 256; i++){
                        cur++;
                        if(strcmp(splittedLines[i], "]")==0){
                            break;
                        }
                        expr2[i-temp] = splittedLines[i];
                    }
                }else{
                    error(lineId);
                    break;
                }

            }else if (isVector(splittedLines[0],varTypes)){
                bool check = true;
                for (int i = cur; i < 256; i++){
                    cur++;
                    if(strcmp(splittedLines[i], "]")==0 ){
                        check = false;
                        break;
                    }
                    expr1[i-2] = splittedLines[i];
                }
            }
            */
        //end of for loop   
        }else if(strcmp(splittedLines[0], "}") == 0){
            strcat(printline, "}");
            int len = strlen(printline);
            printline[len] = '\0';
            memcpy(resultArr[lineId], printline, 100);
        //print statements with first token is equal to print keyword
        }else if(strcmp(splittedLines[0], "print") == 0){
            // parenthesis is used to distinguish the expressions
            if(strcmp(splittedLines[1], "(") == 0){
                //initialization of expression arrays in order to convert them to postfix form 
                char *expr1[256];
                char *result2[256];
                for (int i = 0; i < 256; i++) expr1[i] = calloc(256, sizeof(char*));
                for (int i = 0; i < 256; i++) result2[i] = calloc(256, sizeof(char*));
                char postexpr[256];
                postexpr[0] = '\0';
                int cur = 2;
                int temp = cur;
                for (int i = cur; i < 256; i++){
                    cur++;
                    if(strcmp(splittedLines[i], ")")==0){
                        break;
                    }
                    memcpy(expr1[i-temp],splittedLines[i],10);
                }
                
                memcpy(expr1[cur-temp-1], "exit", 10); 
                if(!strcmp(splittedLines[cur-1], ")")==0){
                    error(lineId);
                    *err = true;
                    break;
                }
                expr( postexpr , expr1, varNames, lineId,err);
                int tmpId = 0;
                char *tmp = strtok(postexpr, " ");
                
                while( tmp != NULL ) {
                    //stores each token in array
                    memcpy(result2[tmpId], tmp, 256); 
                    tmpId += 1;
                    tmp = strtok(NULL, " ");
                }
                memcpy(result2[tmpId], "exit", 10);

                char res1[256];
                res1[0] = '\0';
                int* pvarNum;
                pvarNum = &varNumber;

                //convertion of postfix form expression into the infix form for printing in file.c
                postFixToInfix(result2, res1, varNames,varTypes, lineId, pvarNum, col_dimensions, row_dimensions, vectorArr);
                int index = find(res1,varNames);
                char * type = varTypes[index];
                // if id is scalar, there is no need for indexing
                if(strcmp(type,"scalar")==0){
                    //print to c file
                    strcat(printline, "printf(\"%f\", ");
                    strcat(printline, res1);
                    strcat(printline, ");");
                    int len = strlen(printline);
                    printline[len] = '\0';
                    memcpy(resultArr[lineId], printline, 100);
                // if id is in type of matrix, looks and checks for indexing
                }if (strcmp(type,"matrix")==0){   
                    //print to c file
                    char* colNum = col_dimensions[index];
                    char* rowNum = row_dimensions[index];
                    int len1 = strlen(colNum);
                    colNum[len1] = '\0';
                    int len2 = strlen(rowNum);
                    rowNum[len2] = '\0';
                    strcat(printline, "matrix_printer(");
                    strcat(printline, res1);
                    strcat(printline, ",");
                    strcat(printline, rowNum);
                    strcat(printline, ",");
                    strcat(printline, colNum);
                    strcat(printline, ");");
                    int len = strlen(printline);
                    printline[len] = '\0';
                    memcpy(resultArr[lineId], printline, 100);
                }
            }else{
                error(lineId);
                *err = true;
            }
            
        }
        // empty the array
        memset(newSentence,0,512);

    }
    fclose(fp);

    
    // if there is no error prints the output
    if(!err){
        out = fopen("./file.c","w");
        if(out == NULL)
        {
        printf("Error!");   
        exit(1);             
        }
            outfunc = fopen("./outputfunctions.c","r");
        if(outfunc == NULL)
        {
        printf("Error!");   
        exit(1);             
        }
        fprintf(out,"%s","#include <stdio.h> \n#include <string.h> \n#include <stdlib.h> \n#include <stdbool.h>\n#include <ctype.h> \n#include <math.h>\n");
        char funcs[512];
        while(fgets(funcs,512,outfunc) != NULL){
            fprintf(out,"%s",funcs);
        }
        
        fprintf(out, "%s", "int main (int argc,char *argv[]) {\n");
        for(int i = 0; i<256; i++){
            fprintf(out,"%s",resultArr[i]);
            fprintf(out,"%s","\n");
        }
        fprintf(out, "%s", "}");
        fclose(out);
    }
    


return(0);
}
