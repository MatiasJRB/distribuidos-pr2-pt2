typedef char Mensaje<>;

program PROY2DFS
{
	version PROY2DFSVERS
	{
		Mensaje LS(Mensaje)=1;
		Mensaje GET_ADDRESS(Mensaje)=2;
		Mensaje UPDATE_ADDRESS(Mensaje)=3;
		Mensaje REPORT_CREATE(Mensaje)=4;
		Mensaje REPORT_DELETE(Mensaje)=5;
		Mensaje IS_VALID(Mensaje)=6;
	}=1;
	
}=0x20001003;

