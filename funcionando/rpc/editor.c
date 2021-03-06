#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <stdio.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "nodo-nodo/socketNodos.h"
#include "protocolo.h"
#include "comunicacion.h"

#define CTRL_KEY(k) ((k)&0x1f)
#define TAB_STOP 8

enum editorKey
{
	BACKSPACE = 127,
	ARROW_LEFT = 1000,
	ARROW_RIGHT,
	ARROW_UP,
	ARROW_DOWN,
	DEL_KEY,
	HOME_KEY,
	END_KEY,
	PAGE_UP,
	PAGE_DOWN
};

typedef struct erow
{
	int idx;
	int size;
	int rsize; //render size
	char *chars;
	char *render;
} erow;

struct editorConfig
{
	int cx, cy; // cursor position, current line
	int rx;
	int rowoff; // X offset
	int coloff; // Y offset
	int screenrows; // X screen size
	int screencols; // Y screen size
	int numrows; // 0 or 1
	erow *row;
	int dirty;
	char *filename;
	int nuevo;
	char* ubicacion;
	CLIENT* clnt;
	char statusmsg[80];
	time_t statusmsg_time;
	struct termios orig_termios;
};

struct editorConfig E;

/*** prototypes ***/

void editorSetStatusMessage(const char *fmt, ...);
void editorRefreshScreen();
char *editorPrompt(char *prompt, void (*callback)(char *, int));

/*** terminal ***/

void die(const char *s)
{
	write(STDOUT_FILENO, "\x1b[2J", 4);
	write(STDOUT_FILENO, "\x1b[H", 3);

	exit(1); //codigo de error desconocido
}

void disableRawMode()
{
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &E.orig_termios) == -1)
	{
		die("tcsetattr");
	}
}

void enableRawMode()
{
	if (tcgetattr(STDIN_FILENO, &E.orig_termios) == -1)
		die("tcgetattr");
	atexit(disableRawMode);
	// use it to register our disableRawMode() function to be called automatically when the program exits
	struct termios raw = E.orig_termios;
	raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
	raw.c_oflag &= ~(OPOST);
	raw.c_cflag |= (CS8);
	raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
	raw.c_cc[VMIN] = 0;
	raw.c_cc[VTIME] = 1;
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
		die("tcsetattr");
}

int editorReadKey()
{
	int nread;
	char c;
	while ((nread = read(STDIN_FILENO, &c, 1)) != 1)
	{
		if (nread == -1 && errno != EAGAIN)
			die("read");
	}

	if (c == '\x1b')
	{
		char seq[3];
		if (read(STDIN_FILENO, &seq[0], 1) != 1)
			return '\x1b';
		if (read(STDIN_FILENO, &seq[1], 1) != 1)
			return '\x1b';

		if (seq[0] == '[')
		{
			if (seq[1] >= '0' && seq[1] <= '9')
			{
				if (read(STDIN_FILENO, &seq[2], 1) != 1)
					return '\x1b';
				if (seq[2] == '~')
				{
					// pageup and pagedown <esc>[5~ and <esc>[6~
					switch (seq[1])
					{
					case '1':
						return HOME_KEY;
					case '3':
						return DEL_KEY; 
						//<esc>[3~
					case '4':
						return END_KEY;
					case '5':
						return PAGE_UP;
					case '6':
						return PAGE_DOWN;
					case '7':
						return HOME_KEY;
					case '8':
						return END_KEY;
					}
				}
			}
			else
			{
				switch (seq[1])
				{
				case 'A':
					return ARROW_UP;
				case 'B':
					return ARROW_DOWN;
				case 'C':
					return ARROW_RIGHT;
				case 'D':
					return ARROW_LEFT;
				case 'H':
					return HOME_KEY;
				case 'F':
					return END_KEY;
				}
			}
		}
		else if (seq[0] == 'O')
		{
			switch (seq[1])
			{
			case 'H':
				return HOME_KEY;
			case 'F':
				return END_KEY;
			}
		}
		return '\x1b';
	}
	else
	{
		return c;
	}
}

int getCursorPosition(int *rows, int *cols)
{
	char buf[32];
	unsigned int i = 0;

	if (write(STDOUT_FILENO, "\x1b[6n", 4) != 4)
		return -1;

	while (i < sizeof(buf) - 1)
	{
		if (read(STDIN_FILENO, &buf[i], 1) != 1)
			break;
		if (buf[i] == 'R')
			break;
		i++;
	}
	buf[i] = '\0';

	if (buf[0] != '\x1b' || buf[1] != '[')
		return -1;
	if (sscanf(&buf[2], "%d;%d", rows, cols) != 2)
		return -1;

	return 0;
}

int getWindowSize(int *rows, int *cols)
{
	struct winsize ws;

	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0)
	{
		if (write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12)
			return -1;
		return getCursorPosition(rows, cols);
	}
	else
	{
		*cols = ws.ws_col;
		*rows = ws.ws_row;
		return 0;
	}
}

int editorRowCxToRx(erow *row, int cx)
{
	int rx = 0;
	int j;
	for (j = 0; j < cx; j++)
	{
		if (row->chars[j] == '\t')
			rx += (TAB_STOP - 1) - (rx % TAB_STOP);
		rx++;
	}
	return rx;
}

int editorRowRxToCx(erow *row, int rx)
{
	int cur_rx = 0;
	int cx;
	for (cx = 0; cx < row->size; cx++)
	{
		if (row->chars[cx] == '\t')
			cur_rx += (TAB_STOP - 1) - (cur_rx % TAB_STOP);
		cur_rx++;

		if (cur_rx > rx)
			return cx;
	}
	return cx;
}

void editorUpdateRow(erow *row)
{
	int tabs = 0;
	int j;
	for (j = 0; j < row->size; j++)
		if (row->chars[j] == '\t')
			tabs++;
	
	free(row->render);
	row->render = malloc(row->size + tabs * (TAB_STOP - 1) + 1);

	int idx = 0;
	for (j = 0; j < row->size; j++)
	{
		if (row->chars[j] == '\t')
		{
			row->render[idx++] = ' ';
			while (idx % TAB_STOP != 0)
				row->render[idx++] = ' ';
		}
		else
		{
			row->render[idx++] = row->chars[j];
		}
	}
	row->render[idx] = '\0';
	row->rsize = idx;
}

void editorInsertRow(int at, char *s, size_t len)
{
	if (at < 0 || at > E.numrows)
		return;

	E.row = realloc(E.row, sizeof(erow) * (E.numrows + 1));
	memmove(&E.row[at + 1], &E.row[at], sizeof(erow) * (E.numrows - at));
	for (int j = at + 1; j <= E.numrows; j++)
		E.row[j].idx++;

	E.row[at].idx = at;

	E.row[at].size = len;
	E.row[at].chars = malloc(len + 1);
	memcpy(E.row[at].chars, s, len);
	E.row[at].chars[len] = '\0';

	E.row[at].rsize = 0;
	E.row[at].render = NULL;
	editorUpdateRow(&E.row[at]);

	E.numrows++;
	E.dirty++;
}

void editorFreeRow(erow *row)
{
	free(row->render);
	free(row->chars);
}

void editorDelRow(int at)
{
	if (at < 0 || at >= E.numrows)
		return;
	editorFreeRow(&E.row[at]);
	memmove(&E.row[at], &E.row[at + 1], sizeof(erow) * (E.numrows - at - 1));
	for (int j = at; j < E.numrows - 1; j++)
		E.row[j].idx--;
	E.numrows--;
	E.dirty++;
}

void editorRowInsertChar(erow *row, int at, int c)
{
	if (at < 0 || at > row->size)
		at = row->size; 
		// at is the index we want to insert into
	row->chars = realloc(row->chars, row->size + 2);
	memmove(&row->chars[at + 1], &row->chars[at], row->size - at + 1);
	// memmove to make room for the new charater
	row->size++;
	row->chars[at] = c;
	editorUpdateRow(row);
	E.dirty++;
}

void editorRowAppendString(erow *row, char *s, size_t len)
{
	row->chars = realloc(row->chars, row->size + len + 1);
	memcpy(&row->chars[row->size], s, len);
	row->size += len;
	row->chars[row->size] = '\0';
	editorUpdateRow(row);
	E.dirty++;
}

void editorRowDelChar(erow *row, int at)
{
	if (at < 0 || at >= row->size)
		return;
	memmove(&row->chars[at], &row->chars[at + 1], row->size - at);
	row->size--;
	editorUpdateRow(row);
	E.dirty++;
}

/*** editor operations ***/

void editorInsertChar(int c)
{
	if (E.cy == E.numrows)
	{
		editorInsertRow(E.numrows, "", 0);
	}
	editorRowInsertChar(&E.row[E.cy], E.cx, c);
	E.cx++;
}

void editorInsertNewline()
{
	if (E.cx == 0)
	{
		editorInsertRow(E.cy, "", 0);
	}
	else
	{
		erow *row = &E.row[E.cy];
		editorInsertRow(E.cy + 1, &row->chars[E.cx], row->size - E.cx);
		row = &E.row[E.cy];
		row->size = E.cx;
		row->chars[row->size] = '\0';
		editorUpdateRow(row);
	}
	E.cy++;
	E.cx = 0;
}

void editorDelChar()
{
	if (E.cy == E.numrows)
		return;
	if (E.cx == 0 && E.cy == 0)
		return;

	erow *row = &E.row[E.cy];
	if (E.cx > 0)
	{
		editorRowDelChar(row, E.cx - 1);
		E.cx--;
	}
	else
	// means E.cx == 0
	{
		E.cx = E.row[E.cy - 1].size;
		editorRowAppendString(&E.row[E.cy - 1], row->chars, row->size);
		editorDelRow(E.cy);
		E.cy--;
	}
}

/*** file i/o ***/

char *editorRowsToString(int *buflen)
{
	int totlen = 0;
	int j;
	for (j = 0; j < E.numrows; j++)
		totlen += E.row[j].size + 1;
	*buflen = totlen;

	char *buf = malloc(totlen);
	char *p = buf;
	for (j = 0; j < E.numrows; j++)
	{
		memcpy(p, E.row[j].chars, E.row[j].size);
		p += E.row[j].size;
		*p = '\n';
		p++;
	}

	return buf;
}

void editorOpen(char *filename)
{
	free(E.filename);
	E.filename = strdup(filename);
	
	editorSetStatusMessage(filename);

	FILE *fp = fopen(filename, "r");
	if (!fp)
		die("fopen");

	char *line = NULL;
	size_t linecap = 0;
	//line capacity
	ssize_t linelen;
	while ((linelen = getline(&line, &linecap, fp)) != -1)
	{
		while (linelen > 0 && (line[linelen - 1] == '\n' ||
							   line[linelen - 1] == '\r'))
			linelen--;
		editorInsertRow(E.numrows, line, linelen);
	}
	free(line);
	fclose(fp);
	E.dirty = 0;
}

void updateCoordinador(){
	
	char* ip="localhost";
	
	if(!E.nuevo){
		//report_update(E.clnt,E.filename,ip,E.ubicacion);
	}else{
		report_create(E.clnt,TIPOARCHIVO,E.filename,ip,E.ubicacion);
	}
	
}

void editorSave()
{
	if (E.filename == NULL)
	{
		E.filename = editorPrompt("Guardar como: %s (ESC to cancel)", NULL);
		if (E.filename == NULL)
		{
			editorSetStatusMessage("Guardado cancelado");
			return;
		}
	}

	int len;
	char *buf = editorRowsToString(&len);

	int fd = open(E.filename, O_RDWR | O_CREAT, 0644);
	if (fd != -1)
	{
		if (ftruncate(fd, len) != -1)
		{
			if (write(fd, buf, len) == len)
			{
				close(fd);
				free(buf);
				E.dirty = 0;
				
				//Update de sistema distribuido
				updateCoordinador();
				
				editorSetStatusMessage("%d bytes en disco", len);
				return;
			}
		}
		close(fd);
	}

	free(buf);
	editorSetStatusMessage("Error al guardar, error I/O: %s", strerror(errno));
}

void editorSaveBeforeExit()
{
	if (E.filename == NULL)
	{
		E.filename = editorPrompt("Archivo modificado. Guardar como: %s (ESC = cancelar | CTRL-Q = salir)", NULL);
		if (E.filename == NULL)
		{
			editorSetStatusMessage("Guardado cancelado");
			return;
		}
	}else{
		int salir = 1;
		while(salir){
			char* guardar = editorPrompt("Guardar cambios? si/no: %s",NULL);
			if(strcmp(guardar,"no") == 0){
				E.dirty = 0;
				die("exit");
				return;
			}else{
				if(strcmp(guardar,"si") == 0){
					salir= 0;
				}
			}
		}
	}

	//Guardar comun

	int len;
	char *buf = editorRowsToString(&len);

	int fd = open(E.filename, O_RDWR | O_CREAT, 0644);
	if (fd != -1)
	{
		if (ftruncate(fd, len) != -1)
		{
			if (write(fd, buf, len) == len)
			{
				close(fd);
				free(buf);
				E.dirty = 0;
				
				//Update de sistema distribuido
				updateCoordinador();
				
				die("exit");
				return;
			}
		}
		close(fd);
	}

	free(buf);
	editorSetStatusMessage("Error al guardar, error I/O: %s", strerror(errno));
}

struct abuf
{
	char *b;
	int len;
};

#define ABUF_INIT \
	{             \
		NULL, 0   \
	}

void abAppend(struct abuf *ab, const char *s, int len)
{
	char *new = realloc(ab->b, ab->len + len);

	if (new == NULL)
		return;
	memcpy(&new[ab->len], s, len);
	ab->b = new;
	ab->len += len;
}

void abFree(struct abuf *ab)
{
	free(ab->b);
}

/*** output ***/

void editorScroll()
{
	E.rx = 0;
	if (E.cy < E.numrows)
	{
		E.rx = editorRowCxToRx(&E.row[E.cy], E.cx);
	}
	if (E.cy < E.rowoff)
	{
		E.rowoff = E.cy;
	}
	if (E.cy >= E.rowoff + E.screenrows)
	{
		E.rowoff = E.cy - E.screenrows + 1;
	}
	if (E.rx < E.coloff)
	{
		E.coloff = E.rx;
	}
	if (E.rx >= E.coloff + E.screencols)
	{
		E.coloff = E.rx - E.screencols + 1;
	}
}

void editorDrawRows(struct abuf *ab)
{
	int y;
	for (y = 0; y < E.screenrows; y++)
	{
		int filerow = y + E.rowoff;
		if (filerow >= E.numrows) // LINEAS FUERA DEL ARCHIVO
		{
			if (E.numrows == 0 && y == E.screenrows / 3) //WELCOME PAGE si no se abrio un archivo
			{
				char welcome[80];
				int welcomelen = snprintf(welcome, sizeof(welcome),"Bienvenido al editor de texto");
				if (welcomelen > E.screencols)
					welcomelen = E.screencols;

				// center the welcome info
				int padding = (E.screencols - welcomelen) / 2;
				if (padding)
				{
					abAppend(ab, "", 1);
					padding--;
				}
				while (padding--)
				{
					abAppend(ab, " ", 1);
				}

				abAppend(ab, welcome, welcomelen);
			}
			else
			{
				abAppend(ab, "", 1);
			}
		}
		else  // LINEAS DENTRO DEL ARCHIVO
		{
			int len = E.row[filerow].rsize - E.coloff;
			
			// SCREEN BOUNDS
			if (len < 0)
				len = 0;
			if (len > E.screencols)
				len = E.screencols;
				
			char *c = &E.row[filerow].render[E.coloff];
			int j;
			for (j = 0; j < len; j++) // DE 0 A LONGITUD DE FILEROW (?
			{
				if (iscntrl(c[j]))
				{
					char sym = (c[j] <= 26) ? '@' + c[j] : '?';
					abAppend(ab, "\x1b[7m", 4);
					abAppend(ab, &sym, 1);
					abAppend(ab, "\x1b[m", 3);
				}
				else
				{
					abAppend(ab, "\x1b[39m", 5);
					abAppend(ab, &c[j], 1);
				}
			}
			abAppend(ab, "\x1b[39m", 5);
		}
		abAppend(ab, "\x1b[K", 3);
		abAppend(ab, "\r\n", 2);
	}
}

void editorDrawStatusBar(struct abuf *ab)
{
	abAppend(ab, "\x1b[7m", 4);
	char status[80], rstatus[80];
	
	int len = snprintf(status, sizeof(status), "%.20s - %d lineas %s",
					   E.filename ? E.filename : "[Archivo nuevo]", E.numrows,
					   E.dirty ? "(modificado)" : "");
	int rlen = snprintf(rstatus, sizeof(rstatus), "%d/%d", E.cy + 1, E.numrows);
	if (len > E.screencols)
		len = E.screencols;
	abAppend(ab, status, len);
	while (len < E.screencols)
	{
		if (E.screencols - len == rlen)
		{
			abAppend(ab, rstatus, rlen);
			break;
		}
		else
		{
			abAppend(ab, " ", 1);
			len++;
		}
	}
	abAppend(ab, "\x1b[m", 3);
	abAppend(ab, "\r\n", 2);
}

void editorDrawMessageBar(struct abuf *ab)
{
	abAppend(ab, "\x1b[K", 3);
	int msglen = strlen(E.statusmsg);
	if (msglen > E.screencols)
		msglen = E.screencols;
	if (msglen && time(NULL) - E.statusmsg_time < 5)
		abAppend(ab, E.statusmsg, msglen);
}

void editorRefreshScreen()
{
	editorScroll();

	struct abuf ab = ABUF_INIT;

	abAppend(&ab, "\x1b[?25l", 6);
	abAppend(&ab, "\x1b[H", 3);

	editorDrawRows(&ab);
	editorDrawStatusBar(&ab);
	editorDrawMessageBar(&ab);

	char buf[32];
	snprintf(buf, sizeof(buf), "\x1b[%d;%dH", (E.cy - E.rowoff) + 1, 
	(E.rx - E.coloff) + 1);
	abAppend(&ab, buf, strlen(buf));

	abAppend(&ab, "\x1b[?25h", 6);

	write(STDOUT_FILENO, ab.b, ab.len);
	abFree(&ab);
}

void editorSetStatusMessage(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(E.statusmsg, sizeof(E.statusmsg), fmt, ap);
	va_end(ap);
	E.statusmsg_time = time(NULL);
}

/*** input ***/

char *editorPrompt(char *prompt, void (*callback)(char *, int))
{
	size_t bufsize = 128;
	char *buf = malloc(bufsize);

	size_t buflen = 0;
	buf[0] = '\0';

	while (1)
	{
		editorSetStatusMessage(prompt, buf);
		editorRefreshScreen();

		int c = editorReadKey();
		if (c == DEL_KEY || c == CTRL_KEY('h') || c == BACKSPACE)
		{
			if (buflen != 0)
				buf[--buflen] = '\0';
		}
		else if (c == CTRL_KEY('q'))
		{
			die("Salida con exito");
		}
		else if (c == '\x1b')
		{
			editorSetStatusMessage("");
			if (callback)
				callback(buf, c);
			free(buf);
			return NULL;
		}
		else if (c == '\r')
		{
			if (buflen != 0)
			{
				editorSetStatusMessage("");
				if (callback)
					callback(buf, c);
				return buf;
			}
		}
		else if (!iscntrl(c) && c < 128)
		{
			if (buflen == bufsize - 1)
			{
				bufsize *= 2;
				buf = realloc(buf, bufsize);
			}
			buf[buflen++] = c;
			buf[buflen] = '\0';
		}

		if (callback)
			callback(buf, c);
	}
}

void editorMoveCursor(int key)
{
	erow *row = (E.cy >= E.numrows) ? NULL : &E.row[E.cy];

	switch (key)
	{
	case ARROW_LEFT:
		if (E.cx != 0)
		{
			E.cx--;
		}
		else if (E.cy > 0)
		{
			E.cy--;
			E.cx = E.row[E.cy].size;
		}
		break;
	case ARROW_RIGHT:
		if (row && E.cx < row->size)
		{
			E.cx++;
		}
		else if (row && E.cx == row->size)
		{
			E.cy++;
			E.cx = 0;
		}
		break;
	case ARROW_UP:
		if (E.cy != 0)
		{
			E.cy--;
		}
		break;
	case ARROW_DOWN:
		if (E.cy < E.numrows)
		{
			E.cy++;
		}
		break;
	}

	row = (E.cy >= E.numrows) ? NULL : &E.row[E.cy];
	int rowlen = row ? row->size : 0;
	if (E.cx > rowlen)
	{
		E.cx = rowlen;
	}
}

void editorProcessKeypress()
{

	int c = editorReadKey();

	switch (c)
	{
	case '\r':
		editorInsertNewline();
		break;

	case CTRL_KEY('q'):
		if (E.dirty)
		{
			editorSaveBeforeExit();
			return;
		}
		write(STDOUT_FILENO, "\x1b[2J", 4);
		write(STDOUT_FILENO, "\x1b[H", 3);
		exit(0);
		break;

	case CTRL_KEY('s'):
		editorSave();
		break;

	case HOME_KEY:
		E.cx = 0;
		break;

	case END_KEY:
		if (E.cy < E.numrows)
			E.cx = E.row[E.cy].size;
		break;
		
	case BACKSPACE:
	case CTRL_KEY('h'):
	case DEL_KEY:
		if (c == DEL_KEY)
			editorMoveCursor(ARROW_RIGHT);
		editorDelChar();
		break;

	case PAGE_UP:
	case PAGE_DOWN:
	{
		if (c == PAGE_UP)
		{
			E.cy = E.rowoff;
		}
		else if (c == PAGE_DOWN)
		{
			E.cy = E.rowoff + E.screenrows - 1;
			if (E.cy > E.numrows)
				E.cy = E.numrows;
		}

		int times = E.screenrows;
		while (times--)
			editorMoveCursor(c == PAGE_UP ? ARROW_UP : ARROW_DOWN);
	}
	break;

	case ARROW_UP:
	case ARROW_DOWN:
	case ARROW_LEFT:
	case ARROW_RIGHT:
		editorMoveCursor(c);
		break;

	case CTRL_KEY('l'):
	case '\x1b':
		break;
	
	default:
		editorInsertChar(c);
		break;
	}
}

void initEditor()
{
	E.cx = 0;
	E.cy = 0;
	E.rx = 0;
	E.rowoff = 0;
	E.coloff = 0;
	E.numrows = 0;
	E.row = NULL;
	E.dirty = 0;
	E.filename = NULL;
	E.nuevo= 0;
	E.statusmsg[0] = '\0';
	E.statusmsg_time = 0;

	if (getWindowSize(&E.screenrows, &E.screencols) == -1)
		die("getWindowSize");
	E.screenrows -= 2;
}

int main(int argc, char *argv[])
{
	enableRawMode();
	initEditor();
	if (argc == 2) // Si me pasan un archivo y su ubicacion
	{
		char* nombre= argv[0];
		char* ubicacion = argv[1];
		
		char* srv = "localhost";

		CLIENT* clnt = clnt_create(srv, PROY2DFS, PROY2DFSVERS,"tcp");

		if(clnt == (CLIENT*)NULL)
		{
			clnt_pcreateerror(srv);
			exit(2);
		}
		
		
		
		E.clnt = clnt;
		
		
		//RUTA COMPLETA
		char cwd[PATH_MAX];
		if (getcwd(cwd, sizeof(cwd)) == NULL) {
			exit(4); //Codigo de error de esta verga
		}
		
		char ruta [strlen(nombre)+strlen(ubicacion)+1];
		sprintf(ruta,"%s/%s",ubicacion,nombre);
		
		
		// Esto lo hacemos porque estamos en localhost y le pedimos a la misma carpeta...
		char rutaDestino [strlen(nombre)+strlen(ubicacion)+1];
		sprintf(rutaDestino,"%s/%s","Carpeta2",nombre);
		
		char carpetaNueva [200];
		sprintf(carpetaNueva,"/%s/%s",cwd,"Carpeta2");
		
		char archivoNuevo [200];
		sprintf(archivoNuevo,"%s/%s",carpetaNueva,nombre);
		
		
		mkdir(carpetaNueva, 0777);
		
		if(exists(clnt,TIPOARCHIVO,nombre,ubicacion)) { //Pregunta al coordinador si es valido un archivo.
			
			char* ip = getaddress(clnt,nombre,ubicacion);
			
			int descarga = downloadFile("localhost",ruta,archivoNuevo); //en localhost iria la ip
			
			if(descarga == 0){
				editorOpen(archivoNuevo); //abro el archivo
			}else{
				exit(1); //codigo de error de descarga del archivo
			}
		}else{
			E.nuevo=1;
			editorOpen(archivoNuevo); //creo el archivo que no existe en el sistema distribuido
		}
	}
	editorSetStatusMessage("Ctrl-S = Guardar | Ctrl-Q = Cerrar");
	while (1)
	{
		editorRefreshScreen();
		editorProcessKeypress();
	}
	return 0;
}
