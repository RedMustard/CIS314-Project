/*
 * Alex Geoffrey, Cody Ebert and Travis Barnes
 *
 * Alex: Friday 4pm Lab
 * Travis: Wednesday 3pm Lab
 * Cody: Wednesday 3pm Lab
 *
 * CIS 314 Group Project Fall 2015
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
void fetch(int, int);
void decode(int);
int registerWriteBack(int targetRegister, int value);
int memoryCommands(char * command, int targetRegister, int memoryIndex);

// Global Arrays
char *INSTRUCTIONS[MAX_SIZE]; // Processed instructions from 'in_file' (Up to 200 max instructions)
char *LABELS[20];             // Labels from processed instructions
int LABELLINE[20];            // Line number where the label was found
// static char *INSTRUCTIONS[5] ={"add $a0 $a1 5" , "sub $t0 $v1 6", "add $v1 $at 1", "sw $v1 4($at)", "j label"};
static char *registerArray[32] = {"$zero", "$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3", "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7", "$t8", "$t9", "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7", "$s8", "$k0", "$k1", "$gp", "$sp", "$ra"};
static int registerMemory[33] = {};	// Register data cache
static char *label;		// Current branch label (if beq/bne is called)
int mainMemory [300];
int length = sizeof(INSTRUCTIONS) / sizeof(INSTRUCTIONS[0]);
int next = 0;       // 0 = fetch 1 = Decode, 2 = ALU, 3 = regWriteBack, 4 = memCommands
char *command[5];   // Ie. add, jal, beq, mult, etc. of instruction
int instLine = 0;   // Index counter for accessing INSTRUCTIONS
int dest[5] = {};   // Destination register
int arg1[5] = {};   // Register argument 1
int arg2[5] = {};   // Register argument 2
int offset = 0;
int result[5] = {};
int total = 0;
int num = 0;        // Pipeline instruction counter
int currentInstructions[5] = {-1,-1,-1,-1,-1};

int main(int argc, const char * argv[]) {
    char fileName[50];
    
    //Counts each clock cycle
    int clockCounter = 0;
    
    //Holds current instructions
    //***Need to write population code***
    
    int nextStage[5] = {};
    //Indexes current commands
    int next = 0;
    
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
    
  
    //***Change bounds on loop***
    
    while(instLine < length)
    {
        //Make while, use head and tail in the array/iterator/counter
        //Loops through the current instructions being worked on in this iteration
        for(int i = 0; i < 5 ; i++) {
            next = nextStage[num];
            // Start fetchDecode / ALU Ops

            //If registers are in use, send a bubble
           if(arg1[num] == dest[(total-1)%5]
              || arg1[num] == dest[(total-2)%5]
              || arg2[num] == dest[(total-1)%5]
              || arg2[num] == dest[(total-2)%5])
           {
                fetch(-1, num);
           }
            
            //Otherwise execute normally
           else {
                //Call depending on stage, num is the current instruction being worked on
               if (next == 0) {
                   //***I dont know if these are the correct arguments or not***
                   fetch(instLine, num);

                } else if(next == 1) {
                    decode(currentInstructions[num]);
                    
                } else if (next == 2) {
                    ALU(arg1[num], arg2[num], command[num]);
                    
                } else if (next == 3) {
                    memoryCommands(command[num], arg1[num], instLine);
                    
                } else if (next == 4) {
                    registerWriteBack(arg1[num], result);
                }
                
                //Increments the stage of the current instruction
                nextStage[num] = nextStage[num] +1;
                //The number of stages that have been executed, ever
                total++;
                //Unifying index for all current instructions and corresponding arrays
                num = total%5;
                printf("%d", clockCounter);
               
            } // End else
        } // End for
        
        //Increment again to start at proper index for next iteration
        total++;
        num = total%5;
        instLine++;
        //Increment counter
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


void fetch(int instLine, int num) {
    char *instruction;                          // Copy of instruction
    
    if(instLine == -1) {
        currentInstructions[num] = -1;
    } else {
        instruction = (char *) malloc(32);              // Allocate space for instruction
        strcpy(instruction, INSTRUCTIONS[instLine]);    // Make copy of current instruction
        command[num] = strtok(instruction, " ");             // Get command argument from instruction (add, jal, etc.)
        printf("Current command: %c\n", *command[num]);
    
        free(instruction);
    }
    next++;
}


void decode(int instLine)
{
    char *arg;                            // Token for strtok()
    int i = 0;
    if(instLine != -1)
    {
        // beq and bne: $s, $t, offset
        if (   strcmp(command, "beq") == 0
            || strcmp(command, "bne") == 0) {
            arg = strtok(NULL, " ");
            for (i = 0; i < 32; i++) {			// Find register value for 1st arg
                if (strcmp(arg, registerArray[i]) == 0) {
                    arg1[num] = registerMemory[i];
                    break;
                }
            }
            arg = strtok(NULL, " ");
            if (strcmp(arg, "$0") == 0) {       // If $zero is written as $0... >:(
                arg2[num] = 0;
                label = (strtok(NULL, " "));	// Set global branch label
//                return 0;
            }
            for (i = 0; i < 32; i++) {			// Find register value for 2nd arg
                if (strcmp(arg, registerArray[i]) == 0) {
                    arg2[num] = registerMemory[i];
                    break;
                }
            }
            label = (strtok(NULL, " "));	// Set global branch label
        }
        // add, sub, mult, div, slt: $dest, $arg1, $arg2
        if (   strcmp(command[num], "add") == 0
            || strcmp(command[num], "addi") == 0
            || strcmp(command[num], "sub") == 0
            || strcmp(command[num], "subi") == 0
            || strcmp(command[num], "mult")== 0
            || strcmp(command[num], "div") == 0 )
        {
            arg = strtok(NULL, " ");
            for (i = 0; i < 32; i++) {			// Get value stored in first register
                if (strcmp(arg, registerArray[i]) == 0) {
                    dest[num] = i;
                    break;
                }
            }
            arg = strtok(NULL, " ");
            for (i = 0; i < 32; i++) {			// Get value stored in first register
                if (strcmp(arg, registerArray[i]) == 0) {
                    arg1[num] = registerMemory[i];
                    break;
                }
            }
            if (strcmp(arg, "$0") == 0 ) { arg1[num] = 0; }
            arg = strtok(NULL, " ");
            for (i = 0; i < 32; i++) {			// Get value stored in second register
                if (strcmp(arg, registerArray[i]) == 0) {
                    arg2[num] = registerMemory[i];
                    break;
                }
            }
            
            if (strcmp(arg, "$0") == 0 ) {
                arg2[num] = 0;
                i = 0;
            }
            
            if (i == 32) { arg2[num] = atoi(arg); }	// If arg was immediate (not register), arg2 = immediate value
        }
        
        // sw and lw: $t, offset($s)
        if (   strcmp(command[num], "sw") == 0
            || strcmp(command[num], "lw") == 0 )
        {
            arg = strtok(NULL, " ");
            for (i = 0; i < 32; i++) {			// Get register INDEX
                if (strcmp(arg, registerArray[i]) == 0) {
                    arg1[num] = i;
                    break;
                }
            }
            offset = atoi( strtok(NULL, "(") );	// Offset
            arg = strtok(NULL, ")");
            for (i = 0; i < 32; i++) {					// Get address from 2nd register, and add offset
                if (strcmp(arg, registerArray[i]) == 0) {
                    arg2[num] = registerMemory[i] + offset;
                    break;
                }
            }
        }
        
        // j, jal: target
        if (   strcmp(command[num], "j")   == 0
            || strcmp(command[num], "jr")  == 0 )
        {
            label = strtok(NULL, " ");	// Get label for if branch is true, save globally
        }
        
        // jal: target
        if (   strcmp(command[num], "jal")   == 0) {
            label = strtok(NULL, " ");  // Get label for if branch is true, save globally
            arg1[num] = instLine;
        }
        
        if (  strcmp(command[num], "slt") == 0
            ||strcmp(command[num], "sltu") == 0 )
        {
            arg = strtok(NULL, " ");
            for (i = 0; i < 32; i++) {			// Get value stored in first register
                if (strcmp(arg, registerArray[i]) == 0) {
                    dest[num] = i;
                    break;
                }
            }

            arg = strtok(NULL, " ");
            for (i = 0; i < 32; i++) {            // Get value stored in first register
                if (strcmp(arg, registerArray[i]) == 0) {
                    arg1[num] = registerMemory[i];
                    break;
                }
            }
            arg = strtok(NULL, " ");
            for (i = 0; i < 32; i++) {            // Get value stored in second register
                if (strcmp(arg, registerArray[i]) == 0) {
                    arg2[num] = registerMemory[i];
                    break;
                }
            }
            if (i == 32) { arg2[num] = atoi(arg); }    // If arg was immediate (not register), arg2 = immediate value
            
        }
    }
    next++;
    
}


int ALU(int arg1, int arg2, char * command)
{
    if(currentInstructions[num] != -1)
    {
        //Add
        if(strcmp(command, "add") == 0 || strcmp(command, "addi") == 0)
        {
            result[num] = arg1 + arg2;
            next = 3;
            return 0;
        }
        //Sub
        else if(strcmp(command, "sub") == 0 || strcmp(command, "subi") == 0)
        {
            result[num] = arg1 - arg2;
            next = 3;
            return 0;
        }
        //Multiplication
        else if( strcmp(command, "mult") == 0 || strcmp(command, "multi") == 0)
        {
            result[num] = arg1 * arg2;
            next = 3;
            return 0;
        }
        //Division
        else if(strcmp(command, "div") == 0 || strcmp(command, "divi") == 0)
        {
            result[num] = arg1/arg2;
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
                
        //Set less than commands
        else if (strcmp(command, "slt") == 0 || strcmp(command, "sltu") == 0 || strcmp(command, "slti"))
        {
            if(arg1 < arg2)
            {
                registerMemory[dest[num]] = 1;
                next = 0;
                return 0;
            }
            registerMemory[dest[num]] = 0;
            next = 0;
            return 0;
        }
    }
    next++;
        return 1;
}


////SW/LW
int memoryCommands(char * command, int targetRegister, int memoryIndex)
        {
            if(strcmp(command, "sw") == 0)
            {
                int num = arg2[num]/4;
                mainMemory[index] = registerMemory[arg1[num]];
                
            }
            else if(strcmp(command, "lw") == 0)
            {
                int num = arg2[num]/4;
                registerMemory[arg1[num]] =  mainMemory[index];
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
                        registerMemory[31] = instLine;
                        instLine = LABELLINE[i];
                        //                registerMemory[31] = arg1 + 1;
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
            registerMemory[dest[num]] = value;
            next = 0;
            return 0;
    }