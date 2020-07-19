#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include "protocolo.h"
#include "socketNodos.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <netdb.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 

/*
 typedef struct {
    u_int Mensaje_len;
    char *Mensaje_val;
 }Mensaje; 
*/

#define max_args 3  //Numero maximo de argumentos (-1) cuando se trate de un comando externo
#define max 100  //Numero de caracteres maximo para comando las variables de ambiente*/


/*Declara variables*/
char comando[max]; //comando va a leer el comando que ingrese el usuario
char* args[max_args];
char *path[max];
CLIENT *clnt;
char ip[15];

/*Declara funciones*/
void separarArgumentos();
void listarDirectorio();
void ejecutarCD();
void editor();
void ejecutarMKDIR();
void rm();

/* Structs para el manejo del current working directory */

typedef struct{
    char* name;
    uint size;
}SubDirectorio;
SubDirectorio sd_actual;
SubDirectorio raiz;    	


void* setPath(){

	
}

void ejecutarMKDIR()
{
    if(args[1]==NULL){
		printf("uso: mkdir directorio \n");
    }
	else 
    {
		//Pregunta al coordinador si es valido un directorio con 0.	    


		int maxChar = 512;
		char* cadena = malloc(maxChar * sizeof(char));
		memset(cadena,'\0',1);
		char tipoChar[2];
		sprintf(tipoChar,"%d",0);
		strcat(cadena,tipoChar);
		strcat(cadena,",");
		strcat(cadena,args[1]);
		Mensaje mkdir =
		{
			strlen(cadena),
			cadena
		};
		if(strcmp((char*)path,"/")){ //caso en el que no estoy en root
		    printf("No puedes crear mas niveles de carpetas \n");
		}else{
		    int valid = *exists_1(&mkdir,clnt);
		    //int valid = 0;
		    if(valid)
		    {
			if(valid==1){
			    printf("El directorio ingresado ya existe \n");
			}else{
			    printf("Error DFS: No se pudo crear \n");
			}
		    }
		    else
		    {
			    char size_ip[2];
			    int size = strlen(ip);
			    sprintf(size_ip,"%d",size);
			    char contenido_mensaje[1+sd_actual.size+strlen(ip)+2];
			    strcpy(contenido_mensaje,"0");
			    strcat(contenido_mensaje,args[1]);
			    strcat(contenido_mensaje,size_ip);
			    strcat(contenido_mensaje,(char*)ip);
			    int maxChar = 512;
			    char* cadena = malloc(maxChar * sizeof(char));
			    memset(cadena,'\0',1);
			    char tipoChar[2];
			    sprintf(tipoChar,"%d",0);
			    strcat(cadena,tipoChar);
			    strcat(cadena,",");
			    strcat(cadena,args[1]);
			    strcat(cadena,",");
			    strcat(cadena,size);
			    strcat(cadena,",");
			    strcat(cadena,size_ip);
			    Mensaje mkdir_report =
			    {
				    strlen(cadena),
				    cadena
			    };
			    printf("EL contenido de mensaje es %s \n",cadena);
			    int size_dir= strlen(args[1]);
			    char buffer[size_dir+6];
			    strcpy((char*)buffer,"mkdir ");
			    strcat((char*)buffer,(char*)args[1]);
			    system(buffer);
			    report_create_1(&mkdir_report,clnt);
			    
		    }
		}
		
    }
}

void obtenerIP(){
    system("hostname -I > nombre");
    FILE* arch = fopen("nombre","r");
    fscanf(arch,"%s",ip);	
    fclose(arch);
    remove("nombre");
}

int main(int argc, char *argv[]){
    memset(ip,'\0',15);
    obtenerIP();
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
	    }else if(strcmp(args[0],"mkdir")==0){
		ejecutarMKDIR();
	    }else if(strcmp(args[0],"rm")==0){
		rm();
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
	
	    int maxChar = 512;
	    char* cadena = malloc(maxChar * sizeof(char));
	    memset(cadena,'\0',1);
	    char tipoChar[2];
	    sprintf(tipoChar,"%d",0);
	    strcat(cadena,tipoChar);
	    strcat(cadena,",");
	    strcat(cadena,args[1]);
	    Mensaje cd =
	    {
		    strlen(cadena),
		    cadena
	    };
	    int valid = *exists_1(&cd,clnt);
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



char* getMyIp()
{
    char myIp[256];
    struct hostent *host_entry;
    char *IPbuffer; 
    gethostname(myIp,256);
    host_entry = gethostbyname(myIp); 
    IPbuffer = inet_ntoa(*((struct in_addr*) 
                   host_entry->h_addr_list[0])); 
    return IPbuffer;
}

int isValidIpAddress(char *ipAddress)
{
    struct sockaddr_in sa;
    int result = inet_pton(AF_INET, ipAddress, &(sa.sin_addr));
    return result;
}

int removeLocal(char* direccion)
{
    int result = remove(direccion);
    return result;
}

int rmAux(char* type,char* file)
{
    if(file == NULL || type == NULL)
	return -1;
    
    int tipoOperacion = 0;
    if(!strcmp(type,"-d"))
    {
	//Voy a borrar un directorio
	tipoOperacion = 0;
    }
    else
    if(!strcmp(type,"-f"))
    {
	//Voy a borrar un archivo
	tipoOperacion = 1;
    }
    else
	return -1;
    char toSend[256];
    strcpy(toSend,"");
    strcat(toSend,file);
    /*
    Mensaje msg_to_send = {
	256,
	toSend
    };
    */
    
    int maxChar = 512;
    char* cadena = malloc(maxChar * sizeof(char));
    memset(cadena,'\0',1);
    char tipoChar[2];
    sprintf(tipoChar,"%d",tipoOperacion);
    strcat(cadena,tipoChar);
    strcat(cadena,",");
    strcat(cadena,toSend);
    strcat(cadena,",");
    strcat(cadena, sd_actual.name);
    //printf("La cadena a enviar es: %s.\n Su longitud es: %d.\n",cadena,strlen(cadena));
    Mensaje msg_to_send = {
	strlen(cadena),
	cadena
    };
    
    char* cadena2 = malloc(maxChar * sizeof(char));
    memset(cadena2,'\0',1);
    strcat(cadena2,toSend);
    strcat(cadena2,",");
    strcat(cadena2, sd_actual.name);
    //printf("La cadena2 a enviar es: %s.\n Su longitud es: %d.\n",cadena2,strlen(cadena2));
    Mensaje msg_to_send2 = {
	strlen(cadena2),
	cadena2
    };
    
    int valid = *exists_1(&msg_to_send, clnt);
    if(valid)
    {
	if(!tipoOperacion) //Si es una carpeta debo ver que este vacia
	{
	    Mensaje msg_to_send3 = {
		strlen(toSend),
		toSend
		};
	    int isEmpty = *is_empty_1(&msg_to_send3, clnt);
	    if(!isEmpty){
		printf("No esta vacio.\n");
		return -3;
	    }
	}
	Mensaje* msg_to_rec = getaddress_1(&msg_to_send2, clnt);
	char* ip = msg_to_rec->Mensaje_val;
	//printf("Recibi una ip %s.\n",ip);
	if(isValidIpAddress(ip))
	{
	    if(strcmp(ip,getMyIp()))
	    {
		//No lo tengo yo
		if(removeFile(ip,toSend))
		{
		    report_delete_1(&msg_to_send, clnt);
		    return 0;
		}
		return -4;
	    }
	    else
	    {
		//Lo tengo yo y debo hacer un remove local
		if(!removeLocal(toSend))
		{
		    report_delete_1(&msg_to_send, clnt);
		    return 0;
		}
		return -4;
	    }
	}
	return -4;
    }
    return -2;
}


void rm()
{
    int result = rmAux(args[1],args[2]);
    switch (result)
    {
	case -1: 
	    printf("Uso: rm <-d/-f> <ruta archivo/directorio>\n");
	    break;
	case -2:
	    printf("El nombre de archivo/carpeta no es valido.\n");
	    break;
	case -3:
	    printf("Fallo al borrar '%s': El directorio no esta vacio.\n");
	    break;
	case -4:
	    printf("Ocurrio un error y no se puede eliminar.\n");
	    break;
    }
}
