#include <stdio.h>
#include <string.h>

int main (int argc,char *argv[]) {

    FILE *fp;
    char line[256];
    // array to store splitted line
    char *splittedLines[256][256];

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
    int tokenId = 0;
    int lineId = 0;
    //reads input from file
    while( fgets(line,256,fp) != NULL ) {
        // splits the tokens that includes space between them

        char *token = strtok(line, " ");
        while( token != NULL ) {
            //stores each token in two dimensional array
            /*
                burası tam çalışmıyor. stringleri tutmak için char pointer array kullandım ama
                muhtemelen tokenın değeri değişince pointer array de tokenı tuttuğu için arraydeki
                tüm elemanlar son okunan elemana eşitleniyor.

                mesela son satır matrix B[1] ise splittedLine[0][0] da splittedLine[10][0] da matrix;
                splittedLine[0][1] de splittedLine[10][1] de B[1] değerini tutuyor.
            */
            splittedLines[lineId][tokenId] = token; 
            tokenId += 1;;  
            token = strtok(NULL, " ");
        }
        lineId += 1;
        tokenId = 0;
    }
    fclose(fp);
    
    printf("%s", splittedLines[5][1]);

return(0);

}