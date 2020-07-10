#include <mariadb/my_global.h>
#include <mariadb/mysql.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
    struct archivo elementos[40];
    int cantidad;    
};


void * insertar(char *nombre, char *ip, char *direccion, char *permiso, int  version, int  tipo, char *ruta){
    //Si lo que tengo que agregar es un archivo tengo que hacer un cambio de version en los demas.
    char query[200];
    strcpy(query, "INSERT INTO indexado (nombre, ip, direccion, permiso, version, tipo, ruta)VALUES ('");
    strcat(query, nombre);
    strcat(query, "','");
    strcat(query, ip);
    strcat(query, "','");
    strcat(query, direccion);
    strcat(query, "','");
    strcat(query, permiso); 
    strcat(query, "',");
    strcat(query, version); 
    strcat(query, ",");
    strcat(query, tipo); 
    strcat(query, ",'");
    strcat(query, ruta);
    strcat(query, "');");

    if (tipo = 0){ //Agrego un archivo , por lo tanto tengo que cambiar las versiones de los demas y pasarlos a lectura y luego recien ejecuto la query.
        
    } else { // Agrego una carpeta, ejecuto la query directamente

    }

}

struct listado *funcionLS(char *direccion){
    // Inicializo el motor de mysql  
    MYSQL *con = mysql_init(NULL);
    mysql_real_connect(con, "localhost", "ruso", "rusopass", "proyecto", 0, NULL, 0);
    MYSQL_RES *res;
    MYSQL_ROW row;

    struct listado *resultado = (struct listado *)malloc(sizeof (struct archivo)*40);
    resultado->cantidad = 0;

    char query[100];
    strcpy(query, "SELECT * ");
    strcat(query, "FROM indexado ");
    strcat(query, "WHERE ");
    strcat(query, "direccion = '");
    strcat(query, direccion);
    strcat(query, "';");
    
	mysql_query(con, query);
    // Obtengo el resultado de esa consulta
    res = mysql_use_result(con);
    int i=0;
    printf("________________________________________________\n");
    while ((row = mysql_fetch_row(res)) != NULL) /* recorrer la variable res con todos los registros obtenidos para su uso */
    {   
        resultado->cantidad = resultado->cantidad +1 ; 
        printf("%s \t %s \t %s \t %s \t %s \t %s \t %s \n", row[0],row[1],row[2],row[3],row[4],row[5],row[6]);

        struct archivo *actual = (struct archivo *)malloc(sizeof (struct archivo));
        actual->nombre = (char*)malloc (50*sizeof(char));
        actual->ip    = (char*)malloc (50*sizeof(char));
        actual->direccion = (char*)malloc (50*sizeof(char));
        actual->permiso = (char*)malloc (1*sizeof(char));
        actual->ruta = (char*)malloc (100*sizeof(char));

        strcpy(actual->nombre, row[0]);
        strcpy(actual->ip, row[1]);
        strcpy(actual->direccion, row[2]);
        strcpy(actual->permiso, row[3]);
        actual->version = atoi(row[4]);
        actual->tipo = atoi(row[5]);
        strcpy(actual->ruta, row[6]);

        resultado->elementos[i] = *actual;
        i=i+1;
    } 
    printf("________________________________________________\n" );
    return resultado;

}

struct archivo *buscarArchivo(char *nombre){
    // Inicializo el motor de mysql  
    MYSQL *con = mysql_init(NULL);
    mysql_real_connect(con, "localhost", "ruso", "rusopass", "proyecto", 0, NULL, 0);
    MYSQL_RES *res;
    MYSQL_ROW row;
    
    struct archivo *resultado = (struct archivo *)malloc(sizeof (struct archivo));
    resultado->nombre = (char*)malloc (50*sizeof(char));
    resultado->ip    = (char*)malloc (50*sizeof(char));
    resultado->direccion = (char*)malloc (50*sizeof(char));
    resultado->permiso = (char*)malloc (1*sizeof(char));
    resultado->ruta = (char*)malloc (100*sizeof(char));
       
    char query[100];
    strcpy(query, "SELECT * ");
    strcat(query, "FROM indexado ");
    strcat(query, "WHERE ");
    strcat(query, "nombre = '");
    strcat(query, nombre);
    strcat(query, "' AND version = 0;");

	mysql_query(con, query);
    // Obtengo el resultado de esa consulta
    res = mysql_use_result(con);
   

    printf("________________________________________________\n");
    while ((row = mysql_fetch_row(res)) != NULL) /* recorrer la variable res con todos los registros obtenidos para su uso */
    {   
        printf("%s \t %s \t %s \t %s \t %s \t %s \t %s \n", row[0],row[1],row[2],row[3],row[4],row[5],row[6]);
        strcpy(resultado->nombre, row[0]);
        strcpy(resultado->ip, row[1]);
        strcpy(resultado->direccion, row[2]);
        strcpy(resultado->permiso, row[3]);
        resultado->version = atoi(row[4]);
        resultado->tipo = atoi(row[5]);
        strcpy(resultado->ruta, row[6]);
    } 
    printf("________________________________________________\n ");
    return resultado;
}



// Funciones las cuales utilizaran ustedes Max y Mauro
int main(int argc, char **argv)
{  
    printf("\n METODO DE BUSQUEDA \n");
    printf("Nombre \t Ip \t direccion \t permiso \t version \t tipo \t ruta \n");
    char * nombreArchivo = "ArchivoA.txt";
    struct archivo mostrar;
    mostrar = *buscarArchivo(nombreArchivo);    
    printf("%s \t %s \t %s \t %s \t %d \t %d \t %s \n", mostrar.nombre, mostrar.ip, mostrar.direccion, mostrar.permiso, mostrar.version, mostrar.tipo, mostrar.ruta);
    printf("\n FIN DE METODO DE BUSQUEDA \n");




    printf("\n METODO DE LS \n");
    printf("Nombre \t Ip \t direccion \t permiso \t version \t tipo \t ruta \n");
    char * nombreDirectorio = "Carpeta1";
    struct listado mostrarlistado;
    mostrarlistado = *funcionLS(nombreDirectorio); 

    int limite = mostrarlistado.cantidad;
    int i=0;

    while (i<limite){
        printf("%s \t %s \t %s \t %s \t %d \t %d \t %s \n", mostrarlistado.elementos[i].nombre, mostrarlistado.elementos[i].ip, mostrarlistado.elementos[i].direccion, mostrarlistado.elementos[i].permiso,mostrarlistado.elementos[i].version, mostrarlistado.elementos[i].tipo, mostrarlistado.elementos[i].ruta);
        i = i+1; 
    }
    printf("\n FIN DE METODO DE LS \n");


}
