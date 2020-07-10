El header que se debe importar para utilizar la funcionalidad que proveemos (enviar y recibir un archivo de texto desde un nodo a otro) es socketNodos.h

Este header incluye cuatro funciones:
    · downloadFile(char* ip, char* route, char* destino);
    · uploadFile(FILE *file, char* ip, char* route);
    · startListening();
    · stopListening();

Para que un nodo pueda recibir pedidos de archivos de otros nodos, se debe llamar a la función startListening() en algún momento (idealmente ni bien se levanta el nodo). La función stopListening() la proveemos por las dudas, pero es muy posible que no haya que usarla nunca en este proyecto.

Una vez que se llamó a startListening(), para descargar un archivo de otro simplemente se debe llamar a la función downloadFile() con:
    · la dirección IP del nodo destino,
    · la ruta del archivo a descargar,
    · la ruta donde se lo va a guardar.

Una vez llamada la función downloadFile(), el archivo se va a encontrar en la ruta destino que se especificó.

El archivo nodo.c que se encuentra en esta carpeta lo utilizamos para probar funcionalidad, NO es parte del protocolo de comunicación.

Por ahora la función uploadFile() no está implementada ya que hay que definir el resto de las operaciones del proyecto para usarla (por esta razón, la compilación es con warnings). Sin embargo, su implementación es inversa al downloadFile().
