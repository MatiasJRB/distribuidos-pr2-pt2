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
#include "constantes.h"

/*
Consideraciones a tener en cuenta:
    - sobrescribir archivos?
    - si hay algún problema y hacemos return ERROR, se cortaría todo. 
        una mejor solución quizás sería llamar a una función error() del shell para que notifique 
        al usuario con un mensaje en vez de terminar todo el sistema.
*/


static PAQUETE_SOCKET enviar_paquete, recibir_paquete;

//++++++++++++++++++++NODO CLIENTE+++++++++++++++++++++++++++++++++++++++++++++

int emisorPedidosNodo(char* ip, char* route, char* destino,int opcion) 
{
    int opcion_usuario=-1;
	int sockfd; 
	struct hostent *he; // Se utiliza para convertir el nombre del host	a su dirección IP
	struct sockaddr_in servidorInfo; // dirección del server donde se conectara
	
	if ((he=gethostbyname(ip)) == NULL) 
	{
		// printf("ip recibida: %s------------------------------\n", ip);
		// perror("Error función gethostbyname()\n");
		return ERROR;
	}

  // hilo que envía pedidos
   
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
            // perror("Error función socket()\n");
            return ERROR;
        }

          int optval = 1;
          if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) == -1) 
          {
              // perror("Set socket option\n");        
              return ERROR;
          }

           /* Intentamos conectarnos con el servidor */

        if (connect(sockfd, (struct sockaddr *)&servidorInfo, sizeof(struct sockaddr)) == -1)
        {
            // perror("Error función connect()\n");
            return ERROR;
        }

        switch (opcion_usuario) 
        {
            case DOWNLOAD:
                // download
                snprintf(enviar_paquete.data, MAXDATASIZE-1, "%s", route);
                enviar_paquete.tamanio_data = 0;

                // printf("Usted pidió el archivo %s\n", enviar_paquete.data);

                enviar(sockfd, &enviar_paquete, sizeof(struct PAQUETE_SOCKET));

                int seguir_recibiendo = 1;

                FILE * archivo = fopen(destino, "w");

                if (archivo == NULL) {
                    // perror("fopen\n");
                    return ERROR;
                }

                while (seguir_recibiendo) {
                    recibir(sockfd, &recibir_paquete, sizeof(struct PAQUETE_SOCKET));
                    if(recibir_paquete.identificador==ERROR){
                        seguir_recibiendo=0;
                        // printf("%s", recibir_paquete.data);
                        return ERROR;
                    }
                    fwrite(recibir_paquete.data, sizeof(char), recibir_paquete.tamanio_data, archivo);
                    seguir_recibiendo = !recibir_paquete.ultimo_paquete;
                    enviar_paquete.identificador = ACK;
                    enviar(sockfd, &enviar_paquete, sizeof(struct PAQUETE_SOCKET));
                }

                // printf("Descarga del archivo %s exitosa\n", enviar_paquete.data);
                fclose(archivo);
                break;
            case COPY:
                //copy
                snprintf(enviar_paquete.data, MAXDATASIZE-1, "%s %s ", route, destino);
                enviar_paquete.tamanio_data = strlen(destino)+strlen(route);

                // printf("Usted pidió copiar el archivo  %s en %s\n",route,destino);

                enviar(sockfd, &enviar_paquete, sizeof(struct PAQUETE_SOCKET));
                // Esperar respuesta del nodo que recibe el archivo
                recibir(sockfd, &recibir_paquete, sizeof(struct PAQUETE_SOCKET));
				// printf("%d\n",recibir_paquete.identificador);                
				if(recibir_paquete.identificador==ACK)
					return ACK;
				else if(recibir_paquete.identificador==ERROR)
					return ERROR;

                break;
            case REMOVE:
                enviar_paquete.tamanio_data = strlen(route);
                snprintf(enviar_paquete.data, MAXDATASIZE-1, "%s", route);
                // printf("Usted pidió copiar el archivo  %s en %s\n",route,destino);

                enviar(sockfd, &enviar_paquete, sizeof(struct PAQUETE_SOCKET));
                // Esperar respuesta del nodo que recibe el archivo
                recibir(sockfd, &recibir_paquete, sizeof(struct PAQUETE_SOCKET));

                if(recibir_paquete.identificador==ACK)
					return ACK;
				else if(recibir_paquete.identificador==ERROR)
					return ERROR;
		        break;
	    case MOVE:
		//copy
                snprintf(enviar_paquete.data, MAXDATASIZE-1, "%s %s ", route, destino);
                enviar_paquete.tamanio_data = strlen(destino)+strlen(route);
		
		enviar(sockfd, &enviar_paquete, sizeof(struct PAQUETE_SOCKET));
                // Esperar respuesta del nodo que recibe el archivo
                recibir(sockfd, &recibir_paquete, sizeof(struct PAQUETE_SOCKET));
				// printf("%d\n",recibir_paquete.identificador);                
				if(recibir_paquete.identificador==ACK)
					return ACK;
				else if(recibir_paquete.identificador==ERROR)
					return ERROR;
		break;
            default:
                // printf("La operación no existe.\n");
                return ERROR;
				break;
        }
        close(sockfd);
        return ACK;
    
}
