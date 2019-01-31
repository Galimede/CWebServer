#include <stdio.h>
#include <string.h>
#include "socket.h"
int main(void) {
    /* Arnold Robbins in yhe LJ of February '95, describing RCS */
    /*if (argc > 1 && strcmp(argv[1], "-advice") == 0) {
        printf("Don't Panic!\n");
        return 42;
    }
    printf("Need an advice?\n");
    return 0;*/

    while(1) {
        creer_serveur(8080);      
    }

    return 0;
    
}
