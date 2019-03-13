#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if defined (WIN32)
#include <winsock2.h>
typedef int socklen_t;
#elif defined (linux)
//Socket-server.c
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
//#include <netdb.h>
#endif
#include <signal.h>
#include <pthread.h>

int PORT=8080;
struct sockaddr_in  cli_addr;
char fname[100];
void* SendFileToClient(void *data);

void* SendFileToClient(void *arg)
{
      int connfd=(int*) arg;
      printf("Connection accepted and id: %d\n",connfd);
      printf("Connected to Clent: %s:%d\n",inet_ntoa(cli_addr.sin_addr),ntohs(cli_addr.sin_port));
       write(connfd, fname,256);

        FILE *fp = fopen(fname,"rb");
        if(fp==NULL)
        {
            printf("File opern error");
            return 1;
        }

        /* Read data from file and send it */
        while(1)
        {
            /* First read file in chunks of 256 bytes */
            unsigned char buff[1024]={0};
            int nread = fread(buff,1,1024,fp);
            //printf("Bytes read %d \n", nread);

            /* If read was success, send data. */
            if(nread > 0)
            {
                //printf("Sending \n");
                write(connfd, buff, nread);
            }
            if (nread < 1024)
            {
                if (feof(fp))
		{
                    printf("End of file\n");
		    printf("File transfer completed for id: %d\n",connfd);
		}
                if (ferror(fp))
                    printf("Error reading\n");
                break;
            }
        }
printf("Closing Connection for id: %d\n",connfd);
close(connfd);
shutdown(connfd,SHUT_WR);
sleep(2);
}


void cleanExit(){exit(0);}
int main(void)
{
  int listenfd = 0,connfd = 0,err;
  struct sockaddr_in serv_addr;
  char sendBuff[1025];
  //int numrv;
  int mawConn= 100;
  int nb = 0;

  pthread_t tid;
// far
      /* Socket et contexte d'adressage du client */
//  struct sockaddr_in  cli_addr;
  socklen_t cli_size = sizeof(cli_addr);
// ***
  int clients[mawConn];   // Maintain a list of clients

  listenfd = socket(AF_INET, SOCK_STREAM, 0);
  printf("socket retrieve success\n");

  memset(&serv_addr, '0', sizeof(serv_addr));
  memset(sendBuff, '0', sizeof(sendBuff));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(PORT);

  bind(listenfd, (struct sockaddr*)&serv_addr,sizeof(serv_addr));

  if(listen(listenfd, 10) == -1){
    printf("Failed to listen\n");
    return -1;
  }

  /* Création de l'ensemble de lecture */
  fd_set readfs;

  signal(SIGTERM, cleanExit);
  signal(SIGINT, cleanExit);
  while(1)
  {
    FD_ZERO(&readfs);
    FD_SET(listenfd, &readfs);

                        /* Si une erreur est survenue au niveau du select */
    if(select(listenfd + 1, &readfs, NULL, NULL, NULL) < 0)
    {
      perror("select()");
      exit(errno);
    }
    /* On regarde si la socket serveur contient des
                        informations à lire */
    if(FD_ISSET(listenfd, &readfs))
     {

          struct sockaddr_in  csin;
          int crecsize = sizeof csin;

          connfd = accept(listenfd, (struct sockaddr*)&cli_addr ,&cli_size); // accept awaiting request
          sprintf(sendBuff, "Message from server- Un client se connecte avec la socket %d de %s:%d\n",
                    listenfd, inet_ntoa(cli_addr.sin_addr), htons(cli_addr.sin_port));
          write(connfd, sendBuff, strlen(sendBuff));

          err = pthread_create(&tid, NULL, &SendFileToClient, &connfd);
          if (err != 0)
            printf("\ncan't create thread :[%s]", strerror(err));

          clients[nb] =connfd;    //Add client to list on connection

          //strcpy(sendBuff, "Message from server");
          //write(connfd, sendBuff, strlen(sendBuff));
          close(connfd);
        }
    sleep(1);
  }

  return 0;
}
