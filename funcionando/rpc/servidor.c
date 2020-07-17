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

