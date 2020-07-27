#include <stdio.h>
/* Archivo SocketNodos.h */
#ifndef SOCKETNODOS_H
#define SOCKETNODOS_H

extern int downloadFile(char* ip, char* route, char* destino);

extern int removeFile(char * ip, char * route);

extern int copyFile(char * ip, char * route, char * destino);

extern int moveFile(char *ip, char* route, char* destino);
// extern void uploadFile(FILE *file, char* ip, char* route);

extern void startListening(void * clnt);

extern void stopListening();

#endif /* SOCKETNODOS_H */
