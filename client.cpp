#include <sys/types.h>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
using namespace std;
extern int errno;

int port;
int flag=1;
void afisareTabla ( char * x)
{
    int k=0;
    for (int i = 0; i < strlen(x); i++)
    {
        if (x[i]=='1')  cout<<"| ",k++;
        if ((x[i-1]=='2' || x[i-1]=='3' || x[i-1]=='4')&&x[i]=='1') cout<<"\n";
        if (x[i]=='2')  cout<<"* ",k++;
        if (x[i]=='3')  cout<<"\033[1;32mV\033[0m ",k++;
        if (x[i]=='4')  cout<<"\033[1;31mR\033[0m ",k++;
    }
}
int main (int argc, char *argv[])
{
  int sd; 
  struct sockaddr_in server;  
  char primit[100]; 
  char rand;
  if (argc != 3)
    {
      printf ("[-]Sintaxa: %s <adresa_server> <port>\n", argv[0]);
      return -1;
    }

  port = atoi (argv[2]);

  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("[-]Eroare la socket().\n");
      return errno;
    }

  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr(argv[1]);
  server.sin_port = htons (port);

  if (connect (sd, (struct sockaddr *) &server,sizeof (struct sockaddr)) == -1)
    {
      perror ("[+]Eroare la connect().\n");
      return errno;
    }
  cout<<"\033[1;31m Regula de joc -> \033[0m \n";
  printf("Completeaza cu numere de la 1 la 7 incercand sa conectezi 4 culori pentru a castiga. \nDaca tabla este umpluta si niciunul dintre voi nu a castigat, atunci va fi remiza!\n");
  printf("[+]Astept sa se conecteze partenerul de joc .\n");
  if (read (sd, primit, 100) < 0)
    {
      perror ("[-]Eroare la read() de la server.\n");
      return errno;
    }
    rand=primit[0];


if(rand=='P'){
printf("Esti jucatorul numarul 1! \n");
}
else
  printf("Esti jucatorul numarul 2! \n");


char culoare;
culoare=primit[0];
if(culoare=='P'){
cout<<"Serverul a ales pentru tine culoarea \033[1;32m verde \033[0m ! Spor la joc !\n";
}
else
  cout<<"Serverul a ales pentru tine culoarea \033[1;31m rosie \033[0m ! Spor la joc !\n";
  bzero (primit, 100);
  
while(flag)
{
    bzero (primit, 100);

    if (read (sd, primit, 100) < 0)
      {
        perror ("[client]Eroare la read() de la server.\n");
        return errno;
      }
    if(primit[0]=='w'){
      char scor1=primit[1];
      
      bzero(primit,100);
      cout<<"Ai castigat!\n";
      write(sd,"g",100);
      flag=0;
    }
    else
      if (primit[0]=='l')
    {

      bzero(primit,100);
      cout<<"Ai pierdut !\n"; 
      write(sd,"g",100);
      flag=0;
    } if(primit[0]=='i'){

       bzero(primit,100);
      cout<<"egal  \n"; 
      write(sd,"g",100);
      flag=0;
    }
    else
      if (primit[0]=='j')
    {

      bzero(primit,100);
      cout<<"egal\n"; 
      write(sd,"g",100);
      flag=0;
    }
    if(primit[0]=='r')
    {
      bzero (primit, 100);
      read (0, primit, 100);
      if (write (sd, primit, 100) < 0)
      {
        perror ("[client]Eroare la write() catre server.\n");
        return errno;
      }
    }
    if(primit[0]=='e')
    {
      cout<<"Coloana invalida.\n";
      bzero (primit, 100);     
      read (0, primit, 100);
      if (write (sd, primit, 100) < 0)
      {
        perror ("[client]Eroare la write() catre server.\n");
        return errno;
      }
    }
    if(primit[0]=='t')
    {
      bzero (primit, 100);
      if (read (sd, primit, sizeof(primit)) < 0)
      {
        perror ("[client]Eroare la write() catre server.\n");
        return errno;
      }
      afisareTabla(primit);
    }
   
}
  close (sd);
}