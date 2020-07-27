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

#include <sys/stat.h>


#include "receptorPedidos.h"
#include "enviarRecibir.h"
#include "constantes.h"
#include "../protocolo.h"

static PAQUETE_SOCKET enviar_paquete, recibir_paquete;

void move(CLIENT * clnt, int newfd)
{
	char * ruta = strtok(recibir_paquete.data," ");
	char * destino= strtok(NULL," ");
	char rutita[256];
	memset(rutita,'\0',1);
	getcwd(rutita,100);
	// printf("La ruta actual es: %s\n", rutita); 
	long unsigned int i=0;
	for(i=1;i<strlen(ruta);i++)
	{
	  ruta[i-1]=ruta[i];
	}
	ruta[i-1]='\0';
	
	for(i=1;i<strlen(destino);i++)
	{
	  destino[i-1]=destino[i];
	}
	destino[i-1]='\0';
	//strcpy(ruta,ruta+1);
	if(clnt == (CLIENT*)NULL)
	{
		clnt_pcreateerror("localhost");
		// printf("error cliente copy");
		exit(2);
	}
	/*	
	Mensaje msg ;
	msg.Mensaje_val=destino;
	msg.Mensaje_len=strlen(destino);

	int * to_return =is_valid_1(&msg,clnt);
	printf("%i",*to_return);
	*/
	FILE * archivoNuevo;
	FILE * archivoOrigen;

	char * nombreArchivo;
	char * ruta_aux;
	ruta_aux = strdup(ruta);
	char * pasador= strtok(ruta_aux,"/");
	while(pasador!=NULL)
	{
		nombreArchivo=pasador;
		pasador= strtok(NULL,"/");
		
	}
	// printf("strlen destino: %i\n", strlen(destino));

	if (strlen(destino)) {
		mkdir(destino, 0777); //creo la carpeta destino
		strcat(destino,"/");
	}

	strcat(destino,nombreArchivo);

	// printf("destino: %s\nruta: %s\n", destino, ruta);
	if (!strcmp(destino,ruta)) {
		// printf("son el mismo archivo ...\n");
		enviar_paquete.identificador=ERROR;
		enviar(newfd,&enviar_paquete,sizeof(struct PAQUETE_SOCKET));
		exit(ERROR);
	}

	// printf("archivo a crear en %s\n", destino);
	
	archivoOrigen = fopen(ruta,"r");
	if(archivoOrigen == NULL)
	{
		enviar_paquete.identificador=ERROR;
		enviar(newfd,&enviar_paquete,sizeof(struct PAQUETE_SOCKET));
		exit(ERROR);
	}

	archivoNuevo = fopen(destino,"w+");
	if(archivoNuevo == NULL)
	{
		fclose(archivoOrigen);
		enviar_paquete.identificador=ERROR;
		enviar(newfd,&enviar_paquete,sizeof(struct PAQUETE_SOCKET));
		exit(ERROR);
	}

	char ch;
	while ((ch = fgetc(archivoOrigen)) != EOF)
	{
		fputc(ch, archivoNuevo);
	}

	fclose(archivoOrigen);
	fclose(archivoNuevo);
	// printf("%s\n",recibir_paquete.data);
	int result = remove(ruta);
	
	if(!result)//es exitoso
	{	
		enviar_paquete.identificador=ACK;
		enviar(newfd,&enviar_paquete,sizeof(struct PAQUETE_SOCKET));	
	}
	else
	{
		enviar_paquete.identificador=ERROR;
		enviar(newfd,&enviar_paquete,sizeof(struct PAQUETE_SOCKET));
		exit(ERROR);
	}
}

void * receptorPedidosNodo(void * arg)
{
	CLIENT * clnt = arg;
	int sockfd; // el servidor escuchara por sockfd
	int newfd; // las transferencias de datos se realizara mediante newfd
	
	struct sockaddr_in servidorInfo; // contendra la IP y puerto local
	struct sockaddr_in clienteInfo; // contendra IP y puerto del cliente
	
	int sin_size; // contendra el tamaño de la escructura sockaddr_in
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
	{
		// perror("Error en la creación del socket\n");
		exit(EXIT_FAILURE);
	}

    int optval = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) == -1) {
        // perror("Set socket option\n");        
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
		// perror("Error en función bind()\n");
		exit(errno);
	}
	// Habilita el socket para recibir conexiones, con una cola de x conexiones en espera como máximo
	if (listen(sockfd, 10) == -1)
	{
		// perror("Error en funcion listen()\n");
		exit(errno);
	}
	while(1) 
	{ 
		
		// loop que llama a accept()
		sin_size = sizeof(struct sockaddr_in);
		// Se espera por conexiones
		if ((newfd = accept(sockfd, (struct sockaddr *)&clienteInfo, (socklen_t *) &sin_size)) == -1)
		{
			// perror("Error en función accept()\n");
			exit(errno);
		}
		// printf("Pedido desde el nodo con IP: %s\n", inet_ntoa(clienteInfo.sin_addr));
		// printf("Conexion desde puerto: %d \n", ntohs(clienteInfo.sin_port));
		char* ruta;
		char* destino;

		char* nombreArchivo;
		char* pasador;
		if (!fork()) 
		{ 
			// Comienza el proceso hijo, enviamos los datos mediante newfd
            recibir(newfd,&recibir_paquete,sizeof(struct PAQUETE_SOCKET));
			// printf("Paquete recibido con identificador %d, tamaño %d, y ruta %s \n\n",recibir_paquete.identificador, recibir_paquete.tamanio_data, recibir_paquete.data);
			FILE *archivo;
			switch(recibir_paquete.identificador)
			{
				//Descarga del archivo
				case DOWNLOAD:
						// printf("Pedido de Archivo\n");
                        archivo = fopen(recibir_paquete.data,"r");
                        if(archivo==NULL){
                            // printf("No existe el archivo\n");
                            enviar_paquete.identificador=ERROR;
                            snprintf(enviar_paquete.data, MAXDATASIZE-1, "El archivo que pidió no existe.");
                            enviar(newfd, &enviar_paquete ,sizeof(struct PAQUETE_SOCKET));
                            exit(EXIT_FAILURE);
					    }
                        int leidos = 0;
                        enviar_paquete.ultimo_paquete = 0;
                        while(!enviar_paquete.ultimo_paquete){
                            leidos = fread(enviar_paquete.data, sizeof(char), MAXDATASIZE, archivo);
                            enviar_paquete.identificador=DOWNLOAD;
                            enviar_paquete.tamanio_data = leidos;
                            enviar_paquete.ultimo_paquete = feof(archivo);
                            enviar(newfd, &enviar_paquete,sizeof(struct PAQUETE_SOCKET)); 

                            recibir(newfd,&recibir_paquete,sizeof(struct PAQUETE_SOCKET));
                            if (recibir_paquete.identificador != ACK) {
                                // printf("Error en la transferencia del archivo.\n");
                                exit(EXIT_FAILURE);
                            }
                        }
                        fclose(archivo);
                        // printf("\nArchivo enviado\n");
                        break;
				case COPY:
						
						ruta = strtok(recibir_paquete.data," ");
						destino= strtok(NULL," ");
						char rutita[256];
						memset(rutita,'\0',1);
						getcwd(rutita,100);
						// printf("La ruta actual es: %s\n", rutita); 
						long unsigned int i=0;
						for(i=1;i<strlen(ruta);i++)
						{
						  ruta[i-1]=ruta[i];
						}
						ruta[i-1]='\0';
						
						for(i=1;i<strlen(destino);i++)
						{
						  destino[i-1]=destino[i];
						}
						destino[i-1]='\0';
						//strcpy(ruta,ruta+1);
						if(clnt == (CLIENT*)NULL)
						{
							clnt_pcreateerror("localhost");
							// printf("error cliente copy");
							exit(2);
						}
						/*	
						Mensaje msg ;
						msg.Mensaje_val=destino;
						msg.Mensaje_len=strlen(destino);

						int * to_return =is_valid_1(&msg,clnt);
						printf("%i",*to_return);
						*/
						FILE * archivoNuevo;
						FILE * archivoOrigen;

						nombreArchivo="";
						char * ruta_aux;
						ruta_aux = strdup(ruta);
						pasador= strtok(ruta_aux,"/");
						while(pasador!=NULL)
						{
						    nombreArchivo=pasador;
						    pasador= strtok(NULL,"/");
						    
						}
						// printf("strlen destino: %i\n", strlen(destino));

						if (strlen(destino)) {
							mkdir(destino, 0777); //creo la carpeta destino
							strcat(destino,"/");
						}

						strcat(destino,nombreArchivo);

						// printf("destino: %s\nruta: %s\n", destino, ruta);
						if (!strcmp(destino,ruta)) {
							// printf("son el mismo archivo ...\n");
							enviar_paquete.identificador=ERROR;
							enviar(newfd,&enviar_paquete,sizeof(struct PAQUETE_SOCKET));
							exit(ERROR);
						}

						// printf("archivo a crear en %s\n", destino);
						
						archivoOrigen = fopen(ruta,"r");
						if(archivoOrigen == NULL)
						{
							enviar_paquete.identificador=ERROR;
							enviar(newfd,&enviar_paquete,sizeof(struct PAQUETE_SOCKET));
							exit(ERROR);
						}

						archivoNuevo = fopen(destino,"w+");
						if(archivoNuevo == NULL)
						{
							fclose(archivoOrigen);
							enviar_paquete.identificador=ERROR;
							enviar(newfd,&enviar_paquete,sizeof(struct PAQUETE_SOCKET));
							exit(ERROR);
						}

						char ch;
						while ((ch = fgetc(archivoOrigen)) != EOF)
						{
							fputc(ch, archivoNuevo);
						}

						fclose(archivoOrigen);
						fclose(archivoNuevo);
						
						enviar_paquete.identificador=ACK;
						enviar(newfd,&enviar_paquete,sizeof(struct PAQUETE_SOCKET));


						break;
				case REMOVE:
						i=0;
						for(i=1;i<strlen(recibir_paquete.data);i++)
						{
						  recibir_paquete.data[i-1]=recibir_paquete.data[i];
						}
						recibir_paquete.data[i-1]='\0';

						int result = remove(recibir_paquete.data);
						
						if(!result)//es exitoso
						{	
							enviar_paquete.identificador=ACK;
							enviar(newfd,&enviar_paquete,sizeof(struct PAQUETE_SOCKET));	
						}
						else
						{
							enviar_paquete.identificador=ERROR;
							enviar(newfd,&enviar_paquete,sizeof(struct PAQUETE_SOCKET));
							exit(ERROR);
						}
						break;
				/*case UPLOAD:
						printf("Envio de Archivo\n");
						break;

				case UPDATE:
						printf("Actualizacion de Archivo\n");
						break;*/
				case MOVE:
						move(clnt, newfd);
						break;
					
				default:	
                        // printf("No existe la operación.\n");
						exit(ERROR);
						break;
			}
			close(newfd);
			exit(EXIT_SUCCESS);
		} 
		close(newfd);
	}
	return NULL;
}
