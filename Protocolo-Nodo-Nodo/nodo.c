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

/*
Consideraciones a tener en cuenta:
    - sobrescribir archivos? 
    - si hay algún problema y hacemos exit(), se cortaría todo. 
        una mejor solución quizás sería llamar a una función error() del shell para que notifique 
        al usuario con un mensaje en vez de terminar todo el sistema.
    - un nodo por host ya que todos los nodos escuchan en el mismo puerto
*/

//------------------------------MAIN----------------------------------------


int main(int argc, char *argv[]) 
{
	if (argc < 5) 
	{
		printf("Cantidad invalida de parametros\n");
		exit(EXIT_FAILURE);
	}
	/*
	//Hilo que atenderá los pedidos de otros nodos
	pthread_t thread_a;
    //Meter el phtread_Create dentro de socketNodos.h
    */
   	//Mando al hilo a escuchar en el puerto ingresado por el cliente
	startListening();
    
    if (!strcmp(argv[4], "descargar")) downloadFile(argv[1],argv[2],argv[3]); //comentar en alguna de las dos VM y hacer make

    stopListening();
	return 0;
}
