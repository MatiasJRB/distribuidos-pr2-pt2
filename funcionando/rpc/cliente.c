#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include "protocolo.h"
#include "socketNodos.h"

/*
 typedef struct {
    u_int Mensaje_len;
    char *Mensaje_val;
 }Mensaje; 
*/

#define max_args 2  //Numero maximo de argumentos (-1) cuando se trate de un comando externo
#define max 100  //Numero de caracteres maximo para comando las variables de ambiente*/

/*Declara variables*/
char comando[max]; //comando va a leer el comando que ingrese el usuario
char* args[max_args];
char *path[max];
CLIENT *clnt;

/*Declara funciones*/
void separarArgumentos();
void listarDirectorio();
void ejecutarCD();
void editor();


/* Structs para el manejo del current working directory */

typedef struct{
    char* name;
    uint size;
}SubDirectorio;
SubDirectorio sd_actual;
SubDirectorio raiz;    	


void* setPath(){

	
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
    
    // iniciar la escucha de pedidos de otros nodos
    startListening();
    
    // downloadFile("192.168.0.186", "Makefile", "puto");
    
    int seguir=1;
 

    raiz.name = (char*) malloc(sizeof(char)*5); /* Reservo lugar para '/' y '\0' */
    strcpy(raiz.name,"raiz");
    raiz.size = strlen(raiz.name);
    
 
    sd_actual = raiz;    
    
    strcpy((char*)path,"/");

    while(seguir){
        printf("%s $",path);
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
                ejecutarCD();
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

/*1 el cliente solicita a la shell ejecutar cd con el nombre de la carpeta N que desea acceder [COM5]
2 el nodo le pregunta al coordinador si la direccion N es valido [COM3Y6]
3 el coordinador utiliza su tabla para saber si N es una direccion valida para ser accedida [COM8][COM4]
4 si la ruta es valida el coordinador retorna true, false en caso contrario [COM8][COM4]
5 en caso de recibir false muestra por pantalla mensaje de error [COM5]
6 en caso de recibir true, el nodo accede al directorio NR y actualiza su dirActual[COM5]
*/


void ejecutarCD(){
    if(args[1]==NULL){
	printf("uso: cd directorio \n");
    }else //chequear ..
    {
	if(strcmp(args[1], "..")==0){
	    if(strcmp(sd_actual.name,"raiz")){
		memset(path,'\0',max);
		strcpy((char*)path,"/");
		strcpy(sd_actual.name,"raiz");
		sd_actual.size = strlen("raiz");

		//ruta.sub_directorios[1] = NULL;
	    }else{
		printf("No se pueda ir al padre de la raiz \n");
	    }
	}else{
	    //Pregunta al coordinador si es valido un directorio con 0.
	    
	    char contenido_mensaje[1+sd_actual.size];
	    strcpy(contenido_mensaje,"0");
	    strcat(contenido_mensaje,sd_actual.name);


	     Mensaje cd =
	    {
		    sd_actual.size,
		    contenido_mensaje,
	    };
	    //int valid = is_valid_1(&cd,clnt);
	    int valid = 1;
	    if(valid){
		memset(path,'\0',max);
		strcpy((char*)path,"/");
		sd_actual.size = strlen(args[1]);
		strcpy(sd_actual.name,args[1]);
		strcat((char*)path,sd_actual.name);
		
	    }else{
		printf("El directorio ingresado no existe \n");
	    }
	}
    }
}


void editor(){
}

void listarDirectorio(){

    Mensaje msg_test =
    {
	   sd_actual.size,
	   sd_actual.name,
    };
    
    
    Mensaje* msg_to_rec = ls_1(&msg_test,clnt);
    
    u_int i=0;  
    char mensaje[(*msg_to_rec).Mensaje_len];
    strcpy(mensaje,(*msg_to_rec).Mensaje_val);
    int j=0;

    for(i=0; i<(*msg_to_rec).Mensaje_len; i++){
	while((mensaje[i]!=',') && i<(*msg_to_rec).Mensaje_len){
	    printf("%c",mensaje[j]);
	    i++;
	    j++;
	}
	j++;//saltea la ,
	printf(" ");
    }
    printf("\n");    
}


