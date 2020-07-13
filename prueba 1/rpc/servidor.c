#include <stdio.h>
#include "protocolo.h"
#include "consultas.h"

//el cuerpo de la funcion solo es para testear
//en el futuro tendra una llamada a una funcion del coordinador
Mensaje *ls_1_svc(Mensaje *msg, struct svc_req *req)
{	

	struct listado list = *funcionLS("Carpeta1");
	
	int msj_size=list.cantidad*20;
	char toRet[msj_size];
	static Mensaje to_return;
	int i;
	// En caso de implementar funcionalidades extras, aca vendria algun case con la seleccion a parametros a retornar
	
	for (i=0;i<list.cantidad;i++)
	
	{	
		struct archivo archivoaux= list.elementos[i];
		char * nombreaux = archivoaux.nombre;
		strcat(toRet, nombreaux);
		strcat(toRet, ",");
	}
	Mensaje msg_test;
	msg_test.Mensaje_len = strlen(toRet);
	msg_test.Mensaje_val = malloc(sizeof(msg_test.Mensaje_len));
	memcpy(msg_test.Mensaje_val, toRet, msg_test.Mensaje_len);
	
	to_return = msg_test;
	printf("%s\n",to_return.Mensaje_val);
	return (&to_return);	
}

//el cuerpo de la funcion solo es para testear
//en el futuro tendra una llamada a una funcion del coordinador
Mensaje *getaddress_1_svc(Mensaje *msg, struct svc_req *req)
{
	static Mensaje to_return;
	
	int cant_received = msg->Mensaje_len;
	char* payload_received = msg->Mensaje_val;
	
	printf("Server: Mensaje recibido\n");
	printf("Mensaje_len = %d\n",cant_received);
	printf("Mensaje_val = %s\n",payload_received);
	
	char send_buffer[6] = "chau.";
	Mensaje msg_test;
	msg_test.Mensaje_len = 6;
	msg_test.Mensaje_val = malloc(sizeof(6));
	memcpy(msg_test.Mensaje_val, send_buffer, 6);

	printf("Server: Mensaje enviado\n");
	printf("Mensaje_len = %d\n",6);
	printf("Mensaje_val = %s\n",send_buffer);
	to_return = msg_test;
	return (&to_return);
}

