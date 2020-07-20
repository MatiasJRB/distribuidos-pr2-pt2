#include <stdio.h>
#include <stdlib.h>
#include "consultas.h"
#include <string.h>

#define N 100

// Funcion para verificar si un archivo/carpeta es valido
// Recibe un tipo (0=carpeta,1=archivo), un nombre y la ubicacion del nodo actual
// Retorna 0=falso, 1=verdadero y 2=tipoInvalido
int esValido(char tipo, char* Nombre, char* Ubicacion)
{
	if (tipo == '0')
	{
		struct archivo aux = *buscarCarpeta(Nombre);
		if (strcmp(aux.permiso,"N")==0) return 0;
		else return 1;	
	}
	else
		if (tipo == '1')
		{
			struct archivo aux = *buscarArchivo(Nombre,Ubicacion);
			if (strcmp(aux.permiso,"N")==0) return 0;
			else return 1;
		}
		else
			return 2;
}


// Funcion encargada del LS
// Recibe una ubicacion (carpeta actual)
// Retorna un string con todos los elementos en la carpeta
char* funcionListar(char* Direccion)
{	
	struct listado list = *funcionLS(Direccion);
	
	char temp[N];
	
	// Revisar porque es necesario esto, pero si no lo pones no anda
	strcpy(temp,"");
	
	int i;
	struct archivo archivoaux;
	char* nombreaux;
	
	for (i=0;i<list.cantidad;i++)
	{	
		archivoaux = list.elementos[i];
		nombreaux = archivoaux.nombre;
		strcat(temp, nombreaux);
		strcat(temp, ",");
	}


	int size = strlen(temp);
	char *toRet;
	toRet = malloc(sizeof(temp)*size);
	strcpy(toRet,temp);
	
	return (toRet);	
}

// Funcion para retornar la IP en la que se encuentra un archivo
// Recibe el nombre de un archivo
// Retorna un string con el IP del nodo que tiene el archivo
char* obtenerIP(char* Archivo, char* Ubicacion)
{	
	struct archivo aux = *buscarArchivo(Archivo, Ubicacion);
	char* toRet = aux.ip;
	
	return (toRet);	
}


int carpetaVacia(char* Nombre)
{
	struct listado aux = *funcionLS(Nombre);
	if (aux.cantidad == 0) return 1;
	else return 0;	
}

int insert(char tipo, char* Nombre,char* IP, char* Ubicacion)
{	//printf("INGRESE AL INSERT DEL COORDINADOR.\n");
	if (tipo == '0')
	{
		//printf("TIPO 0.\n");
		char aux[40]="raiz/";
		strcat(aux,Nombre);
		insertar(Nombre,"-","raiz","R","0","0",aux);
		return 1;
	}
	else
		if (tipo == '1')
		{
			//printf("TIPO 1.\n");
			char aux[40]="raiz/";
			if ( strcmp(Ubicacion,aux) !=0 ) {
				strcat(aux,Ubicacion);
				strcat(aux,"/");
				strcat(aux,Nombre);
			}
			insertar(Nombre,IP,Ubicacion,"W","0","1",aux);
			return 1;
		}
		else
			return 0;
}

int delete(char tipo, char* Nombre,char* IP, char* Ubicacion)
{
	//printf("Llegue al delete.\n");
	if (tipo == '0')
	{
		//printf("Tipo 0.\n");
		eliminar(Nombre,"-","raiz","R");
		return 1;
	}
	else
		if (tipo == '1')
		{
			//printf("Tipo 1.\n");
			eliminar(Nombre,IP,Ubicacion,"W");
			return 1;
		}
		else
			return 0;
}

//int main()
//{
	/*int res = esValido('1',"HolaMundo.txt","Carpeta1");
	printf("El resultado es: %i",res);
	printf("\n");
	
	insert('1',"HolaMundo.txt","102.102.102.102","Carpeta1");
	
	
	res = esValido('1',"HolaMundo.txt","Carpeta1");
	printf("El resultado es: %i",res);
	printf("\n");
	
	
	delete('1',"HolaMundo.txt","102.102.102.102","Carpeta1");
	
	res = esValido('1',"HolaMundo.txt","Carpeta1");
	printf("El resultado es: %i",res);
	printf("\n");
	
	*/
	
//	return 1;

//}
