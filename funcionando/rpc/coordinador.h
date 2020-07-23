#ifndef COORDINADOR_H
#define COORDINADOR_H
// Funcion para verificar si un archivo/carpeta es valido
// Recibe un tipo (0=carpeta,1=archivo), un nombre y la ubicacion del nodo actual
// Retorna 0=falso, 1=verdadero y 2=tipoInvalido
int esValido(char tipo, char* Nombre, char* Ubicacion);

// Funcion encargada del LS
// Recibe una ubicacion (carpeta actual)
// Retorna un string con todos los elementos en la carpeta
char* funcionListar(char* Direccion);

// Funcion para retornar la IP en la que se encuentra un archivo
// Recibe el nombre de un archivo y la ubicacion actual del nodo
// Retorna un string con el IP del nodo que tiene el archivo
char* obtenerIP(char* Archivo, char* Ubicacion);

// Inserta un archivo o carpeta en el sistema
// Recibe el nombre del elemento a insertar, su IP y ubicacion (en caso de ser archivo) y el tipo
int insert(char tipo, char* Nombre,char* IP, char* Ubicacion);

// Elimina un archivo o carpeta del sistema
// Recibe el tipo (1=archivo,0=carpeta) el nombre y, en caso de ser archivo, la IP y carpeta en la que se encuentra
int delete(char tipo, char* Nombre,char* IP, char* Ubicacion);
int carpetaVacia(char* Nombre);

// Modifica la IP de un archivo
// Recibe el nombre del archivo, la nueva ip y la carpeta donde se encuentra
int modificarIP(char* NuevaIP, char* Archivo, char* Carpeta);

// Verifica si un determinado nombre es un archivo o no
int isFile(char* Nombre, char* Ubicacion);

// Muevo un archivo de un directorio a otro
// Recibo el nombre del archivo, el directorio actual del archivo y el directorio destino
int modificarDirectorio(char* Nombre, char* CarpetaVieja, char* CarpetaNueva);

char* obtenerArchivoConIp(char* ip);
#endif /* COORDINADOR_H */
