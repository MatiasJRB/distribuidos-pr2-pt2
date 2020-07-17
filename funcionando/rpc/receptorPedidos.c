#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <string.h> 

#include "receptorPedidos.h"
#include "enviarRecibir.h"

#define PUERTO 15000 // puerto en el que cada nodo va a escuchar

#define MAXDATASIZE 512
#define DOWNLOAD 1
#define UPLOAD 2
#define UPDATE 3
#define ERROR 4
#define ACK 5

typedef struct PAQUETE_SOCKET
{
	int identificador; //Identificador del tipo de operacion que se este realizando
	int tamanio_data; //Indicará el tamaño del archivo en bytes
    int ultimo_paquete;
    char data[MAXDATASIZE]; //Indicará la ruta del archivo (id=1) o el archivo en sì (si el id=2)
} PAQUETE_SOCKET;

static PAQUETE_SOCKET enviar_paquete, recibir_paquete;



void * receptorPedidosNodo()
{
	int sockfd; // el servidor escuchara por sockfd
	int newfd; // las transferencias de datos se realizara mediante newfd
	
	struct sockaddr_in servidorInfo; // contendra la IP y puerto local
	struct sockaddr_in clienteInfo; // contendra IP y puerto del cliente
	
	int sin_size; // contendra el tamaño de la escructura sockaddr_in
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
	{
		perror("Error en la creación del socket\n");
		exit(EXIT_FAILURE);
	}

    int optval = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) == -1) {
        perror("Set socket option\n");        
        exit(errno);
    }

	//Prepara los datos del servidor antes de ponerlo a escuchar
	//Asigna valores a la estructura my_addr para luego poder llamar a la funcion bind()
	servidorInfo.sin_family = AF_INET;
	servidorInfo.sin_port = htons(PUERTO); // formato de network byte order PUERTO_PEDIDOS puerto indicado por el usuario
	servidorInfo.sin_addr.s_addr = INADDR_ANY; // automaticamente usa IP local
	bzero(&(servidorInfo.sin_zero), 8); // rellena con ceros el resto de la estructura

	//Asigna un nombre al socket
	if (bind(sockfd, (struct sockaddr *)&servidorInfo, sizeof(struct sockaddr)) == -1)
	{
		perror("Error en función bind()\n");
		exit(EXIT_FAILURE);
	}
	// Habilita el socket para recibir conexiones, con una cola de x conexiones en espera como máximo
	if (listen(sockfd, 10) == -1)
	{
		perror("Error en funcion listen()\n");
		exit(EXIT_FAILURE);
	}
	while(1) 
	{ 
		
		// loop que llama a accept()
		sin_size = sizeof(struct sockaddr_in);
		// Se espera por conexiones
		if ((newfd = accept(sockfd, (struct sockaddr *)&clienteInfo, (socklen_t *) &sin_size)) == -1)
		{
			perror("Error en función accept()\n");
            exit(EXIT_FAILURE);
		}
		printf("Pedido desde el nodo con IP: %s\n", inet_ntoa(clienteInfo.sin_addr));
		printf("Conexion desde puerto: %d \n", ntohs(clienteInfo.sin_port));

		if (!fork()) 
		{ 
			// Comienza el proceso hijo, enviamos los datos mediante newfd
            recibir(newfd,&recibir_paquete,sizeof(struct PAQUETE_SOCKET));
			printf("Paquete recibido con identificador %d, tamaño %d, y ruta %s \n\n",recibir_paquete.identificador, recibir_paquete.tamanio_data, recibir_paquete.data);
			switch(recibir_paquete.identificador)
			{
				//Descarga del archivo
				case DOWNLOAD:
						printf("Pedido de Archivo\n");
                        FILE *archivo = fopen(recibir_paquete.data,"r");
                        if(archivo==NULL){
                            printf("No existe el archivo\n");
                            enviar_paquete.identificador=ERROR;
                            snprintf(enviar_paquete.data, MAXDATASIZE-1, "El archivo que pidió no existe.");
                            enviar(newfd, &enviar_paquete ,sizeof(struct PAQUETE_SOCKET));
                            exit(EXIT_FAILURE);
					    }
                        int leidos = 0;
                        enviar_paquete.ultimo_paquete = 0;
                        while(!enviar_paquete.ultimo_paquete){
                            leidos = fread(enviar_paquete.data, sizeof(char), MAXDATASIZE, archivo);
                            enviar_paquete.identificador=UPLOAD;
                            enviar_paquete.tamanio_data = leidos;
                            enviar_paquete.ultimo_paquete = feof(archivo);
                            enviar(newfd, &enviar_paquete,sizeof(struct PAQUETE_SOCKET)); 

                            recibir(newfd,&recibir_paquete,sizeof(struct PAQUETE_SOCKET));
                            if (recibir_paquete.identificador != ACK) {
                                printf("Error en la transferencia del archivo.\n");
                                exit(EXIT_FAILURE);
                            }
                        }
                        fclose(archivo);
                        printf("\nArchivo enviado\n");
                        break;

				case UPLOAD:
						printf("Envio de Archivo\n");
						break;

				case UPDATE:
						printf("Actualizacion de Archivo\n");
						break;

				default:	
                        printf("No existe la operación.\n");
						break;
			}
			close(newfd);
			//menu();
			exit(EXIT_SUCCESS);
		} 
		close(newfd);
	}
}
