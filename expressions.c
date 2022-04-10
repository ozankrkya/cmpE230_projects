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



void postFixToInfix(char* sentence[], char * result, char * varNames[], char * varTypes[], int lineId, int *varNum, char *coldm[], char*rowdm[], char *vectors[]){

   char ** temps = malloc(sizeof(char*)*256);
   for (int i=0;i<256;i++){
      temps[i] = malloc(sizeof(char)*512);
   }

   for(int i = 0; i <256 ; i++){
      char * token = sentence[i];
      char* temp;
      temp = temps[i];
      char * temp1;
      char * temp2;
      char * temp3;
      if(strcmp(token, "exit") == 0){
         temp1 = pop();
         strcpy(result,temp1);
         break;
      }

      if (strcmp(token,"sqrt") == 0){
         char ** expr_rec = malloc(sizeof(char*)*256);
         for (int i=0;i<256;i++){
            expr_rec[i] = malloc(sizeof(char)*256);
         } 
         int index_paran = 0;
         int paran[80];
         paran[0] = 1;
         index_paran++;
         int sentence_index = i+2;
         int rec_expr_index = 0;
         while(paran[0] != 0){
            if(strcmp(sentence[sentence_index],"(")==0){
               paran[index_paran] = 1;
               strcpy(expr_rec[rec_expr_index],sentence[sentence_index]);
               index_paran++;
               sentence_index++;
               rec_expr_index++;
            }
            else if(strcmp(sentence[sentence_index],")")==0){
               if(index_paran!=1){
                  index_paran-=1;
                  paran[index_paran] = 0;
                  strcpy(expr_rec[rec_expr_index],sentence[sentence_index]);
                  sentence_index++;
                  rec_expr_index++;
               }
               else{
                  index_paran-=1;
                  paran[index_paran] = 0;
                  expr_rec[rec_expr_index] = "exit";
               }
            }else{
               strcpy(expr_rec[rec_expr_index],sentence[sentence_index]);
               sentence_index++;
               rec_expr_index++;
            }
         }
         char *res = calloc(256,sizeof(char));
         postFixToInfix(expr_rec, res, varNames, varTypes, lineId, varNum, coldm, rowdm, vectors);
         strcat(temp, "sqrt");
         strcat(temp, "(");
         strcat(temp, res);
         strcat(temp,")");
         int res_index = find(res, varNames);
         if(strcmp(varTypes[res_index],"scalar")==0){
            varNames[*varNum] = temp;
            varTypes[*varNum] = "scalar";
            rowdm[*varNum] = NULL;
            coldm[*varNum] = NULL;
            (*varNum)++;
            push(temp);
            i = sentence_index; 
            continue;
         }
         else if(strcmp(varTypes[res_index],"matrix")==0){
            //printf("matrixle cagrildim\n");
            error(lineId);
            // sqrt doesnt return scalar
         }
      }

      // implementation for transpose

      if(strcmp(token,"tr")==0){
         char ** expr_rec = malloc(sizeof(char*)*256);
         for (int i=0;i<256;i++){
            expr_rec[i] = malloc(sizeof(char)*256);
         }
         int index_paran = 0;
         int paran[80];
         paran[0] = 1;
         index_paran++;
         int sentence_index = i+2;
         int rec_expr_index = 0;
         while(paran[0] != 0){
            if(strcmp(sentence[sentence_index],"(")==0){
               paran[index_paran] = 1;
               strcpy(expr_rec[rec_expr_index],sentence[sentence_index]);
               index_paran++;
               sentence_index++;
               rec_expr_index++;
            }
            else if(strcmp(sentence[sentence_index],")")==0){
               if(index_paran!=1){
                  index_paran-=1;
                  paran[index_paran] = 0;
                  strcpy(expr_rec[rec_expr_index],sentence[sentence_index]);
                  sentence_index++;
                  rec_expr_index++;
               }
               else{
                  index_paran-=1;
                  paran[index_paran] = 0;
                  expr_rec[rec_expr_index] = "exit";
               }
            }else{
               strcpy(expr_rec[rec_expr_index],sentence[sentence_index]);
               sentence_index++;
               rec_expr_index++;
            }
         }
         char *res = calloc(256,sizeof(char));
         postFixToInfix(expr_rec, res, varNames, varTypes, lineId, varNum, coldm, rowdm, vectors);
         strcat(temp, "tr");
         strcat(temp, "(");
         strcat(temp, res);
         strcat(temp,")");
         int res_index = find(res, varNames);
         //checks for the type returned from the inside
         if(strcmp(varTypes[res_index],"scalar")==0){
            varNames[*varNum] = temp;
            varTypes[*varNum] = "scalar";
            rowdm[*varNum] = NULL;
            coldm[*varNum] = NULL;
            (*varNum)++;
            push(temp);
            i = sentence_index; 
            continue;
         }
         else if(strcmp(varTypes[res_index],"matrix")==0){
            varNames[*varNum] = temp;
            varTypes[*varNum] = "matrix";
            coldm[*varNum] = rowdm[res_index];
            rowdm[*varNum] = coldm[res_index];
            //printf("col now: %s\n",coldm[varNum]);
            (*varNum)++;
            push(temp);
            i = sentence_index; 
            continue;
         }
      }

      //implementation for choose
      if (strcmp(token,"choose")==0){
         char ** expr1 = malloc(sizeof(char*)*256);
         for (int i=0;i<256;i++){
            expr1[i] = malloc(sizeof(char)*256);
         }
         char ** expr2 = malloc(sizeof(char*)*256);
         for (int i=0;i<256;i++){
            expr2[i] = malloc(sizeof(char)*256);
         }
         char ** expr3 = malloc(sizeof(char*)*256);
         for (int i=0;i<256;i++){
            expr3[i] = malloc(sizeof(char)*256);
         }
         char ** expr4 = malloc(sizeof(char*)*256);
         for (int i=0;i<256;i++){
            expr4[i] = malloc(sizeof(char)*256);
         }

         int pipe_count = 0;
         int sentence_index = i+2;
         int expr_index = 0;
         while(pipe_count!=1){
            if (strcmp(sentence[sentence_index],"choose")==0){
               pipe_count -= 3;
               strcpy(expr1[expr_index],sentence[sentence_index]);
               sentence_index++;
               expr_index++;
            }
            else if(strcmp(sentence[sentence_index],"|")==0){
               if(pipe_count<0){
                  strcpy(expr1[expr_index],sentence[sentence_index]);
                  sentence_index++;
                  expr_index++;   
                  pipe_count++;    
               }
               else{
                  pipe_count++;
                  sentence_index++;
                  expr1[expr_index] = "exit";
                  continue;
               }
            }
            else{
               strcpy(expr1[expr_index],sentence[sentence_index]);
               sentence_index++;
               expr_index++;
            }
         }
         expr_index = 0;

         while(pipe_count!=2){
            if (strcmp(sentence[sentence_index],"choose")==0){
               pipe_count -= 3;
               strcpy(expr2[expr_index],sentence[sentence_index]);
               sentence_index++;
               expr_index++;
            }
            else if(strcmp(sentence[sentence_index],"|")==0){
               if(pipe_count<1){
                  strcpy(expr2[expr_index],sentence[sentence_index]);
                  sentence_index++;
                  expr_index++;   
                  pipe_count++;    
               }
               else{
                  pipe_count++;
                  sentence_index++;
                  expr2[expr_index] = "exit";
                  continue;
               }
            }
            else{
               strcpy(expr2[expr_index],sentence[sentence_index]);
               sentence_index++;
               expr_index++;
            }
         }
         expr_index = 0;

         while(pipe_count!=3){
            if (strcmp(sentence[sentence_index],"choose")==0){
               pipe_count -= 3;
               strcpy(expr3[expr_index],sentence[sentence_index]);
               sentence_index++;
               expr_index++;
            }
            else if(strcmp(sentence[sentence_index],"|")==0){
               if(pipe_count<2){
                  strcpy(expr3[expr_index],sentence[sentence_index]);
                  sentence_index++;
                  expr_index++;   
                  pipe_count++;    
               }
               else{
                  pipe_count++;
                  sentence_index++;
                  expr3[expr_index] = "exit";
                  continue;
               }
            }
            else{
               strcpy(expr3[expr_index],sentence[sentence_index]);
               sentence_index++;
               expr_index++;
            }
         }
         expr_index = 0;
         int paran_count = 0;

         while(paran_count != -1){
            if(strcmp(sentence[sentence_index],"(")==0){
               strcpy(expr4[expr_index],sentence[sentence_index]);
               paran_count++;
               expr_index++;
               sentence_index++;
            }
            else if(strcmp(sentence[sentence_index],")")==0){
               if(paran_count > 0){
                  strcpy(expr4[expr_index],sentence[sentence_index]);
                  paran_count -= 1;
                  expr_index++;
                  sentence_index++;
               }
               else{
                  paran_count -= 1;
                  expr4[expr_index] = "exit";
                  continue;
               }
            }
            else{
               strcpy(expr4[expr_index],sentence[sentence_index]);
               expr_index++;
               sentence_index++;
            }
         }
         char *res1 = calloc(256,sizeof(char));
         char *res2 = calloc(256,sizeof(char));
         char *res3 = calloc(256,sizeof(char));
         char *res4 = calloc(256,sizeof(char));
         postFixToInfix(expr1, res1, varNames, varTypes, lineId, varNum, coldm, rowdm, vectors);
         postFixToInfix(expr2, res2, varNames, varTypes, lineId, varNum, coldm, rowdm, vectors);
         postFixToInfix(expr3, res3, varNames, varTypes, lineId, varNum, coldm, rowdm, vectors);
         postFixToInfix(expr4, res4, varNames, varTypes, lineId, varNum, coldm, rowdm, vectors);

         int res1_index = find(res1,varNames);
         int res2_index = find(res2,varNames);
         int res3_index = find(res3,varNames);
         int res4_index = find(res4,varNames);

         if(strcmp(varTypes[res1_index],"scalar") == 0 && strcmp(varTypes[res2_index],"scalar") == 0 \
         && strcmp(varTypes[res3_index],"scalar") == 0 && strcmp(varTypes[res4_index],"scalar") == 0){
         //token to be pushed is created   
            strcat(temp,"choose");
            strcat(temp,"(");
            strcat(temp,res1);
            strcat(temp,",");
            strcat(temp,res2);
            strcat(temp,",");
            strcat(temp,res3);
            strcat(temp,",");
            strcat(temp,res4);
            strcat(temp,")");
            printf("temp in choose: %s\n",temp);
            //it is initialized in the arrays
            varNames[*varNum] = temp;
            varTypes[*varNum] == "scalar";
            rowdm[*varNum] = NULL;
            coldm[*varNum] = NULL;
            (*varNum)++;
            i = sentence_index;
            push(temp);
            continue;
         }
         else{
            //choose expressions must be in type of scalar
            printf("error line 393\n");
            error(lineId);
            continue;
         }
      }
      //BURAYA BAKILACAK DEBUG LAZIM

      //DEBUG LAZIM

      //implementation for indexing 
      
      if(strcmp(token,"[")==0){     
         if (isVector(sentence[i-1],vectors)){   
            char ** exprv = malloc(sizeof(char*)*256);
            for (int i=0;i<256;i++){
               exprv[i] = malloc(sizeof(char)*256);
            }
            int bracket_count = 0;
            int sentence_indexv = i+1;
            int expr_indexv = 0;
           
            while(bracket_count  > -1){
               if(strcmp(sentence[sentence_indexv],"[")==0){
                  strcpy(exprv[expr_indexv],sentence[sentence_indexv]);
                  bracket_count++;
                  expr_indexv++;
                  sentence_indexv++;
               }
               else if(strcmp(sentence[sentence_indexv],"]")==0){
                  if(bracket_count > 0){
                     strcpy(exprv[expr_indexv],sentence[sentence_indexv]);
                     bracket_count -= 1;
                     expr_indexv++;
                     sentence_indexv++;
                  }
                  else{
                     bracket_count -= 1;
                     exprv[expr_indexv] = "exit";
                     continue;
                  }
               }
               else{
                  strcpy(exprv[expr_indexv],sentence[sentence_indexv]);
                  expr_indexv++;
                  sentence_indexv++;
               }
            }
            printf("435\n");
            char *resv = calloc(256,sizeof(char));
            postFixToInfix(exprv, resv, varNames, varTypes, lineId, varNum, coldm, rowdm, vectors);
            int resv_index = find(resv, varNames);
            if ((strcmp(varTypes[resv_index],"scalar")==0) || isInteger(resv)){
               strcat(temp,sentence[i-1]);
               strcat(temp,"[");
               strcat(temp,resv);
               strcat(temp,"]");
               strcat(temp,"[");
               strcat(temp,"1");
               strcat(temp,"]");
               varNames[*varNum] = temp;
               varTypes[*varNum] = "scalar";
               rowdm[*varNum] = NULL;
               coldm[*varNum] = NULL;
               (*varNum)++;
               push(temp);
               i = sentence_indexv;
               continue;
            }
            else{
               printf("error458:\n");
               error(lineId);
               //index type must be a scalar
            }
            
         }
         else{
            
            int matrix_index = find(sentence[i-1],varNames);
            
            if(strcmp(varTypes[matrix_index],"matrix")==0){
               char ** exprm_1 = malloc(sizeof(char*)*256);
               for (int i=0;i<256;i++){
                  exprm_1[i] = malloc(sizeof(char)*256);
               }
               char ** exprm_2 = malloc(sizeof(char*)*256);
               for (int i=0;i<256;i++){
                  exprm_2[i] = malloc(sizeof(char)*256);
               }
               int at_count = 0;
               int sentence_index = i+1;
               int expr_index = 0;
               while(at_count < 1){
                  if(strcmp(sentence[sentence_index],"[")==0){
                     if(isVector(sentence[sentence_index-1],vectors)){
                        strcpy(exprm_1[expr_index],sentence[sentence_index]);
                        expr_index++;
                        sentence_index++;
                     }
                     else{
                        at_count -= 1;
                        strcpy(exprm_1[expr_index],sentence[sentence_index]);
                        expr_index++;
                        sentence_index++;
                     }
                  }
                  else if(strcmp(sentence[sentence_index],"@")==0){
                     if(at_count < 0){
                        strcpy(exprm_1[expr_index],sentence[sentence_index]);
                        expr_index++;
                        sentence_index++;
                        at_count++;
                     }
                     else{
                        exprm_1[expr_index] = "exit";
                        at_count++;
                        sentence_index++;
                        continue;
                     }
                  }
                  else{
                     strcpy(exprm_1[expr_index],sentence[sentence_index]);
                     expr_index++;
                     sentence_index++;
                  }
               }
               expr_index = 0;
               int bracket_count = 0;
               while(bracket_count  > -1){
                  if(strcmp(sentence[sentence_index],"[")==0){
                     strcpy(exprm_2[expr_index],sentence[sentence_index]);
                     bracket_count++;
                     expr_index++;
                     sentence_index++;
                  }
                  else if(strcmp(sentence[sentence_index],"]")==0){
                     if(bracket_count > 0){
                        strcpy(exprm_2[expr_index],sentence[sentence_index]);
                        bracket_count -= 1;
                        expr_index++;
                        sentence_index++;
                     }
                     else{
                        bracket_count -= 1;
                        exprm_2[expr_index] = "exit";
                        continue;
                     }
                  }
                  else{
                     strcpy(exprm_2[expr_index],sentence[sentence_index]);
                     expr_index++;
                     sentence_index++;
                  }
               }
               char *resm1 = calloc(256,sizeof(char));
               char *resm2 = calloc(256,sizeof(char));
               postFixToInfix(exprm_1, resm1, varNames, varTypes, lineId, varNum, coldm, rowdm, vectors);
               postFixToInfix(exprm_2, resm2, varNames, varTypes, lineId, varNum, coldm, rowdm, vectors);
               int resm1_index = find(resm1, varNames);
               int resm2_index = find(resm2, varNames);
               printf("resm1: %d",isInteger(resm1));
               if( ((strcmp(varTypes[resm1_index],"scalar")==0) || isInteger(resm1)) && ((strcmp(varTypes[resm2_index],"scalar")==0) || isInteger(resm2))){
                  strcat(temp,sentence[i-1]);
                  strcat(temp,"[");
                  strcat(temp,resm1);
                  strcat(temp,"]");
                  strcat(temp,"[");
                  strcat(temp,resm2);
                  strcat(temp,"]");
                  varNames[*varNum] = temp;
                  varTypes[*varNum] = "scalar";
                  rowdm[*varNum] = NULL;
                  coldm[*varNum] = NULL;
                  (*varNum)++;
                  push(temp);
                  i = sentence_index;
                  continue;
               }
               else{
                  //index type must be a scalar
                  error(lineId);
                  break;
               }
            }
            else{
               error(lineId);
               break;
               //brackets cannot be used with scalars. 
            }
         }
      }
      
   
      if(strcmp(token, "+")==0 || strcmp(token, "-")== 0 || strcmp(token, "*")== 0) {
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

         //printf("%s", temp1type);
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
                     varNames[*varNum] = temp;
                     varTypes[*varNum] = "matrix";
                     coldm[*varNum] = coldm[temp1Id];
                     rowdm[*varNum] = rowdm[temp1Id];
                     (*varNum)++;
                  }else{
                      //cols and rows are not compatible
                     error(lineId);
                     break;
                  }
               }else{
                  // you must add 2 matrices
                  error(lineId);
                  break;
               }
            }else if(strcmp(temp2type, "matrix") == 0){
                     // you must add 2 matrices
               error(lineId);
               break;
            }else{
               // scalar toplam
               strcat(temp, "( ");
               strcat(temp, temp2);
               strcat(temp, " + ");
               strcat(temp, temp1);
               strcat(temp, " ) ");
               varNames[*varNum] = temp;
               varTypes[*varNum] = "scalar";
               coldm[*varNum] = NULL;
               rowdm[*varNum] = NULL;
               (*varNum)++;
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
                     varNames[*varNum] = temp;
                     varTypes[*varNum] = "matrix";
                     coldm[*varNum] = coldm[temp1Id];
                     rowdm[*varNum] = rowdm[temp1Id];
                     (*varNum)++;
                  }else{
                     //cols and rows are not compatible
                     error(lineId);
                     break;
                  }
               }else{
                      // you must subtract 2 matrices
                  error(lineId);
                  break;
               }
            }else if(strcmp(temp2type, "matrix") == 0){
                        // you must subtract 2 matrices
               error(lineId);
               break;
            }else{
                        // scalar fark
               strcat(temp, "( ");
               strcat(temp, temp2);
               strcat(temp, " - ");
               strcat(temp, temp1);
               strcat(temp, " ) ");
               varNames[*varNum] = temp;
               varTypes[*varNum] = "scalar";
               coldm[*varNum] = NULL;
               rowdm[*varNum] = NULL;
               (*varNum)++;
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
                     varNames[*varNum] = temp;
                     varTypes[*varNum] = "matrix";
                     coldm[*varNum] = coldm[temp1Id];
                     rowdm[*varNum] = rowdm[temp2Id];
                     (*varNum)++;
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
               varNames[*varNum] = temp;
               varTypes[*varNum] = "scalar";
               coldm[*varNum] = NULL;
               rowdm[*varNum] = NULL;
               (*varNum)++;
            }    
         }         
         push(temp);
         //printf("0 %s \n", stack[0]);
         //printf("0 %s \n", stack[1]);
         //printf("tmp1 %s \n" , temp1);
      }else {
            //printf("token: %s\n", token);
            push(token);
      } 
      //memset(temp,0,256);
      //printf("after %s \n", stack[0]);
      //printf("%s \n", stack[1]);
      //printf("%d ", top); 
   }
   free(temps);
}