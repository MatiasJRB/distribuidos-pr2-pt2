typedef char Mensaje<>;

program PROY2DFS
{
	version PROY2DFSVERS
	{
		Mensaje LS(Mensaje)=1;
		Mensaje GETADDRESS(Mensaje)=2;
		int EXISTS(Mensaje)=3;
		int REPORT_UPDATE(Mensaje)=4;
		int REPORT_CREATE(Mensaje)=5;
		int REPORT_DELETE(Mensaje)=6;
		int IS_EMPTY(Mensaje)=7;
		int IS_FILE(Mensaje)=8;
		int REPORT_UPDATE_DIRECTORY(Mensaje)=9;
		Mensaje GET_FILES_IP(Mensaje)=10;
	}=1;
	
}=0x20001003;

