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

#define MAX_SIZE 201

char *INSTRUCTIONS[MAX_SIZE]; // Processed instructions from 'in_file' (Up to 200 max instructions)
char *LABELS[20];             // Labels from processed instructions
int LABELLINE[20];            // Line number where the label was found

void fileProcess(FILE*);

int main(int argc, const char * argv[]) {
    FILE *in_file;
    in_file = fopen("tests/function.asm", "r");
    
    // If 'in_file' hasn't been initialized, stop execution
    if (in_file == NULL) {
        printf("Error Reading File\n");
        exit (0);
    }
    
    fileProcess(in_file);     // Send 'in_file' to be processed
    
    return 0;
} // End main


void fileProcess(FILE*in_file) {
    char *lineArray[MAX_SIZE];      // Parsed lines from 'in_file' for use of parsing instructions
    char *labelArray[MAX_SIZE];     // Parsed lines from 'in_file' for use of parsing labels
    
    const size_t line_size = 301;   // Set buffer size limit for line lengths
    char *line = malloc(line_size);
    
    char *label;  // Token for labels
    char *inst;   // Token for instructions
    char *inst2;
        char *lab2;
    
    int i = 0;    // Counter
    int x = 0;    // Counter
    
//////////////////////// WORKING - STRIPS INITIAL # LINES /////////////////////////
    // While a line exists in 'in_file', insert 'line' into 'lineArray[i]'
    while (fgets(line, line_size, in_file) != NULL)  {
        
        // If a character in 'line' is a tab, make it a space
        for (int l = 0; line[l] != NULL; l++) {
            if (line[l] == '\t') {
                line[l] = ' ';
            }
        }
        
        // Strip all lines that begin with #. Add the rest to 'lineArray'
        if (line[0] != '#' && line[0] != '\n' && strlen(line) > 2) {
            lineArray[i] = malloc(strlen(line));
            labelArray[i] = malloc(strlen(line));
            strcpy(lineArray[i], line);
            strcpy(labelArray[i], line);
            i++;
        }
    } // End while
    
    free(line);         // Done with 'line' now, so deallocate the memory
    fclose(in_file);    // Done with 'in_file_ now, so close it
/////////////////////////////////////////////////////////////////////////////////////

    ////////////////////// PRINT LINEARRAY /////////////////////////
    for (int q = 1; q < i; q++) {
//                printf("%s", lineArray[q]);
    }
    ////////////////////////////////////////////////////////////////
    
    
//////////////////// WORKING - LABEL/LINE NUMBER STRIP ////////////////////////////////
    // Strip labels and corresponding line numbers from 'labelArray'. Store in LABELS and LABELLINE
    for (int p = 0; p < i; p++) {
        label = strtok(labelArray[p]," ");
        
        // Disregard the label/line if it's NULL or blank
        if (label == NULL || strlen(label) <= 1) {
            continue;
        }
        
        // If the label has a ':' in it, add to LABELS and line number to LABELLINE
        for (int j = 0; label[j] != NULL; j++) {
            if (label[j] == ':') {
                label[j] = ' ';
                LABELS[x] = malloc(strlen(label));
                strcpy(LABELS[x], label);
                LABELLINE[x++] = p;
//                printf("%s ", label);
//                printf("%d\n", p);
            }
        } // End for
//        printf("%s\n", label);
    } // End for
///////////////////////////////////////////////////////////////////////////////////////////////////////
    
////////////////////// PRINT LINEARRAY /////////////////////////
    for (int q = 1; q < i; q++) {
//        printf("%s\n", lineArray[q]);
    }
////////////////////////////////////////////////////////////////
    

///////// Removes all inline comments that start with #//////////
    x = 0;
    for (int r = 0; r < i; r++) {
        lab2 = strtok(lineArray[r], " ");
        inst = strtok(lineArray[r], ":");
        
//        lab2 = strtok(NULL,"#");
//        printf("LABEL %s\n", lab2);
//        printf("INST %s\n", inst);
        
        if (inst != NULL) {
            inst2 = strtok(inst, "#");
            
//            INSTRUCTIONS[x] = malloc(30);
//            strcpy(INSTRUCTIONS[x], inst);
//            printf("%s\n", lab2);
            printf("%s\n", inst2);
            x++;
        }
//        free(inst);
    }
////////////////////////////////////////////////////////////////

////////////////////// PRINT ARRAYS ////////////////////////////
    for (int q = 1; q < i; q++) {
//        printf("%s\n", lineArray[q]);
    }
//    printf("\n");
    for (int q = 0; LABELS[q] != NULL; q++) {
//        printf("%s ", LABELS[q]);
//        printf("%d\n", LABELLINE[q]);
//
    }
//    printf("\n");
    for (int q = 0; INSTRUCTIONS[q] != NULL; q++) {
//        printf("%s\n", INSTRUCTIONS[q]);
    }
///////////////////////////////////////////////////////////////
} // End fileParse