sudo apt-get install mariadb-client mariadb-server -y
sudo apt-get install libmysqlclient.dev
sudo apt-get install zlib1g-dev


sudo mysql  //abrir la base


//MIRAR CREAR USUARIOS Y DARLE PERMISOS
https://pimylifeup.com/raspberry-pi-mysql/

//CREAR BD Y TABLAS
CREATE DATABASE proyecto;
CREATE TABLE indexado (
    	nombre varchar(50),
    	ip varchar(50),
    	direccion varchar(50),
    	permiso varchar(1),
	    version int,
	    tipo int,
	    ruta varchar(100)
);



//agregar datos de ejemplo (TE CREA UNA ESTRUCTURA EJEMPLO PARA QUE PUEDAS PROBARLO) 
INSERT INTO indexado (nombre, ip, direccion, permiso,version,tipo,ruta)VALUES ("ArchivoA.txt", "192.168.1.30", "Carpeta1", "W",0,1,"raiz/Carpeta1/ArchivoA.txt");
INSERT INTO indexado (nombre, ip, direccion, permiso,version,tipo,ruta)VALUES ("ArchivoA.txt", "192.168.1.31", "Carpeta2", "R",1,1,"raiz/Carpeta2/ArchivoA.txt");
INSERT INTO indexado (nombre, ip, direccion, permiso,version,tipo,ruta)VALUES ("ArchivoA.txt", "192.168.1.32", "Carpeta1", "R",2,1,"raiz/Carpeta1/ArchivoA.txt");
INSERT INTO indexado (nombre, ip, direccion, permiso,version,tipo,ruta)VALUES ("ArchivoB.txt", "192.168.1.30", "Carpeta2", "W",0,1,"raiz/Carpeta2/ArchivoB.txt");
INSERT INTO indexado (nombre, ip, direccion, permiso,version,tipo,ruta)VALUES ("ArchivoC.txt", "192.168.1.31", "Carpeta2", "W",0,1,"raiz/Carpeta2/ArchivoC.txt");
INSERT INTO indexado (nombre, ip, direccion, permiso,version,tipo,ruta)VALUES ("ArchivoD.txt", "192.168.1.32", "Carpeta1", "W",0,1,"raiz/Carpeta1/ArchivoD.txt");
INSERT INTO indexado (nombre, ip, direccion, permiso,version,tipo,ruta)VALUES ("ArchivoF.txt", "192.168.1.34", "Carpeta3", "W",0,1,"raiz/Carpeta3/ArchivoF.txt");
INSERT INTO indexado (nombre, ip, direccion, permiso,version,tipo,ruta)VALUES ("ArchivoG.txt", "192.168.1.35", "Carpeta1", "W",0,1,"raiz/Carpeta1/ArchivoG.txt");
INSERT INTO indexado (nombre, ip, direccion, permiso,version,tipo,ruta) VALUES ("Carpeta1", "-", "raiz", "R",0,0,"raiz/Carpeta1");
INSERT INTO indexado (nombre, ip, direccion, permiso,version,tipo,ruta) VALUES ("Carpeta2", "-", "raiz", "R",0,0,"raiz/Carpeta2");
INSERT INTO indexado (nombre, ip, direccion, permiso,version,tipo,ruta) VALUES ("Carpeta3", "-", "raiz", "R",0,0,"raiz/Carpeta3");



//COMPILAR ARCHIVO "PRUEBA" NOMBRE EJECUTABLE
gcc -o prueba $(mysql_config --cflags) archivo.c $(mysql_config --libs)


