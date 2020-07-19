typedef char Mensaje<>;

program PROY2DFS
{
	version PROY2DFSVERS
	{
		Mensaje LS(Mensaje)=1;
		Mensaje GETADDRESS(Mensaje)=2;
		int EXISTS(Mensaje)=3;
		Mensaje UPDATE_ADDRESS(Mensaje)=4;
		int REPORT_CREATE(Mensaje)=5;
		Mensaje REPORT_DELETE(Mensaje)=6;
		int IS_EMPTY(Mensaje)=7;
	}=1;
	
}=0x20001003;

