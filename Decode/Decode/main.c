//
//  main.c
//  Decode
//
//  Created by Alex Geoffrey on 11/8/15.
//  Copyright © 2015 MooseMen. All rights reserved.
//


// Forward-delaring function
static int fetchDecode(int index);
static int ALU(int arg1, int arg2, char * command);


// Global variables
static char *INSTRUCTIONS[5];
static char *registerArray[32] = {"$0", "$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3", "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7", "$t8", "$t9", "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7", "$s8", "$k0", "$k1", "$gp", "$sp", "$ra"};
static int registerMemory[32] = {1};	// Register data cache
static char *label;		// Current branch label (if beq/bne is called)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main () {
    int index = 0;
	INSTRUCTIONS[0] = "sw $t0 16($at)";	// ~~~~~ FUNCTION TESTER ~~~~~
	fetchDecode(index);
}

/* fetchDecode:
 *  - Read command
 *  - Determine what arguments are
 *  - Send correct args to ALU
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
		arg = strtok(NULL, ", ");
		for (i = 0; i < 32; i++) {			// Find register value for 1st arg
			if (strcmp(arg, registerArray[i]) == 0) {
				arg1 = registerMemory[i];
				break;
			}
		}
		arg = strtok(NULL, ", ");
		for (i = 0; i < 32; i++) {			// Find register value for 2nd arg
			if (strcmp(arg, registerArray[i]) == 0) {
				arg2 = registerMemory[i];
				break;
			}
		}
		label = (strtok(NULL, ", "));	// Set global branch label
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
// Prints what ALU recieves. To test fetchDecode's outputs.
static int ALU(int arg1, int arg2, char * command) {
	printf("ALU:\n- arg1 = %d\n- arg2 = %d\n- command = %s\n", arg1, arg2, command);
	return 0;
}