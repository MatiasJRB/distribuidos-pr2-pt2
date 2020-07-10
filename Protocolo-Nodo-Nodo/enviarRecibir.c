#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <string.h> 

#include "enviarRecibir.h"

int recibir(int fd, void * mensaje, int size) {
    int recibidos;
    if ((recibidos = recv(fd, mensaje, size, 0)) == -1) {
        perror("Receive\n");
        exit(errno);
    }
    return recibidos;
}

int enviar(int fd, void * mensaje, int size) {
    int enviados;
    if ((enviados = send(fd, mensaje, size, MSG_NOSIGNAL)) == -1) {
        perror("Send\n");
        exit(errno);
    }
    return enviados;
}
