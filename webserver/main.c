#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "socket.h"

void initialiser_signaux(void)
{
    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
    {
        perror(" signal ");
    }
}

int main(void)
{
    initialiser_signaux();
    int s_serveur = creer_serveur(8080);

    int socket_client;
    while (1)
    {
        /* On attend la connexion du client */ 
        socket_client = accept(s_serveur, NULL, NULL);
        if (socket_client == -1)
        {
            perror(" accept ");
            /* traitement d ’ erreur */
        }

        /* On peut maintenant dialoguer avec le client */
        const char *message_bienvenue = "Bonjour\nbienvenue sur le serveur (ou pas)\nc'est encore en construction\nmais ne vous inquietez pas \non travaille dur pour le rendre op sous peu \nj'espere que ca vous plaira\nen attendant allez prendre un cafe\nou un croissant qui sait ?\nOu...\nrevenez plus tard";
        write(socket_client, message_bienvenue, strlen(message_bienvenue));
    }

    return 0;
}
