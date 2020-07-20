/* Archivo SocketNodos.h */
#ifndef ENVIARRECIBIR_H
#define ENVIARRECIBIR_H
/*
Funcion que obtiene el archivo indicado en "ruta" en el nodo
con ip "ip" y puerto "puerto"

Devuelve en un puntero a FILE* el archivo correspondiente 

*/
extern int recibir(int fd, void * mensaje, int size);

/*
Funcion que envía el archivo "file" al nodo en el ip "ip",
puerto "puerto", y ruta "route"
*/
extern int enviar(int fd, void * mensaje, int size);

#endif /* ENVIARRECIBIR_H */
