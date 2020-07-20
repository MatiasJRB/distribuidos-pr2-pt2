#ifndef COMUNICACION_H_
#define COMUNICACION_H_

#include "protocolo.h"

	// devuelve un listado de los archivos y carpetas
	// que se encuentran en el directorio
	char* ls(CLIENT* clnt, char* dir);
	
	// devuelve la direccion ip donde se encuentra un archivo
	// nombre es el nombre del archivo
	// ubicacion es la carpeta dentro del filesystem
	char* getaddress(CLIENT* clnt, char* nombre, char* ubicacion);
	
	// indica si un archivo o directorio existe
	// tipo indica si es un archivo o un directorio
	// nombre es el nombre del archivo o directorio
	// en caso de ser un archivo, ubicacion es la carpeta dentro del fs
	// retorna 0 si no existe, 1 si existe y 2 si el tipo es invalido
	int exists(CLIENT* clnt, char tipo, char* nombre, char* ubicacion);
	
	// reporta la creacion de un archivo o carpeta
	// tipo indica si es un archivo o un directorio
	// nombre es el nombre del archivo o directorio
	// ip es la direccion ip de quien lo aloja
	// en caso de ser un archivo, ubicacion es la carpeta dentro del fs
	int report_create(CLIENT* clnt, char tipo, char* nombre, char* ip, char* ubicacion);
	
	// reporta la eliminacion de un archivo o carpeta
	// tipo indica si es un archivo o un directorio
	// nombre es el nombre del archivo o directorio
	// ip es la direccion ip de quien lo aloja
	// en caso de ser un archivo, ubicacion es la carpeta dentro del fs
	int report_delete(CLIENT* clnt, char tipo, char* nombre, char* ip, char* ubicacion);
	
	// indica si un directorio esta vacio
	int is_empty(CLIENT* clnt, char* nombre);

#endif
