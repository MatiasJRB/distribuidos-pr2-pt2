#include <stdio.h>
#include <stdlib.h>
#include "consultas.h"
#include <string.h>

#define N 40


// Funcion encargada del LS
// Recibe una ubicacion (carpeta actual)
// Retorna un string con todos los elementos en la carpeta
char* funcionListar(char* Direccion)
{	
	struct listado list = *funcionLS(Direccion);
	
	char* temp[N];
	int i;
	struct archivo archivoaux;
	char * nombreaux;
	
	for (i=0;i<list.cantidad;i++)
	{	
		archivoaux= list.elementos[i];
		nombreaux = archivoaux.nombre;
		strcat(temp, nombreaux);
		strcat(temp, ",");
	}

	char* toRet = "";
	int size = strlen(temp);
	toRet = malloc(sizeof(*temp)*size);
	strcat(toRet,temp);
	
	return (toRet);	
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

int main()
{
	char *respuesta = funcionListar("Carpeta1");
	printf("%s",respuesta);
	printf("\n");
	
	char* IP = obtenerIP("ArchivoA.txt");
	printf("%s",IP);
	printf("\n");
	
	
	return 1;

}
