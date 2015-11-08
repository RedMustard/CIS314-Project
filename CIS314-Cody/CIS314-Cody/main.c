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

int main(int argc, const char * argv[])
{
  
    
    return 0;
}
int ALU(int *arg1, int *arg2, char* command)
{
    int result = 0;
    if(strcmp(command, "add") == 0)
    {
        result = *arg1 + *arg2;
        return result;
    }
    else if(strcmp(command, "subtract") == 0)
    {
        result = *arg1 - *arg2;
        return result;
    }
    return 0;
}