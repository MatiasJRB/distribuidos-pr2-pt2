
#define PUERTO 15000 // puerto en el que cada nodo va a escuchar

#define MAXDATASIZE 512
#define DOWNLOAD 1
#define COPY 2
#define MOVE 6
#define REMOVE 3
#define ERROR 4
#define ACK 5

typedef struct PAQUETE_SOCKET
{
	int identificador; //Identificador del tipo de operacion que se este realizando
	int tamanio_data; //Indicará el tamaño del archivo en bytes
    int ultimo_paquete;
    char data[MAXDATASIZE]; //Indicará la ruta del archivo (id=1) o el archivo en sì (si el id=2)
} PAQUETE_SOCKET;
