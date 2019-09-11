#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <stdlib.h>
#include <string.h>

void error(char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    int buffer_size, iterations;
    if (argc < 5) {
       fprintf(stderr,"Hostname, port, buffer size and iterations.");
       exit(0);
    }
    buffer_size = atoi(argv[3]);
    iterations = atoi(argv[4]);
    char buffer[buffer_size];
    
    portno = atoi(argv[2]);
	
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
    if (sockfd < 0) 
        error("ERROR opening socket");
	
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
	
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
     serv_addr.sin_port = htons(portno);
	
    if (connect(sockfd,&serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    printf("Please enter the message: ");
    bzero(buffer,buffer_size);
    for(int i = 0; i < buffer_size; i++)
      buffer[i] = '1';
    
    for(int i = 0; i < iterations; i++){
      n = write(sockfd,buffer,buffer_size);
      if (n < 0) 
        error("ERROR writing to socket");
    }
    
    n = write(sockfd,"\0",1);
    if (n < 0) 
      error("ERROR writing to socket");
    
    bzero(buffer,buffer_size);
	
    n = read(sockfd,buffer,buffer_size);
    if (n < 0) 
         error("ERROR reading from socket");
    
    printf("%s\n",buffer);
    return 0;
}
