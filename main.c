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
        
        //printf("%s \n",newSentence);
        
        
        
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
            memcpy(varNames[varNumber], splittedLines[1], 100);
            varNumber++;
        }else if(strcmp(splittedLines[0],"vector")==0){
            memcpy(varNames[varNumber], splittedLines[1], 100);
            varNumber++;
        }else if(strcmp(splittedLines[0],"scalar")==0){
            memcpy(varNames[varNumber], splittedLines[1], 100);
            varNumber++;
        }
        printf("%s \n",varNames[0]);
        

        // empty the array
        //printf("%s \n",splittedLines[0]);
        memset(newSentence,0,512);
        
    }
    
    
    fclose(fp);
    

return(0);

}