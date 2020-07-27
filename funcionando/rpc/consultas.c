// #include <mariadb/my_global.h>
// #include <mariadb/mysql.h>
#include <mysql/mysql.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "consultas.h"

#define N 500

void * insertar(char *nombre, char *ip, char *direccion, char *permiso, char  *version, char  *tipo, char *ruta){
    // Inicializo el motor de mysql  
    MYSQL *con = mysql_init(NULL);
    mysql_real_connect(con, "localhost", "ruso", "rusopass", "proyecto", 0, NULL, 0);
    //Si lo que tengo que agregar es un archivo tengo que hacer un cambio de version en los demas.
    
    char query[N];
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
    if (tipo = "1"){ //Agrego un archivo , por lo tanto tengo que cambiar las versiones de los demas y pasarlos a lectura y luego recien ejecuto la query.
        char queryupdate[N];
        strcpy(queryupdate, "UPDATE indexado SET permiso = 'R',version = version + 1 WHERE permiso != 'X' and nombre = '");
        strcat(queryupdate, nombre);
        strcat(queryupdate, "AND ruta = '");
        strcat(queryupdate, ruta);
        strcat(queryupdate, "';");
        mysql_query(con, queryupdate); //Ejecuto la query para updatear los registros
    }
    mysql_query(con, query); //Ejecuto la query para agregar el registro
    mysql_close(con);
    //printf("\n Termine de ejecutar el metodo de insertar \n");
    
}

struct listado *funcionLS(char *direccion){
    // Inicializo el motor de mysql  
    MYSQL *con = mysql_init(NULL);
    mysql_real_connect(con, "localhost", "ruso", "rusopass", "proyecto", 0, NULL, 0);
    MYSQL_RES *res;
    MYSQL_ROW row;

    struct listado *resultado = (struct listado *)malloc(sizeof (struct archivo)*1024);
    resultado->cantidad = 0;

    char query[N];
    strcpy(query, "SELECT * ");
    strcat(query, "FROM indexado ");
    strcat(query, "WHERE ");
    strcat(query, "direccion = '");
    strcat(query, direccion);
    strcat(query, "' AND permiso!='X';"); //si permiso es X, es porque esta borrado
    //printf("query: %s\n",query);
	mysql_query(con, query);
    // Obtengo el resultado de esa consulta
    res = mysql_use_result(con);
    int i=0;
    int cant =0;
    while ((row = mysql_fetch_row(res)) != NULL) /* recorrer la variable res con todos los registros obtenidos para su uso */
    {   
        //resultado->cantidad = resultado->cantidad +1 ; 
        cant++;
        struct archivo *actual = (struct archivo *)malloc(sizeof (struct archivo));
        actual->nombre = (char*)malloc (100*sizeof(char));
        actual->ip    = (char*)malloc (100*sizeof(char));
        actual->direccion = (char*)malloc (100*sizeof(char));
        actual->permiso = (char*)malloc (1*sizeof(char));
        actual->ruta = (char*)malloc (200*sizeof(char));

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
    resultado->cantidad = cant;
    //printf("Cantidad: %d.\n",resultado->cantidad);
    //printf("Llegue aca sin problemas\n");
    mysql_close(con);
    return resultado;

}

// Modificacion para que busque un archivo o carpeta, con nombre y direccion porque puede tener el mismo nombre en diferente archivo.
// Recorda que si estoy buscando una carpeta la direccion es - (fijate lso ejemplo en la tabla)
// vos de ante mano sabes si tenes que buscar una carpeta o un archivo
struct archivo *buscarArchivo(char *nombre, char *direccion){
    // Inicializo el motor de mysql  
    MYSQL *con = mysql_init(NULL);
    mysql_real_connect(con, "localhost", "ruso", "rusopass", "proyecto", 0, NULL, 0);
    MYSQL_RES *res;
    MYSQL_ROW row;
    
    struct archivo *resultado = (struct archivo *)malloc(sizeof (struct archivo));
    resultado->nombre = (char*)malloc (100*sizeof(char));
    resultado->ip    = (char*)malloc (100*sizeof(char));
    resultado->direccion = (char*)malloc (100*sizeof(char));
    resultado->permiso = (char*)malloc (1*sizeof(char));
    resultado->ruta = (char*)malloc (200*sizeof(char));
       
    char query[N];
    strcpy(query, "SELECT * ");
    strcat(query, "FROM indexado ");
    strcat(query, "WHERE ");
    strcat(query, "nombre = '");
    strcat(query, nombre);
    strcat(query, "' AND direccion = '");
    strcat(query, direccion);
    strcat(query, "' AND permiso!='X';");
	mysql_query(con, query);
    // Obtengo el resultado de esa consulta
    res = mysql_use_result(con);
    int cantidad=0;
        while ((row = mysql_fetch_row(res)) != NULL) /* recorrer la variable res con todos los registros obtenidos para su uso */
        {   
            cantidad = cantidad + 1;
            strcpy(resultado->nombre, row[0]);
            strcpy(resultado->ip, row[1]);
            strcpy(resultado->direccion, row[2]);
            strcpy(resultado->permiso, row[3]);
            resultado->version = atoi(row[4]);
            resultado->tipo = atoi(row[5]);
            strcpy(resultado->ruta, row[6]);
        } 
    if (cantidad == 0){ // si es 0 significa que nunca encontro un archivo.
        strcpy(resultado->permiso, "N"); //Si el resultado es null, te pongo como nombre el NULL
    }
    mysql_close(con);
    return resultado;
}

// Para eliminar necesito los siguientes datos para asegurarnos que vamos a eliminar correctamente
void * eliminar(char *nombre, char *ip, char *direccion, char *permiso){
    // Inicializo el motor de mysql  
    MYSQL *con = mysql_init(NULL);
    mysql_real_connect(con, "localhost", "ruso", "rusopass", "proyecto", 0, NULL, 0);
    char query[N];
    strcpy(query, "UPDATE indexado SET permiso = 'X' WHERE nombre='");
    strcat(query, nombre);
    strcat(query, "' AND ip='");
    strcat(query, ip);
    strcat(query, "' AND direccion='");
    strcat(query, direccion);
    //strcat(query, "' AND permiso='");
    //strcat(query, permiso); 
    strcat(query, "';");
    //printf("Query delete: %s\n",query);
    mysql_query(con, query); //Ejecuto la query para modificar la entrada
    mysql_close(con);
 
    
}
struct archivo *buscarCarpeta(char *nombre){
    // Inicializo el motor de mysql  
    MYSQL *con = mysql_init(NULL);
    mysql_real_connect(con, "localhost", "ruso", "rusopass", "proyecto", 0, NULL, 0);
    MYSQL_RES *res;
    MYSQL_ROW row;
    
    struct archivo *resultado = (struct archivo *)malloc(sizeof (struct archivo));
    resultado->nombre = (char*)malloc (100*sizeof(char));
    resultado->ip    = (char*)malloc (100*sizeof(char));
    resultado->direccion = (char*)malloc (100*sizeof(char));
    resultado->permiso = (char*)malloc (1*sizeof(char));
    resultado->ruta = (char*)malloc (200*sizeof(char));
       
    char query[300];
    strcpy(query, "SELECT * ");
    strcat(query, "FROM indexado ");
    strcat(query, "WHERE ");
    strcat(query, "nombre = '");
    strcat(query, nombre);
    strcat(query, "' AND tipo = 0 AND permiso!='X';");

	mysql_query(con, query);
    // Obtengo el resultado de esa consulta
    res = mysql_use_result(con);
    int cantidad=0;

        while ((row = mysql_fetch_row(res)) != NULL) /* recorrer la variable res con todos los registros obtenidos para su uso */
        {   
            cantidad = cantidad + 1;
            strcpy(resultado->nombre, row[0]);
            strcpy(resultado->ip, row[1]);
            strcpy(resultado->direccion, row[2]);
            strcpy(resultado->permiso, row[3]);
            resultado->version = atoi(row[4]);
            resultado->tipo = atoi(row[5]);
            strcpy(resultado->ruta, row[6]);
        } 
    
    if (cantidad == 0){ // si es 0 significa que nunca encontro un archivo.
        strcpy(resultado->permiso, "N"); //Si el resultado es null, te pongo como nombre el NULL
    }

    mysql_close(con);
    return resultado;
}

void *updateip(char *nombre, char *ipnueva, char *direccion, char *permiso, char *version, char  *tipo, char *ruta)

{
    // Inicializo el motor de mysql  

    MYSQL *con = mysql_init(NULL);
    mysql_real_connect(con, "localhost", "ruso", "rusopass", "proyecto", 0, NULL, 0);

    char query[500];
    strcpy(query, "UPDATE indexado SET ip= '");
    strcat(query, ipnueva);
    strcat(query, "' WHERE ruta= '");
    strcat(query, ruta);
    strcat(query, "' AND nombre = '");
    strcat(query, nombre); 
    strcat(query, "' AND direccion = '");
    strcat(query, direccion); 
    strcat(query, "';");

    mysql_query(con, query); //Ejecuto la query para modificar la entrada
    mysql_close(con);

}

void * updatePosicion(char *nombre, char *ip, char *direccionNueva, char *permiso, char *version, char  *tipo, char *rutaNueva){
    
    // Inicializo el motor de mysql  
    MYSQL *con = mysql_init(NULL);
    mysql_real_connect(con, "localhost", "ruso", "rusopass", "proyecto", 0, NULL, 0);
    
    char query[500];
    strcpy(query, "UPDATE indexado SET direccion = '");
    strcat(query, direccionNueva);
    strcat(query, "', ruta = '");
    strcat(query, rutaNueva);
    strcat(query, "' WHERE ip = '");
    strcat(query, ip);
    strcat(query, "' AND nombre = '");
    strcat(query, nombre); 
    strcat(query, "';");
  
    mysql_query(con, query); //Ejecuto la query para modificar la entrada
    mysql_close(con);
  
}

char* getFilesByIp(char* ip)
{
    // Inicializo el motor de mysql  
    MYSQL *con = mysql_init(NULL);
    mysql_real_connect(con, "localhost", "ruso", "rusopass", "proyecto", 0, NULL, 0);
    MYSQL_RES *res;
    MYSQL_ROW row;
    char query[500];
    strcpy(query, "SELECT direccion, nombre from indexado where ip = '");
    strcat(query, ip);
    strcat(query, "' and permiso != 'X';");
    
    //printf("Query: %s.\n",query);
    
    mysql_query(con, query); //Ejecuto la query para modificar la entrada
    
    // Obtengo el resultado de esa consulta
    res = mysql_use_result(con);
    int cantidad=0;
    char* resultado = malloc(2048*sizeof(char));
    strcpy(resultado,"");
    while ((row = mysql_fetch_row(res)) != NULL) /* recorrer la variable res con todos los registros obtenidos para su uso */
    {   
        cantidad = cantidad + 1;
        strcat(resultado, row[0]);//Pongo la carpeta
        strcat(resultado,",");
        strcat(resultado, row[1]);//Pongo el nombre
        strcat(resultado,",");
    } 
    mysql_close(con);
    if(cantidad>0){
        //printf("El resultado es: %s\n",resultado);
        return resultado;
    }
    return "";
}
