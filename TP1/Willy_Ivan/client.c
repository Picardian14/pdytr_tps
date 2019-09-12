#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <string.h>
#include <time.h>

#define BUF_SIZE 1008

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

    char buffer[BUF_SIZE+16];
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
	//TOMA EL NUMERO DE PUERTO DE LOS ARGUMENTOS
    portno = atoi(argv[2]);
	
	//CREA EL FILE DESCRIPTOR DEL SOCKET PARA LA CONEXION
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
	//AF_INET - FAMILIA DEL PROTOCOLO - IPV4 PROTOCOLS INTERNET
	//SOCK_STREAM - TIPO DE SOCKET 
	
    if (sockfd < 0) 
        error("ERROR opening socket");
	
	//TOMA LA DIRECCION DEL SERVER DE LOS ARGUMENTOS
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
	
	//COPIA LA DIRECCION IP Y EL PUERTO DEL SERVIDOR A LA ESTRUCTURA DEL SOCKET
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
     serv_addr.sin_port = htons(portno);
	
	//DESCRIPTOR - DIRECCION - TAMAÑO DIRECCION
    if (connect(sockfd,&serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    printf("Please enter the message: ");
    
    bzero(buffer, BUF_SIZE);
    fgets(buffer,BUF_SIZE,stdin);

    //CHECKSUM
    int amount_read =strlen(buffer);
    int num_packets = (amount_read / 16);
    int padding = amount_read % 16;
    char suma[16], checksuma[16];
    bzero(suma, 16);
    for (int i=0; i<num_packets;i++){
        for(int j=0; j<16;j++){
            suma[j]=suma[j]+buffer[(i*16)+j];
        }
    }
    for(int k=0;k<padding;k++){ //Agregamos lo que no entra en paquetes 16
        suma[k]=suma[k]+buffer[(num_packets*16)+k];
    }
    for(int i=0; i<16;i++){ 
        checksuma[i]=~suma[i];
    }
    //FIN CREACION CHECKSUM
    //ENVIA UN MENSAJE AL SOCKET
    memcpy((buffer+amount_read), checksuma, 16);
    
    clock_t start, end=0;
    double cpu_time;
    start = clock();
    FILE *fp;
    fp = fopen("./tiempos1024","a+");
    if(fp ==NULL) printf("No se pudo abrir el archivo. Revisar permisos de escritura");
	n = write(sockfd,buffer,strlen(buffer));
    end = clock();
    if (n < 0) 
         error("ERROR writing to socket");
    bzero(buffer,BUF_SIZE);
	
    //ESPERA RECIBIR UNA RESPUESTA
	n = read(sockfd,buffer,BUF_SIZE);
    end = clock();
    cpu_time = end - start;
    fprintf(fp, "%f\n", cpu_time);
    close(fp);
    if (n < 0) 
         error("ERROR reading from socket");
    
    return 0;
}

