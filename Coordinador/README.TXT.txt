incluir en el makefile lo siguiente:

gcc -lnsl -o coordinador `mysql_config --cflags` coordinador.c consultas.c `mysql_config --libs`