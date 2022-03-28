#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// checks whether the token is reserved by matlang
bool isReserved(char c, int arr[]){
    for(int i = 0; i < sizeof(arr)/sizeof(arr[0]); i++){
        if((int) c == arr[i]){
            return true;
        }
    }
    return false;
}