/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>

void error(char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno, clilen;
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     int buffer_size, iterations;
     if (argc < 4) {
         fprintf(stderr,"Proveer puerto, tamanio de buffer e iteraciones.\n");
         exit(1);
     }

     buffer_size = atoi(argv[2]);
     iterations = atoi(argv[3]);
     char buffer[buffer_size];
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
	 
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
			  
     listen(sockfd,5);
	 
     clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd, 
                 (struct sockaddr *) &cli_addr, 
                 &clilen);
				 
     if (newsockfd < 0) 
       error("ERROR on accept");
     bzero(buffer, buffer_size);
     n = read(newsockfd, buffer, buffer_size);
     if (n < 0) error("ERROR reading from socket");
     int packet_size;
     memcpy(&packet_size, buffer, sizeof(int));
     printf("\n Recibiendo %d bytes de datos.", packet_size);

     int received = 0, err = 0;

     while((received < packet_size) && (n = read(newsockfd,buffer, buffer_size-1))){
       printf("\n %d bytes recibidos.\n", n);
       received += n;
     }

     if (n < 0) error("ERROR reading from socket");
     if (!err)  printf("\n Se recibieron todos los datos correctamente.\n");
     int sent = write(newsockfd,"1",2);
     if (sent < 0) error("ERROR writing to socket");

     n = write(newsockfd,"I got your message",18);
     if (n < 0) error("ERROR writing to socket");
     return 0; 
}
