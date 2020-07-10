#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "protocolo.h"

int main(int argc, char *argv[])
{
	char *srv;
	CLIENT *clnt;

	if(argc != 2)
	{
		printf("El argumento deben ser <ip>\n");
		exit(1);
	}

	srv = argv[1];

	clnt = clnt_create(srv, PROY2DFS, PROY2DFSVERS,"tcp");

	if(clnt == (CLIENT*)NULL)
	{
		clnt_pcreateerror(srv);
		exit(2);
	}
	
		//Mensaje *msg_to_rec;
		char buff[6] = "hola.";
		Mensaje msg_test =
		{
			6,
			buff,
		};
		printf("Cliente: Mensaje enviado\n");
		printf("Mensaje_len = %d\n",6);
		printf("Mensaje_val = %s\n",buff);

		Mensaje* msg_to_rec = ls_1(&msg_test,clnt);
		
		int cant_received = msg_to_rec->Mensaje_len;
		char* payload_received = msg_to_rec->Mensaje_val;
	
		printf("Cliente: Mensaje recibido\n");
		printf("%d\n",cant_received);
		printf("%s\n",payload_received);

	clnt_destroy(clnt);
	return 0;
}
