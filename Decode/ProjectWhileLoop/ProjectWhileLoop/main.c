/*
 * Alex Geoffrey, Cody Ebert and Travis Barnes
 *
 * Alex: Friday 4pm Lab
 * Travis: Wednesday 3pm Lab
 * Cody: Wednesday 3pm Lab
 *
 * CIS 314 Group Project Fall 2015
 *
 *
 *
 * Copyright 2015
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAX_SIZE 201

// Forward-declaring functions:
void fileProcess(FILE*);
int ALU(int, int, char *);
int fetchDecode(int );
int registerWriteBack(int targetRegister, int value);
int memoryCommands(char * command, int targetRegister, int memoryIndex);

// Global Arrays
char *INSTRUCTIONS[MAX_SIZE]; // Processed instructions from 'in_file' (Up to 200 max instructions)
char *LABELS[20];             // Labels from processed instructions
int LABELLINE[20];            // Line number where the label was found

// static char *INSTRUCTIONS[5] ={"add $a0 $a1 5" , "sub $t0 $v1 6", "add $v1 $at 1", "sw $v1 4($at)", "j label"};
static char *registerArray[32] = {"$zero", "$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3", "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7", "$t8", "$t9", "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7", "$s8", "$k0", "$k1", "$gp", "$sp", "$ra"};
static int registerMemory[32] = {};	// Register data cache
static char *label;		// Current branch label (if beq/bne is called)
int mainMemory [300];
int length = sizeof(INSTRUCTIONS) / sizeof(INSTRUCTIONS[0]);
int next = 0;   // 0 = fetchDecode, 1 = ALU, 2 = regWriteBack, 3 = memCommands
char *command;                      // Ie. add, jal, beq, mult, etc. of instruction
int instLine = 0;
int dest = 0;
int arg1 = 0;
int arg2 = 0;
int offset = 0;
int result = 0;


int main(int argc, const char * argv[]) {
    char fileName[50];
    //Clock
    int clockCounter = 0;
    char * currentInstructions[5];
    
    // User input
    printf ("Enter the directory of the file you want to run (Default files in 'tests/fileName.asm'): ");
    scanf ("%s", fileName);
    FILE *in_file;
    in_file = fopen(fileName, "r");
    
    // If 'in_file' hasn't been initialized, stop execution
    if (in_file == NULL) {
        printf("Error Reading File\n");
        exit (0);
    }
    
    fileProcess(in_file);     // Send 'in_file' to be processed
    
    // Start fetchDecode / ALU Ops
    while (instLine < length) {
        if (next == 0) { fetchDecode(instLine); instLine++; }
        if (next == 1) { ALU(arg1, arg2, command); }
        if (next == 2) { memoryCommands(command, arg1, instLine); }
        if (next == 3) { registerWriteBack(arg1, result); }
        clockCounter++;
    }
    
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
                label = strtok(labelArray[p]," ");
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
        
        // Only add parsed reg/instruction to INSTRUCTIONS if there is relevant content (not just spaces)
        if (reg != NULL && strlen(reg) > 5) {
            INSTRUCTIONS[x] = malloc(30);
            strcat(INSTRUCTIONS[x], instruction);   // Add the instruction
            strcat(INSTRUCTIONS[x++], reg);  // Add the registers and constants
        }
        
    } // End for
} // End fileParse

    
int fetchDecode(int instLine) {
        char *arg;							// Token for strtok()
        char *instruction;					// Copy of instruction
        int i = 0;
        
        instruction = (char *) malloc(32);
        strcpy(instruction, INSTRUCTIONS[instLine]);	// Make copy of current instruction
        command = strtok(instruction, " ");		// Get command argument from instruction (add, jal, etc.)
        printf("Current command: %s\n", command);
        
        // beq and bne: $s, $t, offset
        if (   strcmp(command, "beq") == 0
            || strcmp(command, "bne") == 0) {
            arg = strtok(NULL, " ");
            for (i = 0; i < 32; i++) {			// Find register value for 1st arg
                if (strcmp(arg, registerArray[i]) == 0) {
                    arg1 = registerMemory[i];
                    break;
                }
            }
            arg = strtok(NULL, " ");
            for (i = 0; i < 32; i++) {			// Find register value for 2nd arg
                if (strcmp(arg, registerArray[i]) == 0) {
                    arg2 = registerMemory[i];
                    break;
                }
            }
            label = (strtok(NULL, " "));	// Set global branch label
            next = 1;
        }
        
        // add, sub, mult, div, slt: $dest, $arg1, $arg2
        if (   strcmp(command, "add") == 0
            || strcmp(command, "addi") == 0
            || strcmp(command, "sub") == 0
            || strcmp(command, "subi") == 0
            || strcmp(command, "mult")== 0
            || strcmp(command, "div") == 0
            || strcmp(command, "slt") == 0
            || strcmp(command, "sltu") == 0 )
        {
            arg = strtok(NULL, " ");
            for (i = 0; i < 32; i++) {			// Get value stored in first register
                if (strcmp(arg, registerArray[i]) == 0) {
                    dest = i;
                    break;
                }
            }
            arg = strtok(NULL, " ");
            for (i = 0; i < 32; i++) {			// Get value stored in first register
                if (strcmp(arg, registerArray[i]) == 0) {
                    arg1 = registerMemory[i];
                    break;
                }
            }
            arg = strtok(NULL, " ");
            for (i = 0; i < 32; i++) {			// Get value stored in second register
                if (strcmp(arg, registerArray[i]) == 0) {
                    arg2 = registerMemory[i];
                    break;
                }
            }
            if (i == 32) { arg2 = atoi(arg); }	// If arg was immediate (not register), arg2 = immediate value
            next = 1;
        }
        
        // sw and lw: $t, offset($s)
        if (   strcmp(command, "sw") == 0
            || strcmp(command, "lw") == 0 )
        {
            arg = strtok(NULL, " ");
            for (i = 0; i < 32; i++) {			// Get register INDEX
                if (strcmp(arg, registerArray[i]) == 0) {
                    arg1 = i;
                    break;
                }
            }
            offset = atoi( strtok(NULL, "(") );	// Offset
            arg = strtok(NULL, ")");
            for (i = 0; i < 32; i++) {					// Get address from 2nd register, and add offset
                if (strcmp(arg, registerArray[i]) == 0) {
                    arg2 = registerMemory[i] + offset;
                    break;
                }
            }
            next = 2;
        }
        
        // j, jal: target
        if (   strcmp(command, "j")   == 0
            || strcmp(command, "jal") == 0
            || strcmp(command, "jr")  == 0 )
        {
            label = strtok(NULL, " ");	// Get label for if branch is true, save globally
            next = 2;
        }
    
        if ( strcmp(command, "slt") == 0 )
        {
            arg = strtok(NULL, " ");
            for (i = 0; i < 32; i++) {			// Get value stored in first register
                if (strcmp(arg, registerArray[i]) == 0) {
                    dest = i;
                    break;
                }
            }
            arg = strtok(NULL, " ");
            for (i = 0; i < 32; i++) {			// Get value stored in first register
                if (strcmp(arg, registerArray[i]) == 0) {
                    arg1 = registerMemory[i];
                    break;
                }
            }
            arg = strtok(NULL, " ");
            for (i = 0; i < 32; i++) {			// Get value stored in second register
                if (strcmp(arg, registerArray[i]) == 0) {
                    arg2 = registerMemory[i];
                    break;
                }
            }
            if (i == 32) { arg2 = atoi(arg); }	// If arg was immediate (not register), arg2 = immediate value
            next = 1;
        }
    
        free(instruction);
        return 0;
    }


    int ALU(int arg1, int arg2, char * command)
    {
        //Add
        if(strcmp(command, "add") == 0 || strcmp(command, "addi") == 0)
        {
            result = arg1 + arg2;
            next = 3;
            return 0;
        }
        //Sub
        else if(strcmp(command, "sub") == 0 || strcmp(command, "subi") == 0)
        {
            result = arg1 - arg2;
            next = 3;
            return 0;
        }
        //Multiplication
        else if( strcmp(command, "mult") == 0 || strcmp(command, "multi") == 0)
        {
            result = arg1 * arg2;
            next = 3;
            return 0;
        }
        //Division
        else if(strcmp(command, "div") == 0 || strcmp(command, "divi") == 0)
        {
            result = arg1/arg2;
            next = 3;
            return 0;
        }
        //Branch if equal
        else if(strcmp(command, "beq") == 0 )
        {
            if(arg1 == arg2)
            {
                //Loop through labelArray to find proper label
                for(int i = 0; i < 10; i++)
                {   //Finds correct label, and then runs program starting at given index
                    if(strcmp(LABELS[i], label) == 0)
                    {
                        instLine = LABELLINE[i];
                        next = 0;
                        return 1;
                    }
                    
                }
                printf("ERROR LABEL NOT FOUND");
                return 1;
            }
            else if(arg1 != arg2)
            {
                next = 0;
                return 0;
            }
        }
        //Branch if not equal
        else if(strcmp(command, "bne") == 0)
        {
            if(arg1 == arg2)
            {
                next = 0;
                return 0;
            }
            else if(arg1 != arg2)
            {
                //Loop through labelArray to find proper label
                for(int i = 0; i < 10; i++)
                {   //Finds correct label, and then runs program starting at given index
                    if(strcmp(LABELS[i], label) == 0)
                    {
                        instLine = LABELLINE[i];
                        next = 1;
                        return 1;
                    }
                    
                }
                printf("ERROR LABEL NOT FOUND");
                return 1;
            }
        }
        //Set less than commands
        else if (strcmp(command, "slt") == 0 || strcmp(command, "sltu") == 0 || strcmp(command, "slti"))
        {
            if(arg1 < arg2)
            {
                registerMemory[dest] = 1;
                next = 0;
                return 0;
            }
            registerMemory[dest] = 0;
            next = 0;
            return 0;
        }
        return 1;
    }
    //SW/LW
    int memoryCommands(char * command, int targetRegister, int memoryIndex)
    {
        if(strcmp(command, "sw") == 0)
        {
            mainMemory[arg2] = registerMemory[arg1];
        }
        else if(strcmp(command, "lw") == 0)
        {
            registerMemory[arg1] =  mainMemory[arg2];
        }
        else if(strcmp(command, "j") == 0) {
            for(int i = 0; i < 10; i++)
            {   //Finds correct label, and then runs program starting at given index
                if(strcmp(LABELS[i], label) == 0)
                {
                    instLine = LABELLINE[i];
                    next = 0;
                    return 1;
                }
                
            }
            printf("ERROR LABEL NOT FOUND");
            return 1;
        }
        else if(strcmp(command, "jal")==0)
        {
            for(int i = 0; i < 10; i++)
            {   //Finds correct label, and then runs program starting at given index
                if(strcmp(LABELS[i], label) == 0)
                {
                    instLine = LABELLINE[i];
                    registerMemory[31] = instLine;
                    next = 0;
                    break;
                }
                
            }
        }
        else if(strcmp(command, "jr") == 0)
        {
            instLine = registerMemory[31]+1;
        }
        next = 0;
        return 0;
    }
    int registerWriteBack(int targetRegister, int value)
    {
        registerMemory[dest] = value;
        next = 0;
        return 0;
    }
    
