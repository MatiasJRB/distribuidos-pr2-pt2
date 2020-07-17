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

#include "socketNodos.h"
#include "receptorPedidos.h"
#include "emisorPedidos.h"

#define PUERTO 15000 // puerto en el que cada nodo va a escuchar

#define MAXDATASIZE 512
#define DOWNLOAD 1
#define UPLOAD 2
#define UPDATE 3
#define ERROR 4

//Hilo que atenderá los pedidos de otros nodos
pthread_t thread_a;

int downloadFile(char* ip, char* route, char* destino)
{
    emisorPedidosNodo(ip, route, destino, DOWNLOAD);
    return 1;
}

void uploadFile(FILE *file, char* ip, char* route)
{

}

void startListening()
{
    //Mando al hilo a escuchar en el puerto ingresado por el cliente
	pthread_create(&thread_a,NULL,receptorPedidosNodo,NULL);

}
void stopListening()
{
    pthread_join(thread_a, EXIT_SUCCESS);
}