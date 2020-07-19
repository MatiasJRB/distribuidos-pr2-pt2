#include <stdio.h>
#include "protocolo.h"
#include "coordinador.h"

Mensaje *ls_1_svc(Mensaje *msg, struct svc_req *req)
{	
	static Mensaje to_return;
	to_return.Mensaje_val = funcionListar(msg->Mensaje_val);
	to_return.Mensaje_len = 1 + strlen(to_return.Mensaje_val);
	return (&to_return);	
}

Mensaje *getaddress_1_svc(Mensaje *msg, struct svc_req *req)
{
	static Mensaje to_return;
	to_return.Mensaje_val = obtenerIP(msg->Mensaje_val);
	to_return.Mensaje_len = 1 + strlen(to_return.Mensaje_val);
	return (&to_return);
}

int *is_valid_1_svc(Mensaje *msg, struct svc_req *req)
{
	static int to_return;
	to_return=1;//esValido(msg->Mensaje_val);
	return &to_return;
}

Mensaje *update_address_1_svc(Mensaje *msg, struct svc_req *req)
{
	static Mensaje to_return;
	return(&to_return);
}

Mensaje *report_create_1_svc(Mensaje *msg, struct svc_req *req)
{
	static Mensaje to_return;
	return(&to_return);
}

Mensaje *report_delete_1_svc(Mensaje *msg, struct svc_req *req)
{
	printf("Reporto el delete de %s.\n",msg->Mensaje_val);
	char str[64];
	strcpy(str,"");
	char* aux = "Terminado prro";
	strcat(str, aux);
	static Mensaje to_return;
	
	to_return.Mensaje_len =	64;
	to_return.Mensaje_val =	str;
	
	return (&to_return);
}

int *is_empty_1_svc(Mensaje *msg, struct svc_req *req)
{
	static int to_return;
	to_return = carpetaVacia(msg->Mensaje_val);
	return &to_return;
}

