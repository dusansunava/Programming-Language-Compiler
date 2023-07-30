#include <unistd.h>
#include <stdio.h>
#include "tree.h"
#include "./Grammar/grammar.tab.c"

void proceed(tree *t, int currentMode, FILE * fp, char *outputFileName);

int main(int argc, char *argv[]) {

    int opt;
    int fileCheck = 0;
    int outputFileCheck = 0;
    int currentMode = 1; //default mode, if not specified, compiler will make binary code
    int i;
    char fileName[100];
    char outputFileName[100];
    while((opt = getopt(argc, argv, ":f:tbv:o:")) != -1) {
        switch(opt) {
            case 'f':
                //get input file name
                fileCheck = 1;
                for(i = 0; i < strlen(optarg); i++){
                    fileName[i] = optarg[i];
                }
                fileName[i++] = '\0';
                break;
            case 'b':
                //binary
                currentMode = 1;
                break;
            case 't':
                //text
                currentMode = 2;
                break;
            case 'o':
                //output file name
                outputFileCheck = 1;
                for(i = 0; i < strlen(optarg); i++){
                    outputFileName[i] = optarg[i];
                }
                outputFileName[i++] = '\0';
                break;
        }
    }

    //open output file name with given name if is specified
    FILE *outputFile;
    if(outputFileCheck == 1) {
        if(outputFileName[0] == ' ') {
            printf("Give output file name as argument when using -o");
            return 1;
        } else {
            outputFile = fopen(outputFileName, "wb");
        }
    }

    //output input file name
    if(fileCheck == 0) {
        printf("Give file name as argument with -f switch");
        return 1;
    } else {
        FILE *file = fopen(fileName, "r");
        if(file == NULL) {
            printf("There is no file named %s", fileName);
            return 1;
        }
        if(outputFileCheck == 0) {
            strcpy(outputFileName, "a.txt");
            outputFile = fopen("a.txt", "wb");
        }
        fseek(file, 0L, SEEK_END);
        int size = ftell(file);
        rewind(file);
        char buffer[size];
        char c;
        int i = 0;
        do {
            c = fgetc(file);
            buffer[i] = c;
            i++;
        } while (c != EOF);

        //pass input string to parser
        yy_scan_string(buffer);

        //run parser
        yyparse(currentMode, outputFile);

        fclose(outputFile);
        printf("Code from ,,%s'' successfuly saved in folder ,,%s''", fileName, outputFileName);
        return 0;
    }
}



