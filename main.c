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
            if(strcmp(splittedLines[2],"[")==0 && strcmp(splittedLines[6],"]")==0&& isInteger(splittedLines[3])&& isInteger(splittedLines[5])&&\
             strcmp(splittedLines[4],",")==0){
                memcpy(varTypes[varNumber], splittedLines[0], 100);
                if(isValidName(splittedLines[1])){
                    memcpy(varNames[varNumber], splittedLines[1], 100);
                    memcpy(row_dimensions[varNumber], splittedLines[3], 100);
                    memcpy(col_dimensions[varNumber], splittedLines[5], 100);
                }else{
                    error(lineId);
                    break;
                }
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
                memcpy(resultArr[lineId], printline, 256);
                
                varNumber++;
            }else{
                error(lineId);
                break;
            }

        }else if(strcmp(splittedLines[0],"vector")==0){
            memcpy(vectorArr[vectorNum],splittedLines[1],24);
            vectorNum++;
            if(strcmp(splittedLines[2],"[")==0 && strcmp(splittedLines[4],"]")==0 && isInteger(splittedLines[3])){
                memcpy(varTypes[varNumber], "matrix", 100);
                if(isValidName(splittedLines[1])){
                    memcpy(varNames[varNumber], splittedLines[1], 100);
                    memcpy(row_dimensions[varNumber], splittedLines[3], 100);
                }else{
                    error(lineId);
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
                memcpy(resultArr[lineId], printline, 256);
                varNumber++;
            }else{
                error(lineId);
                break;
            }

        }else if(strcmp(splittedLines[0],"scalar")==0){
            memcpy(varTypes[varNumber], splittedLines[0], 100);

            if(isValidName(splittedLines[1])){
                memcpy(varNames[varNumber], splittedLines[1], 100);
            }else{
                error(lineId);
                break;
            }
            row_dimensions[varNumber] = NULL;
            col_dimensions[varNumber] = NULL;
            // printing to c file 
            strcat(printline, "double ") ;    
            strcat(printline, splittedLines[1]) ;
            strcat(printline, "; ") ;  
            memcpy(resultArr[lineId], printline, 100);
            varNumber++;

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
                            }
                        }
                        memcpy(resultArr[lineId], printline, 100);
                        for (int i=0;i<entry_number;i++){
                        
                        }
                    }else if(false){
                        //matrix can assigned by expression assign matrix here


                    }
                    else{
                        //error(lineId);
                        //where are my curly braces or there are not right number of entries for variable matrix
                        //break;
                    }
                    
                }             
            }
            else{
                error(lineId);
                //there is no such variable declared to assign
                break;
            }    
            
        }else if(strcmp(splittedLines[0],"for") == 0){
            //single for
            if(strcmp(splittedLines[3],"in") == 0 && strcmp(splittedLines[1],"(") == 0){
                char *expr1[256];
                for (int i = 0; i < 256; i++) expr1[i] = calloc(256, sizeof(char*));
                char *expr2[256];
                for (int i = 0; i < 256; i++) expr2[i] = calloc(256, sizeof(char*));
                char *expr3[256];
                for (int i = 0; i < 256; i++) expr3[i] = calloc(256, sizeof(char*));
                int cur = 4;
                /*
                char expr1[256];
                char expr2[256];
                char expr3[256];
                expr1[0] = '\0';
                expr2[0] = '\0';
                expr3[0] = '\0';
                */
                for (int i = 4; i < 256; i++){
                    cur++;
                    if(strcmp(splittedLines[i], ":")==0){
                        break;
                    }
                    expr1[i-4] = splittedLines[i];
                }
                int temp = cur;
                for (int i = cur; i < 256; i++){
                    cur++;
                    if(strcmp(splittedLines[i], ":")==0){
                        break;
                    }
                    expr2[i-temp] = splittedLines[i];
                }
                temp = cur;
                for (int i = cur; i < 256; i++){
                    cur++;
                    if(strcmp(splittedLines[i], ")")==0){
                        break;
                    }
                    expr3[i-temp] = splittedLines[i];
                }
                if(!strcmp(splittedLines[cur-1], ")")==0){
                    error(lineId);
                    break;
                }
                



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
                /*
                char expr1[256];
                char expr2[256];
                char expr3[256];
                expr1[0] = '\0';
                expr2[0] = '\0';
                expr3[0] = '\0';
                */
                for (int i = 6; i < 256; i++){
                    cur++;
                    if(strcmp(splittedLines[i], ":")==0){
                        break;
                    }
                    expr1[i-6] = splittedLines[i];
                }
                int temp = cur;
                printf("%d", cur);
                for (int i = cur; i < 256; i++){
                    printf("%s", splittedLines[i]);
                    cur++;
                    if(strcmp(splittedLines[i], ":")==0){
                        break;
                    }
                    expr2[i-temp] = splittedLines[i];
                }
                temp = cur;
                for (int i = cur; i < 256; i++){
                    cur++;
                    if(strcmp(splittedLines[i], ",")==0){
                        break;
                    }
                    expr3[i-temp] = splittedLines[i];
                }
                temp = cur;
                for (int i = cur; i < 256; i++){
                    cur++;
                    if(strcmp(splittedLines[i], ":")==0){
                        break;
                    }
                    expr4[i-temp] = splittedLines[i];
                }
                temp = cur;
                for (int i = cur; i < 256; i++){
                    cur++;
                    if(strcmp(splittedLines[i], ":")==0){
                        break;
                    }
                    expr5[i-temp] = splittedLines[i];
                }
                temp = cur;
                for (int i = cur; i < 256; i++){
                    cur++;
                    if(strcmp(splittedLines[i], ")")==0){
                        break;
                    }
                    expr6[i-temp] = splittedLines[i];
                }
                if(!strcmp(splittedLines[cur-1], ")")==0){
                    error(lineId);
                    break;
                }
                printf("bu %s \n",expr2[0]);
                printf("bu %s \n",expr1[2]);
            }else{
                error(lineId);
            }
            
        //change matrix index
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

            
            
            

            
            
            
            
        }
        if(lineId == 22){
            char result[256];
            char *result2[256];
            for (int i = 0; i < 256; i++) result2[i] = calloc(256, sizeof(char*));
            int tmpId = 0;
            //printf("bu %s \n", splittedLines[3]);
            expr( result , splittedLines, varNames);
            printf("%s \n", result);
            
            char *tmp = strtok(result, " ");
            while( tmp != NULL ) {
                //stores each token in array
                memcpy(result2[tmpId], tmp, 256); 
                tmpId += 1;
                tmp = strtok(NULL, " ");
            }
            memcpy(result2[tmpId], "exit", 256); 
            char result3[256];
            result3[0] = '\0';
            //printf("%d", tmpId);
            //printf("%s", result2[5]);
            int* pvarNum;
            pvarNum = &varNumber;
            
            postFixToInfix(result2, result3, varNames,varTypes, lineId, pvarNum, col_dimensions, row_dimensions, vectorArr);
            printf("result in main: %s",result3);
            printf("varnum: %d\n",*pvarNum);
            //printf("%s \n", result3);
            //printf("%s \n", resultArr[4]);
            //printf("%s \n", resultArr[15]);
            
        }

        
        
        
        // empty the array
        //printf("%s \n",splittedLines[0]);
        memset(newSentence,0,512);

    }
    fclose(fp);

    out = fopen("./file.c","w");
    if(out == NULL)
    {
      printf("Error!");   
      exit(1);             
    }


    
    fprintf(out,"%s","#include <stdio.h> \n#include <string.h> \n#include <stdlib.h> \n#include <stdbool.h>\n#include <ctype.h> \n");
    fprintf(out, "%s", "int main (int argc,char *argv[]) {\n");
    for(int i = 0; i<256; i++){
        fprintf(out,"%s",resultArr[i]);
        fprintf(out,"%s","\n");
    }
    fprintf(out, "%s", "}");
    fclose(out);
    
    
return(0);


return(0);
}
