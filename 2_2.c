// Assignment 2 - Problem 2: Reader-Writer Problem
#include <stdio.h>
#include <stdlib.h>

int main() {
    int readcount = 0, writecount = 0, mutex = 1, wrt = 1, choice;
    
    printf("\n1. Reader\n2. Writer\n3. Exit");
    
    while(1) {
        printf("\n\nEnter your choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                // Reader
                if(mutex == 1 && wrt == 1) {
                    readcount++;
                    if(readcount == 1)
                        wrt = 0;
                    mutex = 0;
                    printf("\nReader %d is reading", readcount);
                    mutex = 1;
                    readcount--;
                    if(readcount == 0)
                        wrt = 1;
                } else {
                    printf("\nWriter is writing, reader must wait!");
                }
                break;
            case 2:
                // Writer
                if(wrt == 1) {
                    wrt = 0;
                    writecount++;
                    printf("\nWriter %d is writing", writecount);
                    wrt = 1;
                } else {
                    printf("\nResource is busy!");
                }
                break;
            case 3:
                exit(0);
            default:
                printf("\nInvalid choice!");
        }
    }
    
    return 0;
}
