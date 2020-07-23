int inicializador()
{
	system("./getFiles.sh"); //Ejecuto el bash para obtener los archivos del nodo
	//Ahora tengo que leer el archivo Log
	FILE *fp = fopen("log", "r");
	if (fp == 0)
	{
		printf("Ocurrio un error al abrir el archivo.\n");
		return -1;
	}
	else
	{
		//Debo leer el contenido
		char * line = NULL;
		size_t len = 0;
		ssize_t read;
		while ((read = getline(&line, &len, fp)) != -1) {
			printf("Retrieved line of length %zu:\n", read);
			printf("%s", line);
		}
		fclose(fp);
	}
}
