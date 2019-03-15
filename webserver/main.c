#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "socket.h"
#include <sys/wait.h>
#include "prototype.h"
#include "http_parse.h"

void traitement_signal(int sig)
{
	int status = 0;
	waitpid(-1, &status, WNOHANG);

	printf(" Signal %d reçu\n ", sig);
}
void initialiser_signaux(void)
{
	struct sigaction sa;
	sa.sa_handler = traitement_signal;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;

	if (sigaction(SIGCHLD, &sa, NULL) == -1)
	{
		perror(" sigaction ( SIGCHLD ) ");
	}
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
		pid_t pid = fork();

		if (socket_client == -1)
		{
			perror(" accept ");
			/* traitement d ’ erreur */
		}
		if (pid == 0)
		{
			/* On peut maintenant dialoguer avec le client */
			FILE *client = fdopen(socket_client, "w+");
			const char *motd = "Bonjour\nbienvenue sur le serveur (ou pas)\nc'est encore en construction\nmais ne vous inquietez pas \non travaille dur pour le rendre op sous peu \nj'espere que ca vous plaira\nen attendant allez prendre un cafe\nou un croissant qui sait ?\nOu...\nrevenez plus tard";
			const int MAXBUFFSIZE = 200;
			char buffer[MAXBUFFSIZE];

			fgets_or_exit(buffer, MAXBUFFSIZE, client);
			printf("REQUEST --> %s", buffer);
			skip_headers(client);
			http_request request;
			int parse_request = parse_http_request(buffer, &request);
			printf("TARGET ----> %s\n", request.target);

			if (parse_request == 0)
				send_response(client, 400, "Bad Request", "Bad request\r\n");
			else if (request.method == HTTP_UNSUPPORTED)
				send_response(client, 405, "Method Not Allowed", "Method Not Allowed\r\n");
			else if (strcmp(request.target, "/") == 0)
				send_response(client, 200, " OK ", motd);
			else
				send_response(client, 404, "Not Found", "Not Found\r\n");

			fclose(client);
		}
		else
		{
			close(socket_client);
		}
	}
	return 0;
}

char * fgets_or_exit(char * buffer, int size, FILE * stream)
{
	char * message = fgets(buffer, size, stream);
	if (message == NULL)
		exit(0);

	return message;
}

void skip_headers(FILE * client){	const int BUFFSIZE = 1024;	char buffer[BUFFSIZE];	do
	{
		fgets_or_exit(buffer, BUFFSIZE, client);
	} while (strcmp(buffer, "\r\n") != 0);}void send_status(FILE * client, int code, const char * reason_phrase){	fprintf(client, "HTTP/1.1 %d %s\r\n", code, reason_phrase);}void send_response(FILE * client, int code, const char * reason_phrase, const char * message_body){	send_status(client, code, reason_phrase);	fprintf(client, "Host: localhost:8080\r\nConnection: close\r\nContent-Length: %li\r\n\r\n%s\r\n", strlen(message_body), message_body);}