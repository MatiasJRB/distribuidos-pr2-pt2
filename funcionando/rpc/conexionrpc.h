#ifndef CONEXIONRPC_H_
#define CONEXIONRPC_H_

	char* ls(char* dir);
	char* getaddress(char* archivo, char* ubicacion);
	int exists(char tipo, char* nombre; char* ubicacion);
	int report_create(char tipo, char* nombre, char* ip, char* ubicacion);
	int report_delete(char tipo, char* nombre, char* ip, char* ubicacion);
	int is_empty(char* nombre);


#endif
