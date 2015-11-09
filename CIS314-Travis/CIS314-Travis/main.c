//  Read, parse, and store MIPS instructions from user specified .asm file into array.
//
//  main.c
//  CIS314-Travis
//
//  Created by Travis Barnes on 11/8/15.
//  Copyright (c) 2015 Travis Barnes. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 100

char *INSTARRAY[MAX_SIZE];
char LABELARRAY[10];
int LABELLINE[10];

int main(int argc, const char * argv[]) {
    FILE *in_file;
    in_file = fopen("tests/bubble.asm", "r");
    
    const size_t line_size = 300;
    char* line = malloc(line_size);
    char* lineArray[MAX_SIZE];
    
//    size_t len = 0;
//    ssize_t read;
    int i = 0;
    
//     If the file hasn't been initialized, stop execution
    if (in_file == NULL) {
        printf("Error Reading File\n");
        exit (0);
    }
    

    while (fgets(line, line_size, in_file) != NULL)  {
        lineArray[i] = malloc(strlen(line));
        strcpy(lineArray[i], line);
        i++;
    }
//    free(line);
    
    for (int q = 0; q < MAX_SIZE; q++) {
        printf("%s", lineArray[q]);
    }

    return 0;
    
}