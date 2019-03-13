#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#if defined (WIN32)
#include <winsock2.h>
typedef int socklen_t;
#elif defined (linux)
//Socket-server.c
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#endif
#include <unistd.h>
int PORT=8080;

int main(void)
{
  int sockfd = 0,n = 0;
  char recvBuff[1024];

  int ret, choix = 1;
  char temp[10];

  struct sockaddr_in serv_addr;
  memset(recvBuff, '0' ,sizeof(recvBuff));
  if((sockfd = socket(AF_INET, SOCK_STREAM, 0))< 0)
  {
    printf("\n Error : Could not create socket \n");
    return 1;
  }
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);
  serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

  if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0)
  {
    printf("\n Error : Connect Failed \n");
    return 1;
  }

  while(choix >0 ) {
    char saisie[10];
    char user[50];
    char pwd[50];
    printf("Entrez un nombre : \n1) #Exit\n2) #Help (list Command)\n3) #ListU ( list user in a server)\n4) #ListF (list files in a server)\n5) #TrfU ( transfert Upload file in a server)\n6) #TrfD ( transfert Download file in a server)\n7) #Private <user> (commute to private )");
    fflush (stdout);
    fgets (saisie, sizeof saisie, stdin);

    if (sscanf (saisie, "%[0-9-]s", temp) == 1) choix = atoi (temp);
    switch (choix) {
      case  1 :  return 0;
      case  2 : break;
      case  3 :   {
        printf (" votre choix %i\n", choix);
        //if (gethostname(char *hostname, size_t size) >0) printf (" hostname : %s\n", hostname);

      }
      case  14 :
      {
        printf (" votre choix %i\n", choix);
        while((n = read(sockfd, recvBuff, sizeof(recvBuff)-1)) > 0)
        {
          recvBuff[n] = 0;
          if(fputs(recvBuff, stdout) == EOF)
          {
            printf("\n Error : Fputs error");
          }
          printf("\n");
        }

        if( n < 0)
        {
          printf("\n Read Error \n");
        }
      }
      case 5 :
      {
        printf("Enter file name to send: ");
        gets(fname);
      }

      case 6 : {

      }
      case 7 : {
        printf (" votre user :\n");
        fgets (user, sizeof user, stdin);
        printf (" votre pwd :\n");
        fgets (pwd, sizeof pwd, stdin);

      }
    }
  }


  return 0;
}
