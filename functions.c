#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// checks whether the token is reserved by matlang
bool isReserved(int c, int arr[]){
    for(int i = 0; i < 11; i++){
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
//takes char pointer and array as input and returns the index of variable, if it is not a variable returns -1
int find(char* name , char* varNames[]){
    for(int i = 0; i < 256; i++){
        if(strcmp(name, varNames[i])==0) {
            return i;
        }
    }
    return -1;
}









