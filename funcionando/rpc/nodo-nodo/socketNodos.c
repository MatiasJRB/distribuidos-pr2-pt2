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
#include "constantes.h"
// #include "protocolo.h"

//Hilo que atenderá los pedidos de otros nodos
pthread_t thread_a;

int downloadFile(char* ip, char* route, char* destino)
{
   return emisorPedidosNodo(ip, route, destino, DOWNLOAD);
   
}

int copyFile(char* ip, char* route, char* destino)
{
     return emisorPedidosNodo(ip,route,destino,COPY);
}

int removeFile(char * ip, char * route)
{
	return emisorPedidosNodo(ip,route,NULL,REMOVE);
	
}
int moveFile(char* ip, char* route,char* destino)
{
	return emisorPedidosNodo(ip,route,destino,MOVE);
}

void startListening(void *clnt)
{
    //Mando al hilo a escuchar en el puerto ingresado por el cliente
	pthread_create(&thread_a,NULL,receptorPedidosNodo,clnt);

}
void stopListening()
{
    pthread_join(thread_a, EXIT_SUCCESS);
}
