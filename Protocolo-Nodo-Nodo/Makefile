all: enviarRecibir.o receptorPedidos.o emisorPedidos.o socketNodos.o bin

bin: nodo.c receptorPedidos.o emisorPedidos.o enviarRecibir.o socketNodos.o
	gcc nodo.c -Wall -Wpedantic -Wextra -o bin receptorPedidos.o emisorPedidos.o enviarRecibir.o socketNodos.o -lpthread

socketNodos.o: socketNodos.c
	gcc socketNodos.c -c -Wall -Wpedantic -Wextra

receptorPedidos.o: receptorPedidos.c
	gcc receptorPedidos.c -c -Wall -Wpedantic -Wextra

emisorPedidos.o: emisorPedidos.c
	gcc emisorPedidos.c -c -Wall -Wpedantic -Wextra

enviarRecibir.o: enviarRecibir.c
	gcc enviarRecibir.c -c -Wall -Wpedantic -Wextra

clean:
	$(RM) bin *.o
