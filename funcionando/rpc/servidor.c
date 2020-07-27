#include <stdio.h>
#include <string.h>
#include "protocolo.h"
#include "coordinador.h"

Mensaje *ls_1_svc(Mensaje *msg, struct svc_req *req)
{	
	static Mensaje to_return;
	to_return.Mensaje_val = funcionListar(msg->Mensaje_val);
	to_return.Mensaje_len = 1 + strlen(to_return.Mensaje_val);
	//printf("Me rompo en el servidor.\n");
	return (&to_return);	
}

Mensaje *getaddress_1_svc(Mensaje *msg, struct svc_req *req)
{
	static Mensaje to_return;
	char* delimiter = ",";
	char* nombre = strtok(msg->Mensaje_val, delimiter);
	char* ubicacion = strtok(NULL, delimiter);
	to_return.Mensaje_val = obtenerIP(nombre, ubicacion);
	to_return.Mensaje_len = 1 + strlen(to_return.Mensaje_val);
	return (&to_return);
}

int *exists_1_svc(Mensaje *msg, struct svc_req *req)
{
	//printf("servidor.c::is_valid_1_svc\n\n");
	//printf("msg->Mensaje_val = %s.\n",msg->Mensaje_val);
	//printf("msg->Mensaje_len = %d.\n",msg->Mensaje_len);
	
	static int to_return;
	char* delimiter = ",";
	char* tipo = strtok(msg->Mensaje_val, delimiter);
	char* nombre = strtok(NULL, delimiter);
	char* ubicacion = NULL;
	if(*tipo == '1')
	{
		ubicacion = strtok(NULL, delimiter);
	}
	to_return = esValido(*tipo, nombre, ubicacion);
	return &to_return;
}

int *report_update_1_svc(Mensaje *msg, struct svc_req *req)
{
	static int to_return;
	//sin implementar
	//tengo que llamar a una funcion de coordinador.h
    char* delimiter = ",";
    char* nombre = strtok(msg->Mensaje_val, delimiter);
    char* ip = strtok(NULL, delimiter);
    char* carpeta = strtok(NULL, delimiter);
    to_return = modificarIP(ip, nombre, carpeta);
	return(&to_return);
}

int *report_create_1_svc(Mensaje *msg, struct svc_req *req)
{
	static int to_return;
	//printf("Mensaje_val = %s.\n",msg->Mensaje_val);
	char* delimiter = ",";
	char* tipo = strtok(msg->Mensaje_val, delimiter);
	char* nombre = strtok(NULL, delimiter);
	char* ubicacion = NULL;
	char* IP = strtok(NULL, delimiter);
	if(*tipo == '1')
	{
		ubicacion = strtok(NULL, delimiter);
	}
	//printf("TIPO: %s.\n",tipo);
	//printf("NOMBRE: %s.\n",nombre);
	//printf("UBICACION: %s.\n",ubicacion);
	//printf("IP: %s.\n",IP);
	int resultado = insert(*tipo, nombre, IP, ubicacion);
	//printf("Llegue al to_return = %d\n",resultado);
	to_return = resultado;
	return &to_return;
}

int *report_update_directory_1_svc(Mensaje *msg, struct svc_req *req)
{
	static int to_return;
	char* delimiter = ",";
    char* nombre = strtok(msg->Mensaje_val, delimiter);
    char* carpetaActual = strtok(NULL, delimiter);
    char* carpetaDestino = strtok(NULL, delimiter);
    to_return = modificarDirectorio(nombre, carpetaActual, carpetaDestino);
	return(&to_return);
}

int *report_delete_1_svc(Mensaje *msg, struct svc_req *req)
{
	/*printf("Reporto el delete de %s.\n",msg->Mensaje_val);
	char str[64];
	strcpy(str,"");
	char* aux = "Terminado prro";
	strcat(str, aux);
	static Mensaje to_return;
	
	to_return.Mensaje_len =	64;
	to_return.Mensaje_val =	str;
	*/
	
	static int to_return;
	//printf("Mensaje_size = %d.\n",msg->Mensaje_len);
	//printf("Mensaje_val = %s.\n",msg->Mensaje_val);
	char* delimiter = ",";
	char* tipo = strtok(msg->Mensaje_val, delimiter);
	char* nombre = strtok(NULL, delimiter);
	char* ubicacion = NULL;
	char* IP = strtok(NULL, delimiter);
	if(*tipo == '1')
	{
		ubicacion = strtok(NULL, delimiter);
	}
	//printf("TIPO: %s\n",tipo);
	//printf("NOMBRE: %s\n",nombre);
	//printf("UBICACION: %s\n",ubicacion);
	//printf("IP: %s\n",IP);
	to_return = delete(*tipo, nombre, IP, ubicacion);
	
	return (&to_return);
}

int *is_empty_1_svc(Mensaje *msg, struct svc_req *req)
{
	static int to_return;
	to_return = carpetaVacia(msg->Mensaje_val);
	return &to_return;
}

int *is_file_1_svc(Mensaje *msg, struct svc_req *req)
{
        static int to_return;
        char* delimiter = ",";
        char* nombre = strtok(msg->Mensaje_val, delimiter);
        char* carpeta = strtok(NULL, delimiter);
        to_return = isFile(nombre, carpeta);
        
        return (&to_return);
}

Mensaje *get_files_ip_1_svc(Mensaje *msg, struct svc_req *req)
{
	
	//printf("Mensaje_size = %d.\n",msg->Mensaje_len);
	//printf("Mensaje_val = %s.\n",msg->Mensaje_val);
	static Mensaje to_return;
	char* resultado = obtenerArchivoConIp(msg->Mensaje_val);
	//printf("Return: %s.\n",resultado);
	to_return.Mensaje_val = resultado;
	to_return.Mensaje_len = 1+strlen(resultado);
	return &to_return;
}

