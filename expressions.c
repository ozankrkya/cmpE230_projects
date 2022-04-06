#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "functions.c"


char * array[20] = {"x", "=" ,"{", "1", "1", "}"};
char * operators[5] = {"x", "+", "-", "(", ")"};

int MAXSIZE = 256;       
char * stack[256];     
int top = -1;            

int isEmpty() {

   if(top == -1)
      return 1;
   else
      return 0;
}
   
int isFull() {

   if(top == MAXSIZE)
      return 1;
   else
      return 0;
}

char * peek() {
   return stack[top];
}

char * pop() {
   char * data;
	
   if(!isEmpty()) {
      data = stack[top];
      top = top - 1;   
      return data;
   } else {
      printf("Could not retrieve data, Stack is empty.\n");
   }
}

void push(char * data) {

   if(!isFull()) {
      top = top + 1;   
      stack[top] = data;
   } else {
      printf("Could not insert data, Stack is full.\n");
   }
}



void postFixToInfix(char* sentence[], char * result, char * varNames[], char * varTypes[], int lineId, int varNum, char *coldm[], char*rowdm[]){
   bool check = true;
   char tokenTypes[256];
   tokenTypes[0] = '\0';
   char totalType[10];
   totalType[0] = '\0';
   char ** temps = malloc(sizeof(char*)*256);
   for (int i=0;i<256;i++){
      temps[i] = malloc(sizeof(char)*512);
   }
   for(int i = 0; i <256 ; i++){
      char * token = sentence[i];
      char* temp;
      temp = temps[i];
        
      int count = 0;
        
      char * temp1;
      char * temp2;
      char * temp3;
      printf("%s \n", token);
        
      if(strcmp(token, "exit") == 0){
         temp1 =pop();
         while(!isEmpty() && (strcmp(peek(),"sqrt")== 0)){
            strcat(temp, " ( ");
            temp2 = pop();
            strcat(temp, temp2);
         }
         strcat(temp, temp1);
         strcat(result, temp);
            //printf("%s", token);
         break;
      }
      if(strcmp(token, "+")==0 || strcmp(token, "-")== 0 || strcmp(token, "*")== 0){
         
            
         temp1 = pop();
         temp2 = pop();


         char * temp1type;
         char * temp2type;
         int temp1Id;
         int temp2Id;
         if(isNumber(temp1)){
            temp1type = "scalar";
         }else{
            temp1Id = find(temp1, varNames);
            temp1type = varTypes[temp1Id];
         }
         if(isNumber(temp2)){
            temp2type = "scalar";
         }else{
            temp2Id = find(temp2, varNames);
            temp2type = varTypes[temp2Id];
         }

         printf("%s", temp1type);
         if(strcmp(token, "+")==0 ){
            if(strcmp(temp1type, "matrix") == 0 ){
               if(strcmp(temp2type, "matrix") == 0){
                  if(strcmp(coldm[temp1Id], coldm[temp2Id])==0 && strcmp(rowdm[temp1Id], rowdm[temp2Id])==0){
                     strcat(temp, "matrix_add( ");
                     strcat(temp, temp2);
                     strcat(temp, " , ");
                     strcat(temp, temp1);
                     strcat(temp, " , ");
                     strcat(temp, rowdm[temp2Id]);
                     strcat(temp, " , ");
                     strcat(temp, coldm[temp2Id]);
                     strcat(temp, ")");
                     varNames[varNum] = temp;
                     varTypes[varNum] = "matrix";
                     coldm[varNum] = coldm[temp1Id];
                     rowdm[varNum] = rowdm[temp1Id];
                     varNum ++;
                  }else{
                              //cols and rows are not compatible
                     error(lineId);
                  }
               }else{
                           // you must add 2 matrices
                  error(lineId);
               }
            }else if(strcmp(temp2type, "matrix") == 0){
                     // you must add 2 matrices
               error(lineId);
            }else{
                     // scalar toplam
               strcat(temp, "( ");
               strcat(temp, temp2);
               strcat(temp, " + ");
               strcat(temp, temp1);
               strcat(temp, " ) ");
               varNames[varNum] = temp;
               varTypes[varNum] = "scalar";
               coldm[varNum] = NULL;
               rowdm[varNum] = NULL;
               varNum ++;
            }

         }else if(strcmp(token, "-")== 0 ){
            if(strcmp(temp1type, "matrix") == 0 ){
               if(strcmp(temp2type, "matrix") == 0){
                  if(strcmp(coldm[temp1Id], coldm[temp2Id])==0 && strcmp(rowdm[temp1Id], rowdm[temp2Id])==0){
                     strcat(temp, "matrix_sub( ");
                     strcat(temp, temp2);
                     strcat(temp, " , ");
                     strcat(temp, temp1);
                     strcat(temp, " , ");
                     strcat(temp, rowdm[temp2Id]);
                     strcat(temp, " , ");
                     strcat(temp, coldm[temp2Id]);
                     strcat(temp, ")");
                     varNames[varNum] = temp;
                     varTypes[varNum] = "matrix";
                     coldm[varNum] = coldm[temp1Id];
                     rowdm[varNum] = rowdm[temp1Id];
                     varNum ++;
                  }else{
                        //cols and rows are not compatible
                        error(lineId);
                  }
               }else{
                      // you must subtract 2 matrices
                  error(lineId);
               }
            }else if(strcmp(temp2type, "matrix") == 0){
                        // you must subtract 2 matrices
               error(lineId);
            }else{
                        // scalar fark
               strcat(temp, "( ");
               strcat(temp, temp2);
               strcat(temp, " - ");
               strcat(temp, temp1);
               strcat(temp, " ) ");
               varNames[varNum] = temp;
               varTypes[varNum] = "scalar";
               coldm[varNum] = NULL;
               rowdm[varNum] = NULL;
               varNum ++;
            }
               
         }else if (strcmp(token, "*")== 0){
            if(strcmp(temp1type, "matrix") == 0 ){
               if(strcmp(temp2type, "matrix") == 0){
                  if(strcmp(coldm[temp2Id], rowdm[temp1Id])==0){
                     strcat(temp, "matrix_mult( ");
                     strcat(temp, temp2);
                     strcat(temp, " , ");
                     strcat(temp, temp1);
                     strcat(temp, " , ");
                     strcat(temp, rowdm[temp2Id]);
                     strcat(temp, " , ");
                     strcat(temp, coldm[temp2Id]);
                     strcat(temp, " , ");
                     strcat(temp, coldm[temp1Id]);
                     strcat(temp, ")");
                     varNames[varNum] = temp;
                     varTypes[varNum] = "matrix";
                     coldm[varNum] = coldm[temp1Id];
                     rowdm[varNum] = rowdm[temp2Id];
                     varNum ++;
                  }else{
                     //cols and rows are not compatible
                     error(lineId);
                  }
               }else{
                     //matrix Scalar yazılacak
               }
            }else if(strcmp(temp2type, "matrix") == 0){
                  // matrix scalar yazılacak
            }else{
               // scalar çarpım
               strcat(temp, "( ");
               strcat(temp, temp2);
               strcat(temp, " * ");
               strcat(temp, temp1);
               strcat(temp, " ) ");
               varNames[varNum] = temp;
               varTypes[varNum] = "scalar";
               coldm[varNum] = NULL;
               rowdm[varNum] = NULL;
               varNum ++;

            }
                  
              
         }
                     

                  
         printf("%s", temp);     
         push(temp);
                  
            //printf("0 %s \n", stack[0]);
            //printf("0 %s \n", stack[1]);
            // printf("tmp1 %s \n" , temp1);
               
               
         
      }else {
            //printf("%s", token);


      
            push(token);
        }
        //memset(temp,0,256);
        //printf("after %s \n", stack[0]);
        //printf("%s \n", stack[1]);
        //printf("%d ", top);
      
    }

   free(temps);

}