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

#include <string.h> 


#include "emisorPedidos.h"
#include "enviarRecibir.h"
/*
Consideraciones a tener en cuenta:
    - sobrescribir archivos?
    - si hay algún problema y hacemos exit(), se cortaría todo. 
        una mejor solución quizás sería llamar a una función error() del shell para que notifique 
        al usuario con un mensaje en vez de terminar todo el sistema.
*/

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

//emisorPedidos(char* ip, char* route, char* destino,int opcion);
//++++++++++++++++++++NODO CLIENTE+++++++++++++++++++++++++++++++++++++++++++++
void emisorPedidosNodo(char* ip, char* route, char* destino,int opcion) 
{
    int opcion_usuario=-1;
	int sockfd; 
	struct hostent *he; // Se utiliza para convertir el nombre del host	a su dirección IP
	struct sockaddr_in servidorInfo; // dirección del server donde se conectara
	
	if ((he=gethostbyname(ip)) == NULL) 
	{
		perror("Error función gethostbyname()\n");
		exit(EXIT_FAILURE);
	}

  // hilo que envía pedidos
   
        //menu();
        //scanf("%i", &opcion_usuario);
        opcion_usuario=opcion;
        //preparo paquete
        enviar_paquete.identificador=opcion;

        // Establecemos servidorInfo con la dirección del server (esto habría que hacerlo siempre despues de que el coordinador le devuelva los datos del servidor)
        servidorInfo.sin_family = AF_INET;
        servidorInfo.sin_port = htons(PUERTO);
        servidorInfo.sin_addr = *((struct in_addr *)he->h_addr);
        bzero(&(servidorInfo.sin_zero), 8);
        
        // Creamos el socket
        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
        {
            perror("Error función socket()\n");
            exit(EXIT_FAILURE);
        }

          int optval = 1;
          if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) == -1) 
          {
              perror("Set socket option\n");        
              exit(errno);
          }

           /* Intentamos conectarnos con el servidor */

        if (connect(sockfd, (struct sockaddr *)&servidorInfo, sizeof(struct sockaddr)) == -1)
        {
            perror("Error función connect()\n");
            exit(EXIT_FAILURE);
        }

        switch (opcion_usuario) 
        {
            case DOWNLOAD:
                // download
                snprintf(enviar_paquete.data, MAXDATASIZE-1, "%s", route);
                enviar_paquete.tamanio_data = 0;

                printf("Usted pidió el archivo %s\n", enviar_paquete.data);

                enviar(sockfd, &enviar_paquete, sizeof(struct PAQUETE_SOCKET));

                int seguir_recibiendo = 1;

                FILE * archivo = fopen(destino, "w");

                if (archivo == NULL) {
                    perror("fopen\n");
                    exit(errno);
                }

                while (seguir_recibiendo) {
                    recibir(sockfd, &recibir_paquete, sizeof(struct PAQUETE_SOCKET));
                    if(recibir_paquete.identificador==ERROR){
                        seguir_recibiendo=0;
                        printf("%s", recibir_paquete.data);
                        exit(EXIT_FAILURE);
                    }
                    fwrite(recibir_paquete.data, sizeof(char), recibir_paquete.tamanio_data, archivo);
                    seguir_recibiendo = !recibir_paquete.ultimo_paquete;
                    enviar_paquete.identificador = ACK;
                    enviar(sockfd, &enviar_paquete, sizeof(struct PAQUETE_SOCKET));
                }

                printf("Descarga del archivo %s exitosa\n", enviar_paquete.data);
                fclose(archivo);

                break;
            case UPDATE:
                break;
            default:
                printf("La operación no existe.\n");
                exit(EXIT_FAILURE);
        }
        close(sockfd);
    
}
