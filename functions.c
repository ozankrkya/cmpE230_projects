#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

// checks whether the token is reserved by matlang
bool isReserved(int c, int arr[]){
    for(int i = 0; i < 12; i++){
        if((int) c == arr[i]){
            return true;
        }
    }
    return false;
}

// takes char pointer and array as input and returns true if name is a variable
// example use isVariable(matrixArray[2].name[0], varNames)
bool isVariable(char* name , char* varNames[]){
    for(int i = 0; i < 256; i++){
        if(strcmp(name, varNames[i])==0) {
            return true;
        }
    }
    return false;
}
int isFunc(char * token) {
   if(strcmp(token, "tr")==0 || strcmp(token, "sqrt")==0 ){
      return 1;
   }else{
      return 0;
   }
}
//takes char pointer and array as input and returns the index of variable, if it is not a variable returns -1
int find(char* name , char* varNames[]){
    for(int i = 0; i < 256; i++){
        if(strcmp(name, varNames[i])==0) {
            return i;
        }
    }
    return -1;
}

//prints error line
void error(int lineId){
    printf("error in line %d", lineId);
}

// checks whether the input is number
bool isNumber(char* name){
    bool check = true;
    bool check2 = true;
    char* q;
    for(q = name; *q != '\0'; q++){
        if(isdigit(*q)){
            continue;
        }else if(check2 && strcmp(q,".")==0){
            check2 = false;
            continue;
        }else{
            check = false;
        }
    }
    return check;
}
int isInteger(char *token) 
{
    int check = true ;
    char *q ; 

    for(q = token ; *q != '\0' ; q++) {
        check = check && isdigit(*q) ;
    }

    return(check) ; 
}    






