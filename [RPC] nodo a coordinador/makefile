all: cliente servidor
.PHONY: all

protocolo.h protocolo_clnt.c protocolo_svc.c protocolo_xdr.c: protocolo.x
	rpcgen protocolo.x


cliente : cliente.c protocolo_clnt.c protocolo_xdr.c
	gcc -lnsl -o cliente cliente.c protocolo_clnt.c protocolo_xdr.c


servidor : servidor.c protocolo_svc.c protocolo_xdr.c
	gcc -lnsl -o servidor servidor.c protocolo_svc.c protocolo_xdr.c

