#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "socket.h"
#include <sys/wait.h>


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
      FILE *fp = fdopen(socket_client, "w+");
      /*const char *message_bienvenue = "Bonjour\nbienvenue sur le serveur (ou pas)\nc'est encore en construction\nmais ne vous inquietez pas \non travaille dur pour le rendre op sous peu \nj'espere que ca vous plaira\nen attendant allez prendre un cafe\nou un croissant qui sait ?\nOu...\nrevenez plus tard\n\n";
      fprintf(fp, "%s", message_bienvenue);*/
      int taillemsg = 200;
      char s[taillemsg];
      fgets(s, taillemsg, fp);
      strcat(s, "\r\n");
      /*char * retour = fgets(s, taillemsg, fp);
      char *retourligne = strcat(s, "\r\n");
      if (retourligne != NULL)
      {
        retourligne = '\0';
      }*/
      if(strcmp("GET / HTTP/1.1\r\n",s)) {
          //fprintf(fp,"HTTP/1.1 400 Bad Request\r\nConnection: close\r\nContent-Length: 17\r\n\r\n400 Bad request\r\n",s);
      } else {
        fprintf(fp,"HTTP/1.1 400 Bad Request\r\nConnection: close\r\nContent-Length: 17\r\n\r\n400 Bad request\r\n");
      }
      fclose(fp);
    }
    else
    {
      close(socket_client);
    }
  }

  return 0;
}
