
#include <string.h>
#include <unistd.h>

#include "console.h"
#include "example.h"

#define GAME_ROWS 24
#define GAME_COLS 80

/**** DIMENSIONS MUST MATCH the ROWS/COLS */

char *GAME_BOARD[] = {
"                                   Lives:",
"/------\\          /------\\          /------\\          /------\\          /------\\",
"|      |          |      |          |      |          |      |          |      |",
"+      +----------+      +----------+      +----------+      +----------+      +",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"",
"",
"",
"" };

#define PLAYER_ANIM_TILES 2
#define PLAYER_HEIGHT 2
static char* PLAYER_GRAPHIC[PLAYER_ANIM_TILES][PLAYER_HEIGHT+1] = {
  {"@@",
   "<>"},
  {"--",
   "<>"}
};

void exampleRun()
{
	int i;
	if (consoleInit(GAME_ROWS, GAME_COLS, GAME_BOARD))
        {
		for (i = 0; i<PLAYER_ANIM_TILES; i++)
		{
			char** tile = PLAYER_GRAPHIC[i];

			consoleClearImage(10,10,PLAYER_HEIGHT, strlen(tile[0]));
			consoleDrawImage(10, 10, tile, PLAYER_HEIGHT);
			consoleRefresh();
			sleep(1);
		}		

                finalKeypress(); /* wait for final key before killing curses and game */
        }       
        consoleFinish();        	
}

