//  Read, parse, and store MIPS instructions from user specified .asm file into array.
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

#define MAX_SIZE 100

char *INSTARRAY[MAX_SIZE];
char LABELARRAY[10];
int LABELLINE[10];

int main(int argc, const char * argv[]) {
    FILE *in_file;
    in_file = fopen("tests/bubble.asm", "r");
    
    const size_t line_size = 300;
    char* line = malloc(line_size);
    char* lineArray[MAX_SIZE];
    
//    size_t len = 0;
//    ssize_t read;
    int i = 0;
    
//     If the file hasn't been initialized, stop execution
    if (in_file == NULL) {
        printf("Error Reading File\n");
        exit (0);
    }
    


    while (fgets(line, line_size, in_file) != NULL)  {
//        printf(line);
//        strcat(INSTARRAY[i], line);
        lineArray[i] = malloc(strlen(line));
        strcpy(lineArray[i], line);
        printf("%s", lineArray[i]);
//        printf(INSTARRAY[i]);
        i++;
    }
//    free(line);
//    for (int q = 0; q < sizeof(INSTARRAY); q++) {
//        printf(INSTARRAY[q]);
//    }

    return 0;
    
}

    
    
    
    
//    //while fgets does not fail to scan a line
//    while(fgets(line, sizeof(line), in_file) != NULL) {
////        printf("Test");
////        printf("%s", line);
//        INSTARRAY[i++] = line;
//        printf("%s", INSTARRAY[i]);
//    }
    
//    printf("While done\n");
    
//    for (int q = 0; q < sizeof(INSTARRAY); q++) {
//        printf("%s", INSTARRAY[q]);
//    }
//        if(sscanf(line, "%[^;];%[^;];%lf;%d", itemCode, item, price, quantity) != 4) //If sscanf failed to scan everything from the scanned line
            //%[^;] scans everything until a ';'
            //%lf scans a double
            //%d scans an int
            //Better to use `"%49[^;];%49[^;];%lf;%d"` to prevent buffer overflows
//        {
//            printf("Bad line detected\n");
//            exit(-1);                          //Exit the program
//    }
        //        printf("ItemCode=%s\n", itemCode);
        //        printf("Item=%s\n", item);
        //        printf("price=%f\n", price);
        //        printf("Quantity=%d\n\n", quantity);    //Print scanned items


    
    
    
    
    
    
    
    
    
    
    
    
        // Scan the file and add each integer to  array
//    for (int q=0; q < strlen(instArray); q++) {
//        fscanf(getline(&line, &len, in_file), "%c", &instArray[q]);
//        printf("%s", instArray[q]);
        
//    }
    
    
    
//    while ((read = getline(&line, &len, in_file)) != -1) {
////        printf("Retrieved line of length %zu :\n", read);
////        fscanf(line, "%s", &instArray[i++]);
//        
//        printf("%s", line);
//    }
    

