/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>

#define BUF_SIZE 1008
void error(char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno, clilen;
     char buffer[BUF_SIZE];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
	 //CREA EL FILE DESCRIPTOR DEL SOCKET PARA LA CONEXION
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
	 //AF_INET - FAMILIA DEL PROTOCOLO - IPV4 PROTOCOLS INTERNET
	//SOCK_STREAM - TIPO DE SOCKET 
	
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     //ASIGNA EL PUERTO PASADO POR ARGUMENTO
	 //ASIGNA LA IP EN DONDE ESCUCHA (SU PROPIA IP)
	 portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
	 
	 //VINCULA EL FILE DESCRIPTOR CON LA DIRECCION Y EL PUERTO
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
			  
	 //SETEA LA CANTIDAD QUE PUEDEN ESPERAR MIENTRAS SE MANEJA UNA CONEXION		  
     listen(sockfd,5);
	 
	 // SE BLOQUEA A ESPERAR UNA CONEXION
     clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd, 
                 (struct sockaddr *) &cli_addr, 
                 &clilen);
				 
     //DEVUELVE UN NUEVO DESCRIPTOR POR EL CUAL SE VAN A REALIZAR LAS COMUNICACIONES
	 if (newsockfd < 0) 
          error("ERROR on accept");
     bzero(buffer,BUF_SIZE);

	//LEE EL MENSAJE DEL CLIENTE
     n = read(newsockfd,buffer,BUF_SIZE+16);

     //HACEMOS CHECKSUM
     int size_sent = strlen(buffer) - 16;
     int num_packets = (size_sent)/16;
     int padding = size_sent % 16;
     char sum[16], checksum[16];
     bzero(sum,16);
     
     for (int i=0; i<num_packets;i++){
        for(int j=0; j<16;j++){
            sum[j]=sum[j]+buffer[(i*16)+j];
        }
    }
    for(int k=0;k<padding;k++){
        sum[k]=sum[k]+buffer[num_packets*16+k];
    }
    memcpy(checksum,(buffer+size_sent),16);
     for(int k=0; k<16;k++){
          if (~(checksum[k] ^ sum[k])){
               error("Error en el checksum. Paquete corrompido");
          }  
     }
     char rcvd_message[size_sent];
     printf("El paquete llego sin errores");
     for (int i=0;i<size_sent;i++){
          rcvd_message[i]=buffer[i];
     }
     printf("\n");
     if (n < 0) error("ERROR reading from socket");
     printf("Here is the message: %s\n",rcvd_message);
	 
	 //RESPONDE AL CLIENTE
     n = write(newsockfd,"I got your message",18);
     if (n < 0) error("ERROR writing to socket");
     return 0; 
}
