#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "protocolo.h"
#include "comunicacion.h"

char* sacar_barra(char* src)
{
	char* to_return = src;
	if(*src == '/')
	{
		to_return = src + sizeof(char);
	}
	return to_return;
}

char* ls(CLIENT* clnt, char* dir)
{
	dir = sacar_barra(dir);
    Mensaje to_send =
    {
        1 + strlen(dir),
        dir,
    };
    Mensaje* rcv = ls_1(&to_send, clnt);
    return rcv->Mensaje_val;
}

char* getaddress(CLIENT* clnt, char* nombre, char* ubicacion)
{
    //necesitamos por lo menos 3 bytes
    //dos para comas y uno para el terminador nulo
    int size = 3;
    ubicacion = sacar_barra(ubicacion);
	size = size + strlen(nombre) + strlen(ubicacion);
    char buf[size];
    buf[0] = '\0';
    strcat(buf,nombre);
    strcat(buf,",");
    strcat(buf,ubicacion);
    Mensaje to_send =
    {
        1 + strlen(buf),
        buf,
    };
    Mensaje* rcv = getaddress_1(&to_send, clnt);
	return rcv->Mensaje_val;
}

int exists(CLIENT* clnt, char tipo, char* nombre, char* ubicacion)
{
    //necesitamos por lo menos 3 bytes
    //uno para el tipo, uno para una coma y uno para el terminador nulo
    int size = 3;
    //es un archivo
    if(tipo == TIPOARCHIVO)
    {
        ubicacion = sacar_barra(ubicacion);
		size = size + 1 + strlen(ubicacion);
    }
    if(tipo == TIPOCARPETA)
    {
		nombre = sacar_barra(nombre);
	}
	size = size + strlen(nombre);
    char buf[size];
    buf[0] = tipo;
    buf[1] = '\0';
    strcat(buf,",");
    strcat(buf,nombre);
    if(tipo == TIPOARCHIVO)
    {   
        strcat(buf,",");
        strcat(buf,ubicacion);
    }
    Mensaje to_send =
    {
        1 + strlen(buf),
        buf,
    };
    int to_return = *exists_1(&to_send,clnt);
    return to_return;
}

int report_create(CLIENT* clnt, char tipo, char* nombre, char* ip, char* ubicacion)
{
    //necesitamos por lo menos 5 bytes
    int size = 5;
    //es un archivo
    if(tipo == TIPOARCHIVO)
    {
        ubicacion = sacar_barra(ubicacion);
		size = size + 1 + strlen(ubicacion);
    }
    if(tipo == TIPOCARPETA)
    {
		nombre = sacar_barra(nombre);
	}
    size = size + strlen(nombre) + strlen(ip);
    char buf[size];
    buf[0] = tipo;
    buf[1] = '\0';
    strcat(buf,",");
    strcat(buf,nombre);
	strcat(buf,",");
    strcat(buf,ip);
    if(tipo == TIPOARCHIVO)
    {   
        strcat(buf,",");
        strcat(buf,ubicacion);
    }
    Mensaje to_send =
    {
        1 + strlen(buf),
        buf,
    };
    int to_return = *report_create_1(&to_send,clnt);
    return to_return;
}

int report_update_directory(CLIENT* clnt,char* nombre, char* ubicacionActual, char* ubicacionDestino)
{
        // el mensaje que tengo que crear es nombre,ubicacion\0
        ubicacionDestino = sacar_barra(ubicacionDestino);
        int size=strlen(nombre)+strlen(ubicacionActual)+strlen(ubicacionDestino)+3;//el 1 es la ,
        char buf[size];
        strcpy(buf,"");
        memset(buf,'\0',1);
        strcat(buf,nombre);
        strcat(buf,",");
        strcat(buf,ubicacionActual);
        strcat(buf,",");
        strcat(buf,ubicacionDestino);
        Mensaje to_send =
        {
            1 + strlen(buf),
            buf,
        };
        int to_return = *report_update_directory_1(&to_send,clnt);
        return to_return;
}

int report_delete(CLIENT* clnt, char tipo, char* nombre, char* ip, char* ubicacion)
{
    //necesitamos por lo menos 4 bytes
    int size = 5;
    //es un archivo
    if(tipo == TIPOARCHIVO)
    {
        ubicacion = sacar_barra(ubicacion);
		size = size + 1 + strlen(ubicacion);
    }
    if(tipo == TIPOCARPETA)
    {
		nombre = sacar_barra(nombre);
	}
	size = size + strlen(nombre) + strlen(ip);
    char buf[size];
    buf[0] = tipo;
    buf[1] = '\0';
    strcat(buf,",");
    strcat(buf,nombre);
	strcat(buf,",");
    strcat(buf,ip);
    if(tipo == TIPOARCHIVO)
    {   
        strcat(buf,",");
        strcat(buf,ubicacion);
    }
    Mensaje to_send =
    {
        1 + strlen(buf),
        buf,
    };
    int to_return = *report_delete_1(&to_send,clnt);
    return to_return;
}

int is_empty(CLIENT* clnt, char* nombre)
{
	nombre = sacar_barra(nombre);
	Mensaje to_send =
    {
        1 + strlen(nombre),
        nombre,
    };
    int rcv = *is_empty_1(&to_send, clnt);
    return rcv;
}

int report_update(CLIENT* clnt, char* nombre, char* ip, char* ubicacion)
{
    //necesitamos por lo menos 3 bytes
    int size = 3;  
    ubicacion = sacar_barra(ubicacion);
    size = size + strlen(nombre) + strlen(ip) + strlen(ubicacion);
    char buf[size];
    buf[0] = '\0';
    strcat(buf,nombre);  
    strcat(buf,",");
    strcat(buf,ip);
    strcat(buf,",");
    strcat(buf,ubicacion);
    Mensaje to_send =
    {
        1 + strlen(buf),
        buf,
    };
    int to_return = *report_update_1(&to_send,clnt);
    return to_return;
}

int is_file(CLIENT* clnt, char* nombre, char* ubicacion)
{
     //necesitamos por lo menos 3 bytes
    int size = 2;  
    ubicacion = sacar_barra(ubicacion);
    size = size + strlen(nombre) + strlen(ubicacion);
    char buf[size];
    buf[0] = '\0';
    strcat(buf,nombre);  
    strcat(buf,",");
    strcat(buf,ubicacion);
    Mensaje to_send =
    {
        1 + strlen(buf),
        buf,
    };
    int to_return = *is_file_1(&to_send,clnt);
    return to_return;
}

char* get_my_documents(CLIENT* clnt, char* ip)
{
	int size=strlen(ip)+2;
	char buf[size];
	strcpy(buf,"");
	memset(buf,'\0',1);
    strcat(buf,ip);
	Mensaje to_send =
    {
        1+strlen(buf),
        buf,
    };
    Mensaje mensaje = *get_files_ip_1(&to_send,clnt);
	return mensaje.Mensaje_val;
}
