//  Read, process, and store MIPS instructions from user specified .asm file into array.
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

char *INSTARRAY[MAX_SIZE]; // Processed instructions from read file
char *LABELARRAY[10];       // Labels from processed instructions
int LABELLINE[10];         // Line number where the label was found

void fileProcess(FILE*);

int main(int argc, const char * argv[]) {
    FILE *in_file;
    in_file = fopen("tests/bubble.asm", "r");
    
    // If 'in_file' hasn't been initialized, stop execution
    if (in_file == NULL) {
        printf("Error Reading File\n");
        exit (0);
    }
    
    fileProcess(in_file); // Process 'in_file'
    
    return 0;
} // End main

void fileProcess(FILE*in_file) {
    char* lineArray[MAX_SIZE]; // Parsed lines from 'in_file'
    const size_t line_size = 300;   // Set buffer size limit for line lengths
    char* line = malloc(line_size);
    
    char* label;
    char* inst;
    char* reg0;
    char* reg1;
    char* reg2;
    
    int i = 0; // Counter
    int x = 0; // Counter
    
    // While a line exists in 'in_file', insert 'line' into 'lineArray[i]'
    while (fgets(line, line_size, in_file) != NULL)  {
        lineArray[i] = malloc(strlen(line));
        
        // Strip all lines that begin with #. Add the rest to 'lineArray'
        if (line[0] != '#') {
            lineArray[i] = malloc(strlen(line));
            strcpy(lineArray[i], line);
            i++;
        } else {
            
        }
        
    } // End while
    
    free(line);         // Deallocate memory used by 'line'
    fclose(in_file);    // Close 'in_file'
    
    
/////////////// PRINT ARRAY /////////////
    for (int q = 0; q < i; q++) {
        printf("%s", lineArray[q]);
    }
////////////////////////////////////////
    
    // Strip line in 'lineArray', located all label names/line numbers and store in respective arrays
//    for (int p = 1; p < i; p++) {
//        label = strtok(lineArray[p]," ");
//        
//        for (int j = 0; j < strlen(label); j++) {
//            if (label[j] == ':') {
//                LABELARRAY[x] = malloc(strlen(label));
//                strcpy(LABELARRAY[x], label);
//                LABELLINE[x++] = p-1;
//            }
//        } // End for
//    } // End for
/////////////////////////////////
/////////LAST WORKING ON//////////
//    for (int p = 1; p < i; p++) {
//        inst = strtok(lineArray[p],",");
//    
//        while (inst != NULL) {
//            printf("%s\n", inst);
//            inst = strtok(NULL, ",");
//
//        }
//    }
/////////////////////////////////////

        
//        for (int j = 0; j < strlen(label); j++) {
//            if (label[j] == ':') {
//                LABELARRAY[x] = malloc(strlen(label));
//                strcpy(LABELARRAY[x], label);
//                LABELLINE[x++] = p-1;
//            }
//        } // End for
//                printf("%s\n", inst[1]);
//    } // End for

    
    
    
/////////////// PRINT LABELARRAY /////////////
//    for (int q = 0; LABELARRAY[q] != NULL; q++) {
//        printf("%s", LABELARRAY[q]);
//        printf("%d\n", LABELLINE[q]);
//    }
//////////////////////////////////////////////
/////////////// PRINT LABELARRAY /////////////
//    for (int q = 0; LABELLINE[q] != NULL; q++) {
//        printf("%d\n", LABELLINE[q]);
//    }
//////////////////////////////////////////////
    
} // End fileParse








/////////////// Strips all lines with # located anywhere
//        for (int j = 0; j < strlen(line); j++) {
//            if (line[j] == '#') {
//                x = 1; // Raise flag
//            }
//        }
//
//        // If flag isn't raised, add line to array
//        if (x == 0) {
//            lineArray[i] = malloc(strlen(line));
//            strcpy(lineArray[i], line);
//            i++;
//        }
///////////////////////////////////////////////////////////





//////////////// Skips #s ////////////////////
//    for (int q = 0; q < i; q++) {
//        x=0;
//        while(lineArray[q][x] != '#' && lineArray[q] != NULL) {
////            printf("%c", lineArray[q][x]);
//            INSTARRAY[j] = malloc(strlen(lineArray[q]));
//            strcpy(INSTARRAY[j], lineArray[q]);
//            x++;
//
//        }
//        j++;
////        printf("\n");
//    }
/////////////////////////////////////////////////
    
    
////////////////PRINT INSTARRAY////////////
//    for (int r = 0; r < MAX_SIZE; r++) {
//        if (INSTARRAY[r] != NULL) {
//            printf("%s", INSTARRAY[r]);
//        }
//    }
///////////////////////////////////////////
