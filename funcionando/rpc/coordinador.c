#include <stdio.h>
#include <stdlib.h>
#include "consultas.h"
#include <string.h>

#define N 8192

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
	//printf("Voy a guardar.\n");
	//printf("La cantidad total a leer es: %d\n",list.cantidad);
	for (i=0;i<list.cantidad;i++)
	{	
		//printf("#%d\n",i);
		archivoaux = list.elementos[i];
		nombreaux = archivoaux.nombre;
		//printf("Elemento Nombre %s\n",nombreaux);
		strcat(temp, nombreaux);
		strcat(temp, ",");	
	}
	//printf("Termine.\n");

	int size = strlen(temp);
	//printf("size  = %d.\n",size);
	char *toRet;
	toRet = malloc(N*sizeof(char));
	strcpy(toRet,temp);
	//printf("Me rompo en el coordinador.\n");
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
	//printf("entre al insert.\n");
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
			char aux[40]="raiz";
			if ( strcmp(Ubicacion,aux) !=0 ) 
			{
				strcat(aux,"/");
				strcat(aux,Ubicacion);
				strcat(aux,"/");
				strcat(aux,Nombre);
			}
			else 
			{
				strcat(aux,"/");
				strcat(aux,Nombre);
			}
			insertar(Nombre,IP,Ubicacion,"W","0","1",aux);
			//printf("Termine de insertar \n");
			int pararetornar = 1;
			return pararetornar;
		}
		else
			return 0;
}

int delete(char tipo, char* Nombre,char* IP, char* Ubicacion)
{
	//printf("Llegue al delete.\n");
	//printf("La ip recibida es %s.\n",IP);
	if (tipo == '0')
	{
		//printf("Voy a eliminar una carpeta.\n");
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

int modificarIP(char* NuevaIP, char* Archivo, char* Carpeta)
{
	struct archivo aux = *buscarArchivo(Archivo,Carpeta);

	updateip(Archivo,NuevaIP,aux.direccion,aux.permiso,"0","1",aux.ruta);
	return 1;
}



int isFile(char* Nombre, char* Ubicacion)
{
	struct archivo aux = *buscarArchivo(Nombre,Ubicacion);

	if (strcmp(aux.permiso,"N") ==0) return 0;
	else return 1;
}

int modificarDirectorio(char* Nombre, char* CarpetaVieja, char* CarpetaNueva)
{
	char dir[40]="raiz/";

	if (strcmp(CarpetaNueva,"raiz") != 0)
	{
		strcat(dir,CarpetaNueva);
		strcat(dir,"/");
	}
	strcat(dir,Nombre);

	struct archivo aux = *buscarArchivo(Nombre,CarpetaVieja);
	updatePosicion(Nombre,aux.ip,CarpetaNueva,aux.permiso,"0","1",dir);

	return 1;
}

char* obtenerArchivoConIp(char* ip)
{
	char* resultado = malloc(2048*sizeof(char));
	memset(resultado,'\0',1);
	strcpy(resultado,getFilesByIp(ip));
	//printf("Archivos son: %s\n",resultado);
	return resultado;
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
