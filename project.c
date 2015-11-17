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
static char *INSTRUCTIONS[5];
static char *registerArray[32] = {"$0", "$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3", "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7", "$t8", "$t9", "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7", "$s8", "$k0", "$k1", "$gp", "$sp", "$ra"};
static int registerMemory[32] = {1};	// Register data cache
static char *label;		// Current branch label (if beq/bne is called)


int main(){
    //These arrays can be declared as globals
    //so we can manipulate them easily by other functions
    //without descoping
    char * instMem[32];                      // Memory for holding instructions (Is it necessary if we are reading a file?)
                                            //YES It is necessary, we will turn each line into a string and place that string into the instruction array
    int storeMem[32];                      // Actual memory storage (What size?)
    int registryCache[32];                         // 32 x 32-bit elements = 32 "registers" = data cache
    for (i = 0, i < 32, i++) {
        instMem[i] = 0;                     // initialize instruction memory to 0
        storeMem[i] = 0;                    // initialize memory to 0
        cache[i] = 0;                       // initialize all registers to 0
    }
    
   while(
    
}
*/
static int fetchDecode(int index) {
    char *arg;							// Token for strtok()
    char *instruction;					// Copy of instruction
    char *command;						// Ie. add, jal, beq, mult, etc. of instruction
    int arg1 = 0;
    int arg2 = 0;
    int offset = 0;
    int i = 0;
    
    instruction = (char *) malloc(32);
    strcpy(instruction, INSTRUCTIONS[index]);	// Make copy of current instruction
    command = strtok(instruction, " ");		// Get command argument from instruction (add, jal, etc.)
    printf("%s\n", instruction);
    printf("%s\n", command);
    
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
        ALU(arg1, arg2, command);
        return 0;
    }
    
    // add, sub, mult, div, slt: $dest, $arg1, $arg2
    if (   strcmp(command, "add") == 0
        || strcmp(command, "sub") == 0
        || strcmp(command, "mult")== 0
        || strcmp(command, "div") == 0
        || strcmp(command, "slt") == 0	)
    {
        arg = strtok(NULL, " ");
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
        ALU(arg1, arg2, command);
        return 0;
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
        offset = (atoi( strtok(NULL, "(") ) ) /4 ;	// Offset = ( MIPS offset / 4 )
        arg = strtok(NULL, ")");
        for (i = 0; i < 32; i++) {					// Get address from 2nd register, and add offset
            if (strcmp(arg, registerArray[i]) == 0) {
                arg2 = registerMemory[i] + offset;
                break;
            }
        }
        ALU(arg1, arg2, command);
        return 0;
    }
    
    // j, jal: target
    if (   strcmp(command, "j")   == 0
        || strcmp(command, "jal") == 0
        || strcmp(command, "jr")  == 0 )
    {
        label = strtok(NULL, " ");	// Get label for if branch is true, save globally
        ALU(0,0, command);
    }
    free(instruction);
    return 0;
}


int ALU(int arg1, int arg2, char * command)
{
    char * labelArray[10];
    int result = 0;
    //Add
    if(strcmp(command, "add") == 0 || strcmp(command, "addi") == 0)
    {
        result = arg1 + arg2;
        return result;
    }
    //Sub
    else if(strcmp(command, "sub") == 0 || strcmp(command, "subi") == 0)
    {
        result = arg1 - arg2;
        return result;
    }
    //Multiplication
    else if( strcmp(command, "mult") == 0 || strcmp(command, "multi") == 0)
    {
        result = arg1 * arg2;
        return result;
    }
    //Division
    else if(strcmp(command, "div") == 0 || strmp(command, "divi") == 0)
    {
        
        result = arg1/arg2;
        return result;
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
                    int temp = labelValueArray[i];
                    fetchDecode(temp);
                    return 1;
                }
                
            }
            return 1;
        }
        else if(arg1 != arg2)
        {
            return 0;
        }
    }
    //Branch if not equal
    else if(strcmp(command, "bne") == 0)
    {
        if(arg1 == arg2)
        {
            return 0;
        }
        else if(arg1 != arg2)
        {
            //Loop through labelArray to find proper label
            for(int i = 0; i < 10; i++)
            {   //Finds correct label, and then runs program starting at given index
                if(strcmp(labelArray[i], label) == 0)
                {
                    int temp = labelValueArray[i];
                    fetchDecode(temp);
                    return 1;
                }
                
            }
            
            return 1;
        }
    }
    //Set less than commands
    else if (strcmp(command, "slt") == 0 || strcmp(command, "sltu") == 0 || strcmp(command, "slti"))
    {
        if(arg1 < arg2)
        {
            return 1;
        }
        return 0;
    }
    
    
    return 1;
}
//SW/LW
int memoryCommands(char * command, int targetRegister, int memoryIndex)
{
    if(strcmp(command, "sw") == 0)
    {
        mainMemory[memoryIndex] = registerArray[targetRegister];
    }
    else if(strcmp(command, "lw") == 0)
    {
        registerArray[targetRegister] =  mainMemory[memoryIndex];
    }
    else if(strcmp(command, "j"))
    {
        for(int i = 0; i < 10; i++)
        {   //Finds correct label, and then runs program starting at given index
            if(strcmp(labelArray[i], label) == 0)
            {
                int temp = labelValueArray[i];
                fetchDecode(temp);
                return 1;
            }
            
        }
        
        return 1;
    }
    else if(strcmp(command, "jal")==0)
    {
        for(int i = 0; i < 10; i++)
        {   //Finds correct label, and then runs program starting at given index
            if(strcmp(labelArray[i], label) == 0)
            {
                
                int temp = labelValueArray[i];
                registerArray[31] = temp;
                fetchDecode(temp);
                return 1;
            }
            
        }
    }
    else if(strcmp(command, "jr") == 0)
    {
        fetchDecode(registerArray[31]+1);
    }
    
    return 0;
}
int registerWriteBack(int targetRegister, int value)
{
    registerArray[targetRegister] = value;
    
    return 0;
}
         return 0;
         }