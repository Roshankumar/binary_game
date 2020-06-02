/* Copyright (C) 2020 Roshankumar Bhamare
 * www.linkedin.com/in/roshankumarbhamare
 * 
 * This file is part of Binary
 *
 * Binary is free software: you can redistribute it3 and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * Binary is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef BINARY_H_
#define BINARY_H_

#include <pdcurs36/panel.h>
#include <pdcurs36/curses.h>

#define WINCOUNT 13			//Total windows count

#pragma pack(1)

typedef struct gameData{		//structure to store game data
	short int score;
	short int lastDeci;
	short int enterpress;
	char userinput[4];
}gD_t;

typedef struct panelData{		//structure to save panel visibility status (TRUE = hidden, FALSE = visible);
	short int hide;
}pD_t;

WINDOW *myWins[WINCOUNT];		//WINDOW structure variable
PANEL *myPanels[WINCOUNT];		//PANEL structure variable
pD_t pData[WINCOUNT];			//struture variable to save panel visibility status
pD_t *temp,*temp1;				//struture variable to control panel visibility status

//Function declarations
void initCurses();
void initWindows();
void initPanelOps();
void initPanelHide();
void frontMenu();
void gameName(WINDOW **wins,int i);
void initOuterWindow(WINDOW **wins,int i);
void detectOs(WINDOW **wins,int i);
void mainMenu(WINDOW **wins,int i);
void help(WINDOW **wins,int i);
void about(WINDOW **wins,int i);
void gameStartMsg(WINDOW **wins,int i);
void gameWindow(WINDOW **wins,int i);
void gameMenu();
void decimalNo(WINDOW **wins,int i);
void binaryNo(WINDOW **wins,int i);
void decimalDisp(WINDOW **wins,int i);
void binaryInput(WINDOW **wins,int i);
void gameResult(WINDOW **wins,int i);
void printJustified(int i, int starty, int startx, int width, char *string, chtype color);
void initGame();
void swap(int *a, int *b);
void shuffleNumbers(int arr[], int n );
double modOfDouble(double a);
int convertToDeci();
void gameCal();
double game();
char* calTime(double time_taken);
char* convertToBinary(int n);

#endif
