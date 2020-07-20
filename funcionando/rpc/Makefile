all: enviarRecibir.o receptorPedidos.o emisorPedidos.o socketNodos.o cliente servidor
.PHONY: all 

protocolo.h protocolo_clnt.c protocolo_svc.c protocolo_xdr.c: protocolo.x
	rpcgen protocolo.x


cliente : cliente.c protocolo_clnt.c protocolo_xdr.c socketNodos.o receptorPedidos.o emisorPedidos.o enviarRecibir.o
	gcc -lnsl -o cliente cliente.c comunicacion.c protocolo_clnt.c protocolo_xdr.c socketNodos.o receptorPedidos.o emisorPedidos.o enviarRecibir.o -lpthread

servidor : servidor.c protocolo_svc.c protocolo_xdr.c consultas.c coordinador.c
	gcc -lnsl -o servidor `mysql_config --cflags` servidor.c coordinador.c protocolo_svc.c protocolo_xdr.c consultas.c `mysql_config --libs`
	
socketNodos.o: socketNodos.c
	gcc socketNodos.c -c -Wall -Wpedantic -Wextra

receptorPedidos.o: receptorPedidos.c
	gcc receptorPedidos.c -c -Wall -Wpedantic -Wextra

emisorPedidos.o: emisorPedidos.c
	gcc emisorPedidos.c -c -Wall -Wpedantic -Wextra

enviarRecibir.o: enviarRecibir.c
	gcc enviarRecibir.c -c -Wall -Wpedantic -Wextra

clean:
	rm -f *.o cliente servidor protocolo_svc.c protocolo_xdr.c protocolo_clnt.c protocolo.h
