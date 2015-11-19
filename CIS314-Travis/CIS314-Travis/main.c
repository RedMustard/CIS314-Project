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
    in_file = fopen("tests/bubble.asm", "r");
    
    // If 'in_file' hasn't been initialized, stop execution
    if (in_file == NULL) {
        printf("Error Reading File\n");
        exit (0);
    }
    
    fileProcess(in_file);     // Send 'in_file' to be processed
    
    return 0;
} // End main


void fileProcess(FILE*in_file) {
    char *labelArray[MAX_SIZE];     // Parsed lines from 'in_file' for use of parsing labels
    char *instArray[MAX_SIZE];      // Parsed lines from 'in_file' for use of parsing instructions
    char *regArray[MAX_SIZE];       // Parsed lines from 'in_file' for use of parsing registers/constants
    
    const size_t line_size = 301;   // Set buffer size limit for line lengths
    char *line = malloc(line_size);
    
    char *label;         // Token for labels
    char *instruction;   // Token for instructions
    char *reg;           // Token for registers/constants

    
    int i = 0;    // Counter for lines in 'in_file'
    int x = 0;    // Counter for array indexes
    
    // While a line exists in 'in_file', insert 'line' into 'lineArray[i]'
    while (fgets(line, line_size, in_file) != NULL)  {
        
        // If a character in 'line' is a tab, make it a space
        for (int l = 0; line[l] != NULL; l++) {
            if (line[l] == '\t') {
                line[l] = ' ';
            }
        }
        
        // Strip all lines that begin with '#', '\n', or that are empty.
        //  Add the rest to 'lineArray', 'instArray', and 'labelArray'
        if (line[0] != '#' && line[0] != '\n' && strlen(line) > 2) {
            instArray[i] = malloc(strlen(line));
            regArray[i] = malloc(strlen(line));
            labelArray[i] = malloc(strlen(line));
            strcpy(instArray[i], line);
            strcpy(regArray[i], line);
            strcpy(labelArray[i], line);
            i++;
        }
    } // End while
    
    free(line);         // Done with 'line' now, so deallocate the memory
    fclose(in_file);    // Done with 'in_file_ now, so close it
    
    // Tokenize labels and corresponding line numbers from 'labelArray'. Store in LABELS and LABELLINE
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
            }
        } // End for
    } // End for
    
    // Tokenize labels and instructions from 'instArray' and remove labels (only keep instructions).
    //  Tokenize registers/constants from 'regArray' and remove any comments (delimited by '#')
    //  Concatenate instructions and registers/constants into INSTRUCTIONS
    x = 0;
    for (int r = 0; r < i; r++) {
        instruction = strtok(instArray[r], " "); // Create token at first value (label or instruction)
        
        reg = strtok(regArray[r], " ");  // Create token another at first value
        reg = strtok(NULL, "#");         //   Then tokenize everything up to a comment (delimited by '#')
        
        // If a ':' exists in 'inst', replace 'instruction' with a space (Removes the labels)
        for (int s = 0; instruction[s] != NULL; s++) {
            if (instruction[s] == ':') {
                strcpy(instruction, "");
            }
        }
        
        strcat(instruction, " "); // Add a space to the end of 'inst'
        
        // If a character in 'reg' is a '\n' or ',' make it a space
        for (int s = 0; reg != NULL && reg[s] != NULL; s++) {
            if (reg[s] == '\n' || reg[s] == ',') {
                reg[s] = ' ';
            }
        }
        
        // Only add to INSTRUCTIONS if the line has
        if (reg != NULL) {
            INSTRUCTIONS[x] = malloc(30);
            strcat(INSTRUCTIONS[x], instruction);   // Add the instruction
            strcat(INSTRUCTIONS[x++], reg);  // Add the registers and constants
        }
            
    } // End for
    
    for (int s = 0; INSTRUCTIONS[s] != NULL; s++) {
        printf("%s\n", INSTRUCTIONS[s]);
    }
} // End fileParse