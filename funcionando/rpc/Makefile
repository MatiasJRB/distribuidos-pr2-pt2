all: rpcgen enviarRecibir.o receptorPedidos.o emisorPedidos.o socketNodos.o editor cliente servidor
.PHONY: all 

rpcgen: protocolo.x
	rpcgen protocolo.x

cliente : cliente.c protocolo_clnt.c protocolo_xdr.c socketNodos.o receptorPedidos.o emisorPedidos.o enviarRecibir.o
	gcc -lnsl -o cliente cliente.c comunicacion.c protocolo_clnt.c protocolo_xdr.c socketNodos.o receptorPedidos.o emisorPedidos.o enviarRecibir.o -lpthread

servidor : servidor.c protocolo_svc.c protocolo_xdr.c consultas.c coordinador.c
	gcc -lnsl -o servidor `mysql_config --cflags` servidor.c coordinador.c protocolo_svc.c protocolo_xdr.c consultas.c `mysql_config --libs`
	
socketNodos.o: nodo-nodo/socketNodos.c
	gcc nodo-nodo/socketNodos.c -c -Wall -Wpedantic -Wextra

editor: editor.c protocolo_clnt.c protocolo_xdr.c socketNodos.o receptorPedidos.o emisorPedidos.o enviarRecibir.o
	gcc -lnsl -o editor editor.c comunicacion.c  protocolo_clnt.c protocolo_xdr.c socketNodos.o receptorPedidos.o emisorPedidos.o enviarRecibir.o -lpthread

receptorPedidos.o: nodo-nodo/receptorPedidos.c
	gcc nodo-nodo/receptorPedidos.c -c -Wall -Wpedantic -Wextra

emisorPedidos.o: nodo-nodo/emisorPedidos.c
	gcc nodo-nodo/emisorPedidos.c -c -Wall -Wpedantic -Wextra

enviarRecibir.o: nodo-nodo/enviarRecibir.c
	gcc nodo-nodo/enviarRecibir.c -c -Wall -Wpedantic -Wextra

clean:
	rm -f *.o cliente servidor editor protocolo_svc.c protocolo_xdr.c protocolo_clnt.c protocolo.h
