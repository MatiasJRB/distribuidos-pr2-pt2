#include <stdio.h>
#include "protocolo.h"
#include "coordinador.h"

//el cuerpo de la funcion solo es para testear
//en el futuro tendra una llamada a una funcion del coordinador
Mensaje *ls_1_svc(Mensaje *msg, struct svc_req *req)
{	
	static Mensaje to_return;
	Mensaje msg_test;
	
	char* buf = funcionListar(msg->Mensaje_val);
	msg_test.Mensaje_len = 1+strlen(buf);
	
	msg_test.Mensaje_val = malloc(msg_test.Mensaje_len);
	memcpy(msg_test.Mensaje_val, buf, msg_test.Mensaje_len);
	
	free(buf);
	
	to_return = msg_test;
	return (&to_return);	
}

//el cuerpo de la funcion solo es para testear
//en el futuro tendra una llamada a una funcion del coordinador
Mensaje *getaddress_1_svc(Mensaje *msg, struct svc_req *req)
{
	static Mensaje to_return;
	Mensaje msg_test;
	
	char* buf = obtenerIP(msg->Mensaje_val);
	msg_test.Mensaje_len = 1+strlen(buf);
	
	msg_test.Mensaje_val = malloc(msg_test.Mensaje_len);
	memcpy(msg_test.Mensaje_val, buf, msg_test.Mensaje_len);
	
	to_return = msg_test;
	return (&to_return);
}

