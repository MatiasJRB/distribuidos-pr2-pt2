#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include "protocolo.h"

#define max_args 2  //Numero maximo de argumentos (-1) cuando se trate de un comando externo
#define max 100  //Numero de caracteres maximo para comando las variables de ambiente*/

/*Declara variables*/
char comando[max]; //comando va a leer el comando que ingrese el usuario
char* args[max_args];
char* pwd;
CLIENT *clnt;

/*Declara funciones*/
void separarArgumentos();
void listarDirectorio();
void cambiarDirectorio();
void editor();


/* Structs para el manejo del current working directory */

typedef struct{
    char* nombre;
}SubNombre;

/*
*	Struct que contiene cada parte del directorio
* 	/carpeta -> ["/", "carpeta"]
*/
typedef struct{
    SubNombre sub_rutas[2];
    int cont;
}PWD;


/* A partir de un PWD obtengo en string la ruta completa */
char* getPath(PWD* ruta){

	int i;
	int cantMemory = 1;
	char* pathStr = (char*) malloc(sizeof(char));

	for(i=0; i<ruta->cont;i++){
		char* name = ruta->sub_rutas[i].nombre;
		cantMemory += strlen(name);		
		pathStr = realloc(pathStr,cantMemory);
		strcat(pathStr,name);		
	}
	
	return pathStr;
}

int main(int argc, char *argv[]){
    
    char *srv;

    if(argc != 2)
    {
	    printf("El argumento deben ser <ip>\n");
	    exit(1);
    }

    srv = argv[1];

    clnt = clnt_create(srv, PROY2DFS, PROY2DFSVERS,"tcp");

    if(clnt == (CLIENT*)NULL)
    {
	clnt_pcreateerror(srv);
	exit(2);
    }
    
    
    int seguir=1;
    PWD ruta;

    SubNombre raiz;    	
    raiz.nombre = (char*) malloc(sizeof(char)*2); /* Reservo lugar para '/' y '\0' */
    strcpy(raiz.nombre,"/");
    
    ruta.cont = 1;
    ruta.sub_rutas[0] = raiz;
    char* path = getPath(&ruta);
    pwd = (char*) malloc(sizeof(char)*strlen(path)+1);
    strcpy(pwd,path);

    while(seguir){
        printf("%s>",pwd);
        __fpurge(stdin); //Limpia el buffer de entrada del teclado.
        memset(comando,'\0',max);  //Borra cualquier contenido previo del comando.
        scanf("%[^\n]s",comando);   //Espera hasta que el usuario ingrese algun comando.
        if(strlen(comando)>0){   //verifica que haya ingresado algo seguido de un enter
            separarArgumentos();    //Separar comando de sus argumentos//
            if (strcmp(args[0],"ls")==0){
                listarDirectorio();

            }else if(strcmp(args[0],"exit")==0){
                seguir=0;
                exit(0);
            }else if(strcmp(args[0],"cd")==0){
                cambiarDirectorio();
            }else if(strcmp(args[0],"editor")==0){
                editor();
            }else{
                printf("No se reconoce el comando ingresado\n");
            }
        }
    }
    clnt_destroy(clnt);
}

void separarArgumentos(){
    int i;
    for(i=0; i<max_args; i++)   //borra argunmentos antiguos.
        args[i]=NULL;
    const char s[2]=" ";
    char* token=strtok(comando,s);    //separa "comando" en multiples strings segun la aparicion de s.
    for(i=0; i<max_args && token!=NULL; i++){
        args[i]=token;
        token=strtok(NULL,s);   //Avanza hacia el siguiente string.
    }
}

void cambiarDirectorio(){
}

void editor(){
}

void listarDirectorio(){

  //CARGA DEL STRUCT ES LO QUE DEBERIAMOS LLAMAR A RPC function_ls(pwd)
  //Mensaje *m1;
  //m1 =(Mensaje*) malloc(sizeof(Mensaje));
  //(*m1).Mensaje_val = "Carpeta1";
  //(*m1).Mensaje_len = 8;
    
    //Mensaje *msg_to_rec;
    char buff[8] = "Carpeta1";
    Mensaje msg_test =
    {
	    8,
	    buff,
    };
    printf("Cliente: Mensaje enviado\n");
    printf("Mensaje_len = %d\n",6);
    printf("Mensaje_val = %s\n",buff);

    Mensaje* msg_to_rec = ls_1(&msg_test,clnt);
    
    int cant_received = msg_to_rec->Mensaje_len;
    char* payload_received = msg_to_rec->Mensaje_val;

    printf("Cliente: Mensaje recibido\n");
    printf("%d\n",cant_received);
    printf("%s\n",payload_received);
    
  /*
  //__________________________________________________
    Mensaje* msg_to_rec = ls_1(&msj,clnt);
    
    
		
 printf("aca2\n");
  
  u_int i=0;
  
  char mensaje[(*msg_to_rec).Mensaje_len];
  strcpy(mensaje,(*msg_to_rec).Mensaje_val);
  char *c = malloc(sizeof(char));
  char *nombres = malloc(sizeof(char));
    for(i=0; i<(*msg_to_rec).Mensaje_len; i++){
      while(mensaje[i]!=',' && i<(*msg_to_rec).Mensaje_len){

	*c =(char) mensaje[i];
	strcat(nombres,c);
        i++;
      }
      strcat(nombres," ");
    }
  printf("significativo %s\n",nombres);
  printf("\n");
  free(msg_to_rec);
  free(c);
  free(nombres);
  * */
  
}


