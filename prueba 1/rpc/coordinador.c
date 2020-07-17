#include <stdio.h>
#include <stdlib.h>
#include "consultas.h"
#include "coordinador.h"
#include <string.h>

#define N 400


// Funcion encargada del LS
// Recibe una ubicacion (carpeta actual)
// Retorna un string con todos los elementos en la carpeta
char* funcionListar(char* Direccion)
{	
	struct listado list = *funcionLS(Direccion);
	char* temp = malloc(N * sizeof(char));
	memset(temp,'\0',1);
	for (int i=0;i<list.cantidad;i++)
	{	
		strcat(temp, list.elementos[i].nombre);
		strcat(temp, ",");
	}
	return (temp);	
}

// Funcion para retornar la IP en la que se encuentra un archivo
// Recibe el nombre de un archivo
// Retorna un string con el IP del nodo que tiene el archivo
char* obtenerIP(char* Archivo)
{	
	struct archivo aux = *buscarArchivo(Archivo);
	char* toRet = aux.ip;
	
	return (toRet);	
}

//~ int main()
//~ {
	//~ while(getc(stdin) != 'q')
	//~ {
	//~ char *respuesta = funcionListar("Carpeta1");
	//~ printf("%s",respuesta);
	//~ printf("\n");
	//~ free(respuesta);
	
	//~ char* IP = obtenerIP("ArchivoA.txt");
	//~ printf("%s",IP);
	//~ printf("\n");
	//~ free(IP);
	//~ }
	
	//~ return 0;

//~ }
