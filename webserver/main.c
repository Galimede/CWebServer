#include <stdio.h>
#include <string.h>
#include <signal.h>
#include "socket.h"

void initialiser_signaux(void)
{
    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
    {
        perror("signal");
    }
}

int main(void)
{
    /* Arnold Robbins in yhe LJ of February '95, describing RCS */
    /*if (argc > 1 && strcmp(argv[1], "-advice") == 0) {
        printf("Don't Panic!\n");
        return 42;
    }
    printf("Need an advice?\n");
    return 0;*/
    
    initialiser_signaux();
    creer_serveur(8080);

    return 0;
}
