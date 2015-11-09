//
//  main.c
//  CIS314-Cody
//
//  Created by Cody C Ebert on 11/8/15.
//  Copyright © 2015 Cody C Ebert. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#define ARR_SIZE = 10;

int main(int argc, const char * argv[])
{
    int arr[5] = {1,2,3,4,5};
   
    char * command = "add";
    printf("%d",ALU(arr[0], arr[1], command));

    return 0;
}
int ALU(int arg1, int arg2, char* command, char * label)
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
                    return fetchDecode(i);
                }
                else
                    return 0;
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
                    return fetchDecode(i);
                }
                else
                    return 0;
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
        return 2;
    }
    return result;
}
int memoryCommands()
{
    
    return 0;
}