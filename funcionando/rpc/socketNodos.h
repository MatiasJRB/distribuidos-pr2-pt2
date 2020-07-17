#include <stdio.h>
/* Archivo SocketNodos.h */
#ifndef SOCKETNODOS_H
#define SOCKETNODOS_H
/*
Funcion que obtiene el archivo indicado en "ruta" en el nodo
con ip "ip" y puerto "puerto"

Devuelve en un puntero a FILE* el archivo correspondiente 

*/
extern int downloadFile(char* ip, char* route, char* destino);

/*
 * descripción...
 */
extern int removeFile(char * ip, char * route);

// extern int copyFile(char * ip, char * route, char * destino);

/*
Funcion que envía el archivo "file" al nodo en el ip "ip",
puerto "puerto", y ruta "route"
*/
extern void uploadFile(FILE *file, char* ip, char* route);


extern void startListening();

extern void stopListening();
#endif /* SOCKETNODOS_H */
