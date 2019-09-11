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

//     for(int i = 0; i < iterations; i++){
//       n = read(newsockfd,buffer, buffer_size-1);
//       printf("\n %d bytes recibidos.\n", n);
//       for(int j = 0; j < n; j++){
//         if(buffer[j] != '1'){
//           printf("\n El byte %d esta mal : %d en la iteracion %d.\n", j, buffer[j], i);
//           break;
//         }
//       }
//       if (n < 0) error("ERROR reading from socket");
//     }

     int end = 0;

     while(!end && (n = read(newsockfd,buffer, buffer_size-1))){
     
       printf("\n %d bytes recibidos.\n", n);
       for(int j = 0; j < n; j++){
         if(buffer[j] == '\0'){
	   printf("\n Fin del paquete. Byte %d\n", j);
           end = 1;
           break;
         }
         if(buffer[j] != '1'){
           printf("\n El byte %d esta mal : %d.\n", j, buffer[j]);
           break;
         }
       }
       if (n < 0) error("ERROR reading from socket");
     }

     if (n < 0) error("ERROR reading from socket");
     

     n = write(newsockfd,"I got your message",18);
     if (n < 0) error("ERROR writing to socket");
     return 0; 
}
