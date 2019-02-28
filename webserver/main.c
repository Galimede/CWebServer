#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "socket.h"
#include <sys/wait.h>

void traitement_signal ( int sig )
{
  int status =0;
  waitpid(-1,&status, WNOHANG);

  printf ( " Signal %d reçu\n " , sig );
}
void initialiser_signaux(void)
{
  struct sigaction sa ;
  sa.sa_handler = traitement_signal ;
  sigemptyset (&sa . sa_mask );
  sa.sa_flags = SA_RESTART ;

  if ( sigaction ( SIGCHLD , &sa , NULL ) == -1){
    perror ( " sigaction ( SIGCHLD ) " );
  }
  if (signal(SIGPIPE, SIG_IGN) == SIG_ERR){
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
    if (pid == 0){
      /* On peut maintenant dialoguer avec le client */
      FILE *fp = fdopen(socket_client,"w+");
      const char *message_bienvenue = "Bonjour\nbienvenue sur le serveur (ou pas)\nc'est encore en construction\nmais ne vous inquietez pas \non travaille dur pour le rendre op sous peu \nj'espere que ca vous plaira\nen attendant allez prendre un cafe\nou un croissant qui sait ?\nOu...\nrevenez plus tard\n\n";
      //write(socket_client, message_bienvenue, strlen(message_bienvenue));
      //fwrite(message_bienvenue, strlen(message_bienvenue), 1, fp);
      fprintf(fp, "%s",message_bienvenue);
      int taillemsg = 100;
      char s[taillemsg];
      fgets(s,taillemsg,fp);
      printf("%lu\n",strlen(s) );
      fprintf(fp,"<WebServer>\n%s</WebServer>\n",s );
      fclose(fp);
    } else{
      close(socket_client);
    }

  }

  return 0;
}
