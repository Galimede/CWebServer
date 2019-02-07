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

	int optval = 1;
	if ( setsockopt ( socket_serveur , SOL_SOCKET , SO_REUSEADDR , & optval , sizeof ( int )) == -1)
	perror ( " Can not set SO_REUSEADDR option " );
	if (bind (socket_serveur,(struct sockaddr *) &saddr, sizeof(saddr)) == -1) {
		/* traitement de l ’ erreur */
		perror("bind socket_serveur");
	}

	// Enfin on demande au système de pouvoir écouter sur le port
	if (listen (socket_serveur, 10) == -1) {
		perror ("listen socket_serveur");
		/* traitement d ’ erreur */
	}

	return socket_serveur;
}
