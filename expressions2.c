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

int  expr(char *, char**, char**) ;
int  term(char *, char**, char**) ;
int  moreterms(char *, char**, char**) ;
int  factor(char *, char**, char**) ;
int  morefactors(char *, char**, char**) ;
int  is_integer(char *) ; 
int isFunction(char * token);
int isChoose(char * token);
int isVectorId(char * token);


   int     numtokens ; 
   char    str[N]   ; 
   
  


int expr(char *str, char * tokens[], char * varNames[])
{
   char str1[N], str2[N] ; 
   
   str1[0] = str2[0] = '\0' ; 
   if (!  term(str1 , tokens, varNames)) {
      return(0) ; 
   } 
   if (!  moreterms(str2 , tokens, varNames)) {
      return(0) ; 
   }
   
   strcat(str1,str2) ; 
   strcpy(str,str1) ; 
   return(1) ;  
}

int term(char *str, char * tokens[], char * varNames[])
{
   char str1[N], str2[N] ; 
   
   str1[0] = str2[0] = '\0' ; 
   if (!  factor(str1 , tokens, varNames)) {
      return(0) ; 
   } 
   if (!  morefactors(str2 , tokens, varNames)) {
      return(0) ; 
   }

   strcat(str1,str2) ; 
   strcpy(str,str1) ; 
   return(1) ;  
}

int moreterms(char *str , char * tokens[], char * varNames[])
{
   char str1[N], str2[N], str3[N] ; 
   
   str1[0] = str2[0] = str3[0] = '\0' ; 
   
   if ( (strcmp(tokens[cur],"+") == 0 ) || (strcmp(tokens[cur],"-") == 0 ) ) {
       strcpy(str1,tokens[cur]) ; 
       strcat(str1," ") ; 
       cur++ ;  
       if (!  term(str2, tokens, varNames)) {
          return(0) ; 
       } 
       if (!  moreterms(str3, tokens, varNames)) {
         return(0) ; 
       }
   } 
   //printf("term %s \n", str2);
   //printf("more term %s \n", str3);
   //printf("token %s \n", str1);
   strcat(str2,str1) ; 
   strcat(str3,str2) ; 
   strcpy(str,str3) ; 
   return(1) ;  
}

int factor(char *str , char* tokens[], char * varNames[])
{
   char str1[N] ; 
   char str2[N] ; 
   char str3[N] ; 
   char str4[N] ; 
   
   str1[0] = '\0' ; 
   str2[0] = '\0' ;
   str3[0] = '\0' ;
   str4[0] = '\0' ;

    printf("%s\n",tokens[cur]) ; 
    if ( isInteger(tokens[cur])  ) {
       strcat(str,tokens[cur]) ; 
       strcat(str," ") ; 
       cur++ ; 
       return(1) ; 
    } 
    if ( isVariable(tokens[cur], varNames)  ) {
       strcat(str,tokens[cur]) ; 
       strcat(str," ") ; 
       cur++ ; 
       printf("%s \n", tokens[cur]);
       if ( strcmp(tokens[cur],"[") == 0 ) {
         strcat(str,tokens[cur]) ; 
         strcat(str," ") ; 
         cur++ ; 
         if ( ! expr(str1, tokens, varNames) ) {
            return(0) ;    
         }
   
         strcat(str,str1) ; 
         strcat(str," ") ; 
         
         if ( strcmp(tokens[cur],",") == 0 ) { 
            strcat(str,"@") ; 
            strcat(str," ") ; 
            cur ++;
            if ( ! expr(str2, tokens, varNames) ) {
            return(0) ;    
            }
            strcat(str,str2) ; 
            strcat(str," ") ;
            if ( strcmp(tokens[cur],"]") != 0 ) { 
               printf("%s", tokens[cur]);
               printf("Error: expecting paranthesis\n") ; 
               return(0) ;
            }
            
         }else{
            strcat(str,"@") ; 
            strcat(str," ") ; 
            strcat(str,"1") ; 
            if ( strcmp(tokens[cur],"]") != 0 ) { 
               printf("%s", tokens[cur]);
               printf("Error: expecting paranthesis\n") ; 
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
       if ( ! expr(str1, tokens, varNames) ) {
          return(0) ;    
       }
       if ( strcmp(tokens[cur],")") != 0 ) { 
          printf("%s", tokens[cur]);
          printf("Error: expecting paranthesis\n") ; 
          return(0) ;
       }
       cur++ ; 
       strcpy(str,str1) ; 
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
         if ( ! expr(str1, tokens, varNames) ) {
            return(0) ;    
         }
         if ( strcmp(tokens[cur],")") != 0 ) { 
            printf("%s", tokens[cur]);
            printf("Error: expecting paranthesis\n") ; 
            return(0) ;
         }
         strcat(str, str1);
         strcat(str,tokens[cur]) ; 
         strcat(str," ") ; 
         
         cur++ ; 
         
         
         return(1) ; 
        }else{
         printf("Error");
         return 0;
      }
    }
    if(isChoose(tokens[cur])){
        
       strcat(str,tokens[cur]) ; 
       strcat(str," ") ; 
       cur ++;
       if ( strcmp(tokens[cur],"(") == 0 ) {
         strcat(str,tokens[cur]) ; 
         strcat(str," ") ; 
         cur++ ; 
         if ( ! expr(str1, tokens, varNames) ) {
            return(0) ;    
         }
         if ( strcmp(tokens[cur],",") != 0 ) { 
            printf("%s", tokens[cur]);
            printf("Error: expecting comma\n") ; 
            return(0) ;
         }
         cur++;
         if ( ! expr(str2, tokens, varNames) ) {
            return(0) ;    
         }
         if ( strcmp(tokens[cur],",") != 0 ) { 
            printf("%s", tokens[cur]);
            printf("Error: expecting comma\n") ; 
            return(0) ;
         }
         cur++;
         if ( ! expr(str3, tokens, varNames) ) {
            return(0) ;    
         }
         if ( strcmp(tokens[cur],",") != 0 ) { 
            printf("%s", tokens[cur]);
            printf("Error: expecting comma\n") ; 
            return(0) ;
         }
         cur++;
         if ( ! expr(str4, tokens, varNames) ) {
            return(0) ;    
         }
         if ( strcmp(tokens[cur],")") != 0 ) { 
            printf("%s", tokens[cur]);
            printf("Error: expecting paranthesis\n") ; 
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
         printf("Error 10");
         return 0;
      }
    }
    printf("Error: expecting factor\n") ; 
    return(0) ; 
}


int morefactors(char *str , char * tokens[], char * varNames[])
{
   char str1[N], str2[N], str3[N] ; 
   
   str1[0] = str2[0] = str3[0] = '\0' ; 
   
   if ( (strcmp(tokens[cur],"*") == 0 ) || (strcmp(tokens[cur],"/") == 0 ) ) {
       strcpy(str1,tokens[cur]) ; 
       strcat(str1," ") ; 
       cur++ ;  
       
       if (!  factor(str2,tokens, varNames)) {
          return(0) ; 
       } 
       if (!  morefactors(str3, tokens, varNames)) {
         return(0) ; 
       }
   } 
   strcat(str3,str1) ; 
   strcat(str2,str3) ; 
   strcpy(str,str2) ; 
   return(1) ;  
}


int is_integer(char *token) 
{
    int isnumber = 1 ;
    char *q ; 

    for(q = token ; *q != '\0' ; q++) {
        isnumber = isnumber && isdigit(*q) ;
    }

    return(isnumber) ; 
}    

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