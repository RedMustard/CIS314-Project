//  Created by Cody C Ebert on 11/7/15.
//  Copyright © 2015 Cody C Ebert. All rights reserved.
//

#include <stdio.h>

#define MEMORY_SIZE = 20;
//Registry, we can also split the registers up depending on their function as mentioned in 314project channel
int registry[32];

//An array of strings for commands
//We could also parse/decifer each command and
//give each command an integer value
char *instructionMemory[MEMORY_SIZE];
//This will hold our integer data
int dataMemory[MEMORY_SIZE];

//Various MIPS instruction functions
int storeWord(int , int);
int setOnLessThan(int , int);
int setOnGreaterThan(int , int );

//Control Logic is the main function
int main(int argc, const char * argv[])
{
    
    
    return 0;
}
//ALU
int ALU(int arg1, int arg2, char* command)
{
    int result = 0;
    if(strcmp(command, "add") == 0)
    {
        result = arg1 + arg2;
        return result;
    }
    else if(strcmp(command, "subtract") == 0)
    {
        result = arg1 - arg2;
        return result;
    }
    return 0;
}

//Various MIPS commands

int storeWord(int word, int index)
{
    return 0;
}
int setOnLessThan(int arg1, int arg2)
{
    if(arg1 >= arg2)
    {
        return 0;
    }
    else if(arg1 < arg2)
    {
        return 1;
    }
    
}
int setOnGreaterThan(int arg1, int arg2)
{
    if(arg1 <= arg2)
    {
        return 0;
    }
    else if (arg1 > arg2)
    {
        return 1;
    }
}