#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#define max_args 2  //Numero maximo de argumentos (-1) cuando se trate de un comando externo
#define max 100  //Numero de caracteres maximo para comando las variables de ambiente*/

/*Declara variables*/
char comando[max]; //comando va a leer el comando que ingrese el usuario
char* args[max_args];
char* pwd;

/*Declara funciones*/
void separarArgumentos();
void listarDirectorio();
void cambiarDirectorio();
void editor();

/* Structs para el manejo de los nombres de archivos */

typedef struct{
    u_int Mensaje_len;
    char* Mensaje_val;
}Mensaje;

typedef struct{
    char name[20];
}NombreArchivos;


/* Structs para el manejo del current working directory */

typedef struct{
    char* nombre;
}SubNombre;

/*
*	Struct que contiene cada parte del directorio
* 	/carpeta -> ["/", "carpeta"]
*/
typedef struct{
    SubNombre sub_rutas[20];
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

int main(){
    int seguir=1;
    PWD ruta;

    SubNombre raiz;    	
   	raiz.nombre = (char*) malloc(sizeof(char)*2); /* Reservo lugar para '/' y '\0' */
   	strcpy(raiz.nombre,"/");
    
    ruta.cont = 1;
    ruta.sub_rutas[0] = raiz;


	//SubNombre carpeta;
	//carpeta.nombre = (char*) malloc(sizeof(char)*strlen("fotos")+1);
	//strcpy(carpeta.nombre,"fotos");
	
	//ruta.cont = 2;
    //ruta.sub_rutas[1] = carpeta;
    
    //SubNombre otraCarpeta;
    //otraCarpeta.nombre = (char*) malloc(sizeof(char)*strlen("/vacaciones")+1);
	//strcpy(otraCarpeta.nombre,"/vacaciones");
	//ruta.cont = 3;
    //ruta.sub_rutas[2] = otraCarpeta;

	char* path = getPath(&ruta);
	

	pwd = (char*) malloc(sizeof(char)*strlen(path)+1);
	
    strcpy(pwd,path);
    
	printf("path: %s\n",pwd);

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
  Mensaje m1;
  m1.Mensaje_val = "archivo1.c , archivo2.c, carpeta1";
  m1.Mensaje_len = 33;
  //__________________________________________________
  
  
  int i=0;
  int j=0;
  int cont=0;
  NombreArchivos name_files[20];
    for(i=0; i<m1.Mensaje_len; i++){
      while(m1.Mensaje_val[i]!=',' && i<m1.Mensaje_len){
        name_files[cont].name[j]= m1.Mensaje_val[i];
        i++;
        j++;
      }
      j=0;
      i++; //salteo el separador;
      printf(" %s ",name_files[cont].name); 
      cont++;
   
    }
  printf("\n");
}


