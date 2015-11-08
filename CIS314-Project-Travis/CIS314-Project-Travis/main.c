//
//  main.c
//  CIS314-Project
//
//  Created by Travis Barnes on 11/8/15.
//  Copyright (c) 2015 Travis Barnes. All rights reserved.
//

#include <stdio.h>

int main(int argc, const char * argv[]) {
    /////////////////////////////////////////////////////
    /////////////////// FILE READ //////////////////////
    FILE *in_file;
    in_file = fopen("", "r");
    int size;
    int value;
    char in_file_name[] = FILE_NAME;
    
    // If the file hasn't been initialized, stop execution
    if (in_file == NULL) {
        printf("Error Reading File\n");
        exit (0);
    }
    // Scan the file and add each integer to  array
    for (int i=0; i < MAX_SIZE; i++) {
        fscanf(in_file, "%d", &array[i-1]);
    }
    /////////////////////////////////////////////////////
}
