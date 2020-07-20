#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "protocolo.h"
#include "comunicacion.h"

char* ls(CLIENT* clnt, char* dir)
{
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
    int size = 2 + strlen(nombre) + strlen(ubicacion);
    char buf[size];
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
    int size = 3 + strlen(nombre);
    if(ubicacion != NULL)
    {
        size = size + 1 + strlen(ubicacion);
    }
    char buf[size];
    buf[0] = tipo;
    strcat(buf,",");
    strcat(buf,nombre);
    if(ubicacion != NULL)
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
    int size = 4 + strlen(nombre) + strlen(ip);
    if(ubicacion != NULL)
    {
        size = size + 1 + strlen(ubicacion);
    }
    char buf[size];
    buf[0] = tipo;
    strcat(buf,",");
    strcat(buf,nombre);
    strcat(buf,",");
    strcat(buf,ip);
    if(ubicacion != NULL)
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

int report_delete(CLIENT* clnt, char tipo, char* nombre, char* ip, char* ubicacion)
{
    return 0;
}


