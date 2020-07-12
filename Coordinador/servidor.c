#include <stdio.h>
#include <time.h>
#include <string.h>
#include "protocolo.h"

#include "consultas.c"

// Estructuras de consultas
struct archivo 
{
    char *nombre;            // Nombre del archivo
    char *ip;                // ip del archivo
    char *direccion;         // direccion del padre del archivo
    char *permiso;           // w o r, segun escritura o lectura
    int  version;            // numero de version con numero 0, es la mas actual
    int  tipo;               // 0 si es archivo, 1 si es carpeta
    char *ruta;              // ruta completa del archivo
};

struct listado
{
    struct archivo elementos[40];
    int cantidad;    
};

// Verifico que la carpeta realmente exista en el sistema
// Retorno 1 si existe, 0 caso contrario
int carpetaValida(char* nombre)
{
	int toRet=0;
	listado list = funcionLS(msg->Mensaje_val);
	if (list[0] != NULL) toRet=1;

	return toRet;
}

// Funcion ls
// Inicialmente solo retorna el nombre de los archivos y carpetas en el directorio pasado por parametro, consultar cuantas funcionalidades extras agregar
Mensaje *ls_1_svc(Mensaje *msg, struct svc_req *req)
{

	listado list = funcionLS(msg->Mensaje_val);

	char* toRet="";

	// En caso de implementar funcionalidades extras, aca vendria algun case con la seleccion a parametros a retornar
	int i;
	for (i=0;i<list->cantidad;i++)
		{
			strcat(toRet, list[i]->nombre);
			strcat(toRet, ",");
		}
	
	return (&toRet);
	
}

// Funcion inicial del VIM
// Despues te deben avisar, por otra peticion, cuando se guarde el archivo nuevo
Mensaje *getaddress_1_svc(Mensaje *msg, struct svc_req *req)
{
	archivo toRet = buscarArchivo(msg->Mensaje_val);
	return (&toRet->ip);
}



/**
Mensaje *ls_1_svc(Mensaje *msg, struct svc_req *req)
{
	if (carpetaValida(msg))
	{	
	
		listado list = funcionLS(msg->Mensaje_val);

		char* toRet[list->cantidad];

		// En caso de implementar funcionalidades extras, aca vendria algun case con la seleccion a parametros a retornar
		int i;
		for (i=0;i<list->cantidad;i++)
			toRet[i]=list[i]->nombre;
	
		return (&toRet);
	}

	return &"Carpeta no existente en el sistema";
}

// Funcion inicial del VIM
// Despues te deben avisar, por otra peticion, cuando se guarde el archivo nuevo
Mensaje *getaddress_1_svc(Mensaje *msg, struct svc_req *req)
{
	
	if (carpetaValida(msg))
	{	
		archivo toRet = buscarArchivo(msg->Mensaje_val);

		return (&toRet->ip);
	}

	return &"Carpeta no existente en el sistema";
}

