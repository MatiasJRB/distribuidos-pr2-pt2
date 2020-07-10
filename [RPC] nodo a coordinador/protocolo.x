typedef char Mensaje<>;

program PROY2DFS
{
	version PROY2DFSVERS
	{
		Mensaje LS(Mensaje)=1;
		Mensaje GETADDRESS(Mensaje)=2;
		Mensaje MKDIR(Mensaje)=3; //el coordinador si no puede crear esa direccion debe retornar que no pudo hacerlo
		Mensaje CD(Mensaje)=4; //el coordinador debe indicar si no existe la direccion
		Mensaje RM(Mensaje)=5; //el coordinador si no puede remover ese archivo debe retornar que no pudo hacerlo
		Mensaje CPY(Mensaje)=6; 
		Mensaje MV(Mensaje)=7;		
	}=1;
	
}=0x20001003;

