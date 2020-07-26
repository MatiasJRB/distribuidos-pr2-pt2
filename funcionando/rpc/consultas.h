struct archivo
{
    char *nombre;            // Nombre del archivo
    char *ip;                // ip del archivo
    char *direccion;         // direccion del padre del archivo
    char *permiso;           // w o r, segun escritura o lectura
    int  version;            // numero de version con numero 0, es la mas actual
    int  tipo;               // 0 si es archivo, 1 si es carpeta
    char *ruta;              // ruta completa del archivo
};

struct listado
{
    struct archivo elementos[100];
    int cantidad;    
};

extern void *insertar(char *nombre, char *ip, char *direccion, char *permiso, char  *version, char  *tipo, char *ruta);

extern struct listado *funcionLS(char *direccion);

extern struct archivo *buscarArchivo(char *nombre, char *direccion);

void * eliminar(char *nombre, char *ip, char *direccion, char *permiso);

struct archivo *buscarCarpeta(char *nombre);

void *updateip(char *nombre, char *ipnueva, char *direccion, char *permiso, char *version, char  *tipo, char *ruta);

void * updatePosicion(char *nombre, char *ip, char *direccionNueva, char *permiso, char *version, char  *tipo, char *rutaNueva);

char* getFilesByIp(char* ip);
