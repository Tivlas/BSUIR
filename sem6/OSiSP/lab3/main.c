#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "morse.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s input_file output_file\n", argv[0]);
        return 1;
    }

    char *inputFileName = argv[1];
    char *outputFileName = argv[2];

    FILE *inputFile = fopen(inputFileName, "r");
    if (inputFile == NULL) {
        printf("Error opening input file.\n");
        return 1;
    }

    FILE *outputFile = fopen(outputFileName, "w");
    if (outputFile == NULL) {
        printf("Error opening output file.\n");
        fclose(inputFile);
        return 1;
    }

    fseek(inputFile, 0, SEEK_END);
    long fileSize = ftell(inputFile);
    fseek(inputFile, 0, SEEK_SET);
   
    char *text = (char*) malloc(fileSize + 1);
    if (text == NULL) {
        printf("Error allocating memory.\n");
        fclose(inputFile);
        return 1;
    }
    
    size_t bytesRead = fread(text, 1, fileSize, inputFile);
    text[bytesRead] = '\0'; 

    size_t len = strlen(text);
    char* result = (char*)malloc((len * (MORSE_CHAR_MAX_LEN + 1) + 1) * sizeof(char));

    stringToMorse(text, result);
    fprintf(outputFile, "%s", result);
    fprintf(outputFile, "%s", "\n");

    fclose(inputFile);
    fclose(outputFile);
    free(result);
    free(text);

    printf("Done.\n");
    return 0;
}