#ifndef COMUNICACION_H_
#define COMUNICACION_H_

#include "protocolo.h"

	char* ls(CLIENT* clnt, char* dir);
	char* getaddress(CLIENT* clnt, char* nombre, char* ubicacion);
	int exists(CLIENT* clnt, char tipo, char* nombre, char* ubicacion);
	int report_create(CLIENT* clnt, char tipo, char* nombre, char* ip, char* ubicacion);
	int report_delete(CLIENT* clnt, char tipo, char* nombre, char* ip, char* ubicacion);
	int is_empty(CLIENT* clnt, char* nombre);

#endif
