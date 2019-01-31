#include <stdio.h>
#include "socket.h"
#include <sys/types.h>         
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

int creer_serveur(int port) {

// On crée d'abord la socket
int socket_serveur ;
socket_serveur = socket (AF_INET, SOCK_STREAM , 0);

	if (socket_serveur == -1) {
	/* traitement de l ’ erreur */
	perror("socket_serveur");
	}
	/* Utilisation de la socket serveur */

	// Ensuite on configure et attache la socket à une interface réseau 
	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET; /* Socket ipv4 */
	saddr.sin_port = htons(port); /* Port d’écoute */
	saddr.sin_addr.s_addr = INADDR_ANY; /* écoute sur toutes les interfaces */

	if (bind (socket_serveur,(struct sockaddr *) &saddr, sizeof(saddr)) == -1) {
		/* traitement de l ’ erreur */
		perror("bind socket_serveur");
	}
	
	// Enfin on demande au système de pouvoir écouter sur le port et de pouvoir attendre des connexions 
	if (listen (socket_serveur, 10) == -1) {
		perror ("listen socket_serveur");
		/* traitement d ’ erreur */
	}

	// Maintenant on peu accepter des connexions des clients
	// /!\ LA FONCTION ACCEPT EST BLOQUANTE 

	int socket_client ;
	
	socket_client = accept ( socket_serveur , NULL , NULL );
	if ( socket_client == -1) {
		perror ( " accept " );
		/* traitement d ’ erreur */
	}
	
	/* On peut maintenant dialoguer avec le client */
	const char * message_bienvenue = "Bonjour\nbienvenue sur le serveur (ou pas)\nc'est encore en construction\nmais ne vous inquietez pas \non travaille dur pour le rendre op sous peu \nj'espere que ca vous plaira\nen attendant allez prendre un cafe\nou un croissant qui sait ?\nOu...\nrevenez plus tard" ;
	write ( socket_client , message_bienvenue , strlen ( message_bienvenue ));

	return 0;
}