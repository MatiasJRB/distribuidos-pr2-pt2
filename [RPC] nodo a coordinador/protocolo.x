typedef char Mensaje<>;

program PROY2DFS
{
	version PROY2DFSVERS
	{
		Mensaje LS(Mensaje)=1;
		Mensaje GETADDRESS(Mensaje)=2;
		Mensaje MKDIR(Mensaje)=3; 
		Mensaje CD(Mensaje)=4; 
		Mensaje RM(Mensaje)=5; 
		Mensaje CPY(Mensaje)=6; 
		Mensaje MV(Mensaje)=7;		
	}=1;
	
}=0x20001003;

