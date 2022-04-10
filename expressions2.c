#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "expressions.c"

#define MAXTOKENS   100 
#define TOKENLENGTH 20 
#define N  1000 


char str[N] ; 
int  cur = 0 ; 

int  expr(char *, char**, char**, int, bool *) ;
int  term(char *, char**, char**, int, bool *) ;
int  moreterms(char *, char**, char**, int, bool *) ;
int  factor(char *, char**, char**, int, bool *) ;
int  morefactors(char *, char**, char**, int, bool *) ;
int  is_integer(char *) ; 
int isFunction(char * token);
int isChoose(char * token);
int isVectorId(char * token);


   int     numtokens ; 
   
  

// takes the sentence as char pointer array, then parses the array recursively
int expr(char *str, char * tokens[], char * varNames[], int lineId, bool * err)
{
   char str1[N], str2[N] ; 
   str1[0] = str2[0] = '\0' ; 
   if (!  term(str1 , tokens, varNames, lineId,err)) {
      return(0) ; 
   } 
   if (!  moreterms(str2 , tokens, varNames, lineId,err)) {
      return(0) ; 
   }
   
   strcat(str1,str2) ; 
   strcpy(str,str1) ; 
   int len = strlen(str);
   str[len] = '\0';
   if(strcmp(tokens[cur], "exit")==0){
      cur = 0;
   }
   return(1) ;  
}
// looks for factors and more factors to split term
int term(char *str, char * tokens[], char * varNames[], int lineId, bool * err)
{
   char str1[N], str2[N] ; 
   
   str1[0] = str2[0] = '\0' ; 
   if (!  factor(str1 , tokens, varNames,lineId,err)) {
      return(0) ; 
   } 
   if (!  morefactors(str2 , tokens, varNames, lineId,err)) {
      return(0) ; 
   }

   strcat(str1,str2) ; 
   strcpy(str,str1) ; 
   int len = strlen(str);
   str[len] = '\0';
   return(1) ;  
}
// looks for terms and more terms to split moreterms
int moreterms(char *str , char * tokens[], char * varNames[], int lineId, bool * err)
{
   char str1[N], str2[N], str3[N] ; 
   
   str1[0] = str2[0] = str3[0] = '\0' ; 
   
   if ( (strcmp(tokens[cur],"+") == 0 ) || (strcmp(tokens[cur],"-") == 0 ) ) {
       strcpy(str1,tokens[cur]) ; 
       int len = strlen(str1);
       str1[len] = '\0';
       strcat(str1," ") ; 
       cur++ ;  
       if (!  term(str2, tokens, varNames, lineId,err)) {
          return(0) ; 
       } 
       if (!  moreterms(str3, tokens, varNames, lineId,err)) {
         return(0) ; 
       }
   } 
   strcat(str2,str1) ; 
   strcat(str3,str2) ; 
   strcpy(str,str3) ; 
   int len = strlen(str);
   str[len] = '\0';
   return(1) ;  
}
// looks factors, if it is an expression, calls expression function. else returns base case
int factor(char *str , char* tokens[], char * varNames[],int lineId, bool * err)
{
   char str1[N] ; 
   char str2[N] ; 
   char str3[N] ; 
   char str4[N] ; 
   
   str1[0] = '\0' ; 
   str2[0] = '\0' ;
   str3[0] = '\0' ;
   str4[0] = '\0' ;

    
    if ( isNumber(tokens[cur])  ) {
       strcat(str,tokens[cur]) ; 
       strcat(str," ") ; 
       cur++ ; 
       return(1) ; 
    } 
    
    if ( isVariable(tokens[cur], varNames)  ) {
       //converts variables to postfix
       strcat(str,tokens[cur]) ; 
       strcat(str," ") ; 
       cur++ ; 
       if ( strcmp(tokens[cur],"[") == 0 ) {
         strcat(str,tokens[cur]) ; 
         strcat(str," ") ; 
         cur++ ; 
         if ( ! expr(str1, tokens, varNames,lineId,err) ) {
            return(0) ;    
         }
   
         strcat(str,str1) ; 
         strcat(str," ") ; 
         
         if ( strcmp(tokens[cur],",") == 0 ) { 
            
            strcat(str,"@") ; 
            strcat(str," ") ; 
            cur ++;
            if ( ! expr(str2, tokens, varNames, lineId,err) ) {
            return(0) ;    
            }
            strcat(str,str2) ; 
            strcat(str," ") ;
            if ( strcmp(tokens[cur],"]") != 0 ) { 
               printf("%s", tokens[cur]);
               error(lineId) ; 
               *err =true;
               return(0) ;
            }
            
         }else{
            if (strcmp(tokens[cur],"]") != 0 ) { 
               printf("%s", tokens[cur]);
               error(lineId) ; 
               *err =true;
               return(0) ;
            }
         }
         
         
         
         strcat(str,tokens[cur]) ; 
        
         strcat(str," ") ; 
          
       }
       return(1) ; 
    } 
    if ( strcmp(tokens[cur],"(") == 0 ) {
       cur++ ; 
       if ( ! expr(str1, tokens, varNames, lineId,err) ) {
          return(0) ;    
       }
       if ( strcmp(tokens[cur],")") != 0 ) { 
          printf("%s", tokens[cur]);
          error(lineId) ; 
          *err =true;
          return(0) ;
       }
       cur++ ; 
       strcpy(str,str1) ; 
       int len = strlen(str);
       str[len] = '\0';
       return(1) ; 
    }
    if(isFunction(tokens[cur])){
        
       strcat(str,tokens[cur]) ; 
       strcat(str," ") ; 
       cur ++;
       if ( strcmp(tokens[cur],"(") == 0 ) {
         strcat(str,tokens[cur]) ; 
         strcat(str," ") ; 
         cur++ ; 
         if ( ! expr(str1, tokens, varNames, lineId,err) ) {
            return(0) ;    
         }
         if ( strcmp(tokens[cur],")") != 0 ) { 
            printf("%s", tokens[cur]);
            error(lineId) ; 
            *err =true;
            return(0) ;
         }
         strcat(str, str1);
         strcat(str,tokens[cur]) ; 
         strcat(str," ") ; 
         
         cur++ ; 
         
         
         return(1) ; 
        }else{
         error(lineId) ; 
         *err =true;
         return 0;
      }
    }
    if(isChoose(tokens[cur])){
        //converts choose to postfix
       strcat(str,tokens[cur]) ; 
       strcat(str," ") ; 
       cur ++;
       if ( strcmp(tokens[cur],"(") == 0 ) {
         strcat(str,tokens[cur]) ; 
         strcat(str," ") ; 
         cur++ ; 
         if ( ! expr(str1, tokens, varNames, lineId,err) ) {
            return(0) ;    
         }
         if ( strcmp(tokens[cur],",") != 0 ) { 
            printf("%s", tokens[cur]);
            error(lineId) ;  
            *err =true;
            return(0) ;
         }
         cur++;
         if ( ! expr(str2, tokens, varNames, lineId,err) ) {
            return(0) ;    
         }
         if ( strcmp(tokens[cur],",") != 0 ) { 
            printf("%s", tokens[cur]);
            error(lineId) ; 
            *err =true;
            return(0) ;
         }
         cur++;
         if ( ! expr(str3, tokens, varNames, lineId,err) ) {
            return(0) ;    
         }
         if ( strcmp(tokens[cur],",") != 0 ) { 
            printf("%s", tokens[cur]);
            error(lineId) ; 
            *err =true;
            return(0) ;
         }
         cur++;
         if ( ! expr(str4, tokens, varNames, lineId,err) ) {
            return(0) ;    
         }
         if ( strcmp(tokens[cur],")") != 0 ) { 
            printf("%s", tokens[cur]);
            error(lineId) ; 
            *err =true;
            return(0) ;
         }
         strcat(str, str1);
         strcat(str, " | ");
         strcat(str, str2);
         strcat(str, " | ");
         strcat(str, str3);
         strcat(str, " | ");
         strcat(str, str4);
         strcat(str,tokens[cur]) ; 
         strcat(str," ") ; 
         
         cur++ ; 
         
         
         return(1) ; 
        }else{
         error(lineId) ; 
         *err =true;
         return 0;
      }
    }
    return(0) ; 
}

//looks morefactors to split it to factors and morefactors
int morefactors(char *str , char * tokens[], char * varNames[], int lineId, bool * err)
{
   char str1[N], str2[N], str3[N] ; 
   
   str1[0] = str2[0] = str3[0] = '\0' ; 
   
   if ( (strcmp(tokens[cur],"*") == 0 ) || (strcmp(tokens[cur],"/") == 0 ) ) {
       strcpy(str1,tokens[cur]) ; 
       int len = strlen(str1);
       str1[len] = '\0';
       strcat(str1," ") ; 
       cur++ ;  
       
       if (!  factor(str2,tokens, varNames, lineId,err)) {
          return(0) ; 
       } 
       if (!  morefactors(str3, tokens, varNames, lineId,err)) {
         return(0) ; 
       }
   } 
   strcat(str3,str1) ; 
   strcat(str2,str3) ; 
   strcpy(str,str2) ; 
   int len = strlen(str);
   str[len] = '\0';
   
   return(1) ;  
}

   
// checks the token whether it is transpose or sqrt
int isFunction(char * token) {
   if(strcmp(token, "tr")==0 || strcmp(token, "sqrt")==0 ){
      return 1;
   }else{
      return 0;
   }
}

int isChoose(char * token) {
   if(strcmp(token, "choose")==0 ){
      return 1;
   }else{
      return 0;
   }
}
int isVectorId(char * token) {
   if(strcmp(token, "id")==0 ){
      return 1;
   }else{
      return 0;
   }
}