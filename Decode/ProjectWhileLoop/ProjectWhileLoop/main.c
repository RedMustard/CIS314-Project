/*
 * Alex Geoffrey, Cody Ebert and Travis Barnes
 * Alex: Thursday 2PM Lab Section
 * CIS 314 Group Project Fall 2015
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

// Forward-declaring functions:

int ALU(int, int, char *);
int fetchDecode(int );
int registerWriteBack(int targetRegister, int value);
int memoryCommands(char * command, int targetRegister, int memoryIndex);

// Global Variables
static char *INSTRUCTIONS[5] ={"add $a0 $a1 5" , "sub $t0 $v1 6", "add $v1 $at 1", "sw $v1 4($at)", "j label"};
static char *registerArray[32] = {"$zero", "$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3", "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7", "$t8", "$t9", "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7", "$s8", "$k0", "$k1", "$gp", "$sp", "$ra"};
static int registerMemory[32] = {0, 1, 2,3,4,5,6,7,9};	// Register data cache
static char *label;		// Current branch label (if beq/bne is called)
int mainMemory [300];
int labelValueArray[5] = {3};
char * labelArray[10]= {"label"};
int length = sizeof(INSTRUCTIONS) / sizeof(INSTRUCTIONS[0]);
int next = 0;   // 0 = fetchDecode, 1 = ALU, 2 = regWriteBack, 3 = memCommands
char *command;                      // Ie. add, jal, beq, mult, etc. of instruction
int instLine = 0;
int dest = 0;
int arg1 = 0;
int arg2 = 0;
int offset = 0;
int result = 0;


int main ()
{
    //INSTRUCTIONS[5] = {"add $a0 $a1 5" , "sub $t0 $v1 6", "add $v1 $at 1", "sw $v1 4($at)", "j label"};	// ~~~~~ FUNCTION TESTER ~~~~~
    while (instLine < length) {
        if (next == 0) { fetchDecode(instLine); instLine++; }
        if (next == 1) { ALU(arg1, arg2, command); }
        if (next == 2) { memoryCommands(command, arg1, instLine); }
        if (next == 3) { registerWriteBack(arg1, result); }

    }
}
    
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
                    result = registerMemory[i];
                    break;
                }
            }
            label = (strtok(NULL, " "));	// Set global branch label
            next = 1;
        }
        
        // add, sub, mult, div, slt: $dest, $arg1, $arg2
        if (   strcmp(command, "add") == 0
            || strcmp(command, "sub") == 0
            || strcmp(command, "mult")== 0
            || strcmp(command, "div") == 0
            || strcmp(command, "slt") == 0	)
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
            next = 3;
        }
        
        // j, jal: target
        if (   strcmp(command, "j")   == 0
            || strcmp(command, "jal") == 0
            || strcmp(command, "jr")  == 0 )
        {
            label = strtok(NULL, " ");	// Get label for if branch is true, save globally
            next = 2;
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
                    if(strcmp(labelArray[i], label) == 0)
                    {
                        instLine = labelValueArray[i];
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
                    if(strcmp(labelArray[i], label) == 0)
                    {
                        instLine = labelValueArray[i];
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
                next = 0;       // HOW TO IMPLEMENT THIS, GLOBAL FOR SET?
                return 1;
            }
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
            mainMemory[memoryIndex] = registerMemory[targetRegister];
        }
        else if(strcmp(command, "lw") == 0)
        {
            registerMemory[targetRegister] =  mainMemory[memoryIndex];
        }
        else if(strcmp(command, "j") == 0) {
            for(int i = 0; i < 10; i++)
            {   //Finds correct label, and then runs program starting at given index
                if(strcmp(labelArray[i], label) == 0)
                {
                    instLine = labelValueArray[i];
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
                if(strcmp(labelArray[i], label) == 0)
                {
                    instLine = labelValueArray[i];
                    registerMemory[31] = instLine;
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
        registerMemory[targetRegister] = value;
        next = 0;
        return 0;
    }
    
