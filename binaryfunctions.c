/* Copyright (C) 2020 Roshankumar Bhamare
 * www.linkedin.com/in/roshankumarbhamare
 * 
 * This file is part of Binary
 *
 * Binary is free software: you can redistribute it and/or modify
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

#include "include/binary.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <ctype.h>

//Global items
int mainRowG = 40, mainColG = 70;   //variables to save outer border size
int mainPosXG = 0, mainPosYG = 0;	//variables to save x and y coordinates
gD_t gameValue;						//Game data strucutre variable
static int gameCount = 0;			//Variable to store game play count

void initCurses()							//program initializing function
{
	int i;
	initscr();								//Allocate memory and clear the screen
	cbreak();								//Terminal in cbreake mode and process single char at a time
	noecho();								//Do not display(print) typed characters on screen
	curs_set(FALSE);						//Make cursur invisible
	keypad(stdscr, TRUE);					//Make keypad enable
    resize_term(mainRowG, mainColG);    	//resize window to 40x70
	#ifdef A_COLOR							//conditional compilation
    	if(has_colors()) start_color();		//has color -> checks whether terminal supports color or not
		init_pair(1,COLOR_WHITE,COLOR_BLACK);
		for (i = 16; i < 256; i++)
			init_pair(i, i, COLOR_BLACK);	//Creating multiple text colors with common black background
	#endif									//closes #ifdef directive
}

void initWindows()				//function calling different window creating functions
{
	initOuterWindow(myWins,0);	//outer boarder window(0)
    detectOs(myWins,1);			//user os detecting window(1)	
	mainMenu(myWins,2);			//main menu and controls window(2)
    gameName(myWins,3);			//Game name window(3)
    help(myWins,4);				//help for user window(4)
	about(myWins,5);			//Game info window(5)
	gameStartMsg(myWins,6);		//game start/status message window(6)
	gameWindow(myWins,7);		//actual game window(7)
	decimalNo(myWins,8);		//label window to display decimal numer(8)
	binaryNo(myWins,9);			//label window for user input(9)
	decimalDisp(myWins,10);		//window displaying decimal number(10)
	binaryInput(myWins,11);		//window accepting user binary input(11)
	gameResult(myWins,12);		//window showing game result details(12)
}

void initPanelOps()				//function initializing panels
{
	int i;
	/* Attach a panel to each window */ 	/* Order is bottom up */
	/* After Push 12, order: stdscr-0-1-2-3-4-5-6-7-8-9-10-11-12 */
	for(i=0;i<WINCOUNT;i++)
		myPanels[i] = new_panel(myWins[i]);

	/* Initialize panel datas saying that nothing is hidden */
	for(i=0;i<WINCOUNT;i++)
		pData[i].hide = FALSE;

	for(i=0;i<WINCOUNT;i++)
		set_panel_userptr(myPanels[i], &pData[i]);
}

void initPanelHide()			//function hideing some panels before start of game
{
	int i;
	for(i=0;i<WINCOUNT;i++)
	{
		//panel no < 4 are visible, others hidden
		if(i<4)
		{
			temp = (pD_t *)panel_userptr(myPanels[i]);
			show_panel(myPanels[i]);
			temp->hide = FALSE;
		}
		else
		{
			temp = (pD_t *)panel_userptr(myPanels[i]);
			hide_panel(myPanels[i]);
			temp->hide = TRUE;
		}
	}
}

void frontMenu()			//main menu function
{
	int i, ch = 0;
	while((ch = getch()) != 'q')
	{	
		switch(ch)
		{	
			case 'h':
				temp = (pD_t *)panel_userptr(myPanels[4]);		//selecting panel using panel pointer
				temp1 = (pD_t *)panel_userptr(myPanels[5]);
				show_panel(myPanels[4]);
				hide_panel(myPanels[5]);	
				temp->hide = FALSE;			//displaying help window and hiding about window
				temp1->hide = TRUE;
				break;
			case 'a':			
				temp = (pD_t *)panel_userptr(myPanels[5]);
				temp1 = (pD_t *)panel_userptr(myPanels[4]);
				show_panel(myPanels[5]);
				hide_panel(myPanels[4]);
				temp->hide = FALSE;
				temp1->hide = TRUE;
				break;
			case 10:								//Entering into game
				for(i=1;i<6;i++)
				{
					temp = (pD_t *)panel_userptr(myPanels[i]);
					hide_panel(myPanels[i]);
					temp->hide = TRUE;
				}
				temp = (pD_t *)panel_userptr(myPanels[6]);
				show_panel(myPanels[6]);
				temp->hide = FALSE;
				update_panels();
				doupdate();
				gameMenu();
				break;
			default :			
				temp = (pD_t *)panel_userptr(myPanels[4]);
				temp1 = (pD_t *)panel_userptr(myPanels[5]);
				hide_panel(myPanels[4]);
				hide_panel(myPanels[5]);
				temp->hide = TRUE;
				temp1->hide = TRUE;
				break;
		}
		update_panels();
		doupdate();
	}
}

void initOuterWindow(WINDOW **wins,int i)		//defining outer window
{
    wins[i] = newwin(mainRowG,mainColG,mainPosXG,mainPosYG);
    
	wattron(wins[i],COLOR_PAIR(255));
		wborder(wins[i],'|','|','-','-','*','*','*','*');
	wattroff(wins[i],COLOR_PAIR(255));

    wrefresh(wins[i]);
}

void detectOs(WINDOW **wins,int i)				//function to detect OS of user
{
	wins[i]  = newwin(mainRowG-38,mainColG-44,mainPosXG+37,mainPosYG+22);
    char osname[26];

	#ifdef _WIN32
	    #ifdef _WIN64
	        sprintf(osname," %s ","Your OS: Windows 64 bit");
	    #else 
	        sprintf(osname," %s ","Your OS: Windows 32 bit");
	    #endif
	#elif __linux__
	    sprintf(osname," %s ","Your OS: Linux");
	#elif __unix__
	    sprintf(osname," %s ","Your OS: Other unix OS");
	#else
	    sprintf(osname," %s ","Unable to Detect Your OS");
	#endif

	printJustified(i, 0, 0, (mainColG-44), osname, COLOR_PAIR(255));
	wrefresh(wins[i]);
}

void mainMenu(WINDOW **wins,int i)			//Main control menu
{	
	wins[i] = newwin(mainRowG-37,mainColG-32,mainPosXG+32,mainPosYG+16);
 	char mainMenu1[] = {"-> Press ENTER to play <-"};
	char mainMenu2[] = {"Help (h)  |  About (a)  |  Quit (q)"};
	printJustified(i, 0, 0, (mainColG-30), mainMenu1, COLOR_PAIR(227));
	printJustified(i, 2, 0, (mainColG-30), mainMenu2, COLOR_PAIR(229));
    wrefresh(wins[i]);
}

void gameName(WINDOW **wins,int i)			//defining game name
{
    wins[i]  = newwin(mainRowG-28,mainColG-10,mainPosXG+2,mainPosYG+5);
	char name[12][60] ={{"1100000011                                                10"},
						{"11       11    0                                          10"},
						{"11        11                                   01        10 "},
						{"11       11    1                                01      10  "},
						{"1100000011     1   1                             01    10   "},
						{"11      11     1   1000001      110011   1        01  10    "},
						{"11       11    1   1      1    1      1   1   1     11      "},
						{"11        11   1   1       1   1      1    1 1      11      "},
						{"11       11    1   1       1   1      1     1       11      "},
						{"1100000011     1   1       1    110011 000  1       11      "},
						{" "},
						{"------------------------------- a digital number system game"}};
		
	int x, colorno = 21;

	for(x = 0; x < 11; x++)
    {
		wattron(myWins[i],COLOR_PAIR(colorno));
			mvwaddstr(wins[i],x,0,name[x]);
		wattroff(myWins[i],COLOR_PAIR(colorno));
		if((x%2)!=0) colorno+=6;
    }

	wattron(myWins[i],COLOR_PAIR(157));
	mvwaddstr(wins[i],11,0,name[11]);
	wattroff(myWins[i],COLOR_PAIR(157));
}

void help(WINDOW **wins,int i)
{
    char label[6]={"HELP"};
	char label1[]={"-:Players Objective:-"};
	char label2[]={"-:Controls:-"};
    wins[i]  = newwin(mainRowG-25,mainColG-30,mainPosXG+15,mainPosYG+15);
	wattron(wins[i],COLOR_PAIR(91));
		box(wins[i],0,0);
		mvwaddch(wins[i], 2, 0, ACS_LTEE); 
		mvwhline(wins[i], 2, 1, ACS_HLINE, (mainColG-30) - 2); 
		mvwaddch(wins[i], 2, (mainColG-30) - 1, ACS_RTEE);
	wattroff(wins[i],COLOR_PAIR(91));
	wattron(wins[i],COLOR_PAIR(1));
    printJustified(i, 1, 0, (mainColG-30), label, COLOR_PAIR(148));
    printJustified(i, 3, 0, (mainColG-30), label1, COLOR_PAIR(1));
	mvwprintw(wins[i],4,1,"To enter 4-bit binary equivalent for");
	mvwprintw(wins[i],5,1,"given decimal number");
	printJustified(i, 7, 0, (mainColG-30), label2, COLOR_PAIR(1));
	mvwprintw(wins[i],8,1,"Info. is displyed either at bottom or");
	mvwprintw(wins[i],9,1,"top side of screen.");
	mvwprintw(wins[i],10,1,"Letter in brackets '( )' controls");
	mvwprintw(wins[i],11,1,"respective operation.");
	mvwprintw(wins[i],13,1,"Press 'any key' to exit this window");
    wrefresh(wins[i]);
}

void about(WINDOW **wins,int i)
{
    char label[6]={"ABOUT"};
	char version[]={"-:Binary v1.0:-"};
	char copyright[]={"Copyright (C) 2020 Roshankumar Bhamare"};
	char license1[]={"GNU GENERAL PUBLIC LICENSE"};
	char license2[]={"Version 3"};
	char contact1[]={"-:Contact:-"};
	char contact2[]={"www.linkedin.com/in/roshankumarbhamare"};
    wins[i]  = newwin(mainRowG-24,mainColG-28,mainPosXG+15,mainPosYG+14);
	wattron(wins[i],COLOR_PAIR(91));
		box(wins[i],0,0);
		mvwaddch(wins[i], 2, 0, ACS_LTEE); 
		mvwhline(wins[i], 2, 1, ACS_HLINE, (mainColG-28) - 2); 
		mvwaddch(wins[i], 2, (mainColG-28) - 1, ACS_RTEE); 
	wattroff(wins[i],COLOR_PAIR(91));
    printJustified(i, 1, 0, (mainColG-28), label, COLOR_PAIR(148));
	printJustified(i, 3, 0, (mainColG-28), version, COLOR_PAIR(1));
	wattron(wins[i],COLOR_PAIR(1));
	mvwprintw(wins[i],4,1,"The game is been designed by developer");
	mvwprintw(wins[i],5,1,"to practice with PDcurses in c language.");
	mvwprintw(wins[i],6,1,"Users valuable suggestions/comments");
	mvwprintw(wins[i],7,1,"are always welcome.");
	printJustified(i, 9, 0, (mainColG-28), contact1, COLOR_PAIR(1));
	printJustified(i, 10, 0, (mainColG-28), contact2, COLOR_PAIR(1));
	printJustified(i, 12, 0, (mainColG-28), copyright, COLOR_PAIR(1));
	printJustified(i, 13, 0, (mainColG-28), license1, COLOR_PAIR(1));
	printJustified(i, 14, 0, (mainColG-28), license2, COLOR_PAIR(1));
    wrefresh(wins[i]);
}

//Function prints text in middle of window with defined width, row position, text and color
void printJustified(int i, int col, int row, int width, char *text, chtype colorpair)
{	
	int length = 0, x = 0, y = 0;
	float temp;

	if(myWins[i] == NULL)	myWins[i] = stdscr;
	
	getyx(myWins[i], y, x);
	if(row != 0)	x = row;
	if(col != 0)	y = col;
	if(width == 0)	width = mainColG;

	length = strlen(text);
	temp = (width - length)/ 2;
	x = row + (int)temp;

	wattron(myWins[i], colorpair);
		mvwprintw(myWins[i], y, x, "%s", text);
	wattroff(myWins[i], colorpair);

	refresh();
}

void gameStartMsg(WINDOW **wins,int i)		//i = 6
{
	char label[] = {"Press 's' to start game"};
	wins[i] = newwin(mainRowG-37,mainColG-42,mainPosXG+5,mainPosYG+21);
	wattron(myWins[i],COLOR_PAIR(47));
		box(wins[i],0,0);
	wattroff(myWins[i],COLOR_PAIR(47));
		mvwprintw(wins[i],1,1,"  ");
	printJustified(i, 1, 0, (mainColG-42), label, COLOR_PAIR(227));
 	wrefresh(wins[i]);
}

void gameWindow(WINDOW **wins,int i)		//i = 7
{
	char label[16] = {"Lets begin...!!"};
	char stopgame[] = {"Press ENTER to stop game"};
	wins[i] = newwin(mainRowG-35,mainColG-42,mainPosXG+3,mainPosYG+21);
	printJustified(i, 1, 0, (mainColG-42), label, COLOR_PAIR(47));
	printJustified(i, 3, 0, (mainColG-42), stopgame, COLOR_PAIR(47));
 	wrefresh(wins[i]);
}

void gameMenu()					//main game menu
{
	int i, ch = 0;
	while((ch = getch()))
	{	
		switch(ch)
		{
			case 'h':
				temp = (pD_t *)panel_userptr(myPanels[4]);		//selecting panel using panel pointer
				temp1 = (pD_t *)panel_userptr(myPanels[5]);
				show_panel(myPanels[4]);
				hide_panel(myPanels[5]);	
				temp->hide = FALSE;			//displaying help window and hiding about window
				temp1->hide = TRUE;
				break;
			case 'a':			
				temp = (pD_t *)panel_userptr(myPanels[5]);
				temp1 = (pD_t *)panel_userptr(myPanels[4]);
				show_panel(myPanels[5]);
				hide_panel(myPanels[4]);
				temp->hide = FALSE;
				temp1->hide = TRUE;
				break;
			case 's':
				temp = (pD_t *)panel_userptr(myPanels[6]);
				hide_panel(myPanels[6]);
				temp->hide = TRUE;
				for(i=7;i<WINCOUNT;i++)
				{
					temp1 = (pD_t *)panel_userptr(myPanels[i]);
					show_panel(myPanels[i]);
					temp1->hide = FALSE;
				}
				initGame();
				break;
			case 'q': 
				exit(0);	
				break;
			default :			
				temp = (pD_t *)panel_userptr(myPanels[4]);
				temp1 = (pD_t *)panel_userptr(myPanels[5]);
				hide_panel(myPanels[4]);
				hide_panel(myPanels[5]);
				temp->hide = TRUE;
				temp1->hide = TRUE;
				break;
		}
		update_panels();
		doupdate();
	}
}

void decimalNo(WINDOW **wins,int i)
{
	wins[i] = newwin(mainRowG-36,mainColG-60,mainPosXG+12,mainPosYG+20);
	wattron(wins[i],COLOR_PAIR(255));
	mvwprintw(wins[i],0,0,"Decimal No");
	wrefresh(wins[i]);
}

void binaryNo(WINDOW **wins,int i)
{
	wins[i] = newwin(mainRowG-36,mainColG-60,mainPosXG+12,mainPosYG+40);
 	wattron(wins[i],COLOR_PAIR(255));
	mvwprintw(wins[i],0,0,"Binary No");
	wrefresh(wins[i]);
}

void decimalDisp(WINDOW **wins,int i)
{
	wins[i] = newwin(mainRowG-36,mainColG-60,mainPosXG+14,mainPosYG+24);
	wattron(wins[i],COLOR_PAIR(255));
	wrefresh(wins[i]);
}

void binaryInput(WINDOW **wins,int i)
{
	wins[i] = newwin(mainRowG-39,mainColG-66,mainPosXG+14,mainPosYG+42);
	wattron(wins[i],COLOR_PAIR(255));
	wrefresh(wins[i]);
}

void gameResult(WINDOW **wins,int i)
{
	wins[i] = newwin(mainRowG-28,mainColG-20,mainPosXG+20,mainPosYG+10);
	wattron(wins[i],COLOR_PAIR(255));
	wrefresh(wins[i]);
}

void initGame()					//initializing game
{
	/*Hide all except window 0, 7+ window */
	int i;
	for(i=0;i<WINCOUNT;i++)
	{
		if((i==0) || (i>6))
		{
			temp = (pD_t *)panel_userptr(myPanels[i]);
			show_panel(myPanels[i]);
			temp->hide = FALSE;
		}
		else
		{
			temp = (pD_t *)panel_userptr(myPanels[i]);
			hide_panel(myPanels[i]);
			temp->hide = TRUE;
		}
		update_panels();
		doupdate();
	}
	gameCal();
}

void gameCal()				//executing game and calculating game parameters
{
	int ch = 0, dec;
	char label0[19];
	char label1[36];
	char label2[13];
	char label3[14];
	char label4[20];
	char label5[23];
	char *b, *time;

	temp = (pD_t *)panel_userptr(myPanels[12]);
	show_panel(myPanels[12]);
	temp->hide = FALSE;

	//Calling game logic
	clock_t t; 
    t = clock(); 

	game();

	t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds 

	noecho();

	++gameCount;
	
	time = calTime(time_taken);
	werase(myWins[12]);

	dec = convertToDeci();	//getting decimal of wrong binary input of user
	b = convertToBinary(gameValue.lastDeci);
	
	if(gameValue.score < 15)
	{
		if((dec < 0) || (gameValue.enterpress == 1))
		{
			sprintf(label1,"You stopped the game..!!!");	//25 char
			printJustified(12, 2, 0, (mainColG-20), label1, COLOR_PAIR(190));
		}
		else
		{
			sprintf(label0,"Wrong answer...!!!");	//18 char
			sprintf(label1,"Your answer %4s(%d) | Correct answer %4s(%d)", gameValue.userinput, dec, b, gameValue.lastDeci);	//43 char
			printJustified(12, 0, 0, (mainColG-20), label0, COLOR_PAIR(196));
			printJustified(12, 2, 0, (mainColG-20), label1, COLOR_PAIR(255));
		}
	}
	else
	{
		sprintf(label1,"You Win this game..!!");	//36 char
		printJustified(12, 2, 0, (mainColG-20), label1, COLOR_PAIR(184));
	}

	sprintf(label2,"* Game Over *");	//13 char
	printJustified(12, 4, 0, (mainColG-20), label2, COLOR_PAIR(195));
	
	sprintf(label3,"Score is: %d/15",gameValue.score);	//14 char
	printJustified(12,6,0,(mainColG-20), label3, COLOR_PAIR(228));
	
	if(gameValue.score != 15)
	{
		sprintf(label4,"You lost by %d points",15-gameValue.score);	//20 char
		printJustified(12,8,0,(mainColG-20), label4, COLOR_PAIR(228));
	}
	else
	{
		sprintf(label4,"Try again, make it quick!");	//20 char
		printJustified(12,8,0,(mainColG-20), label4, COLOR_PAIR(255));
	}
	
	sprintf(label5,"Game play time: %s",time);	//23 char
	printJustified(12,10,0,(mainColG-20), label5, COLOR_PAIR(207));
	
	wrefresh(myWins[12]);
}

void game()				//actual game execution
{	
	#ifdef PDCURSES
		PDC_set_blink(TRUE);			
		PDC_set_bold(TRUE);
	#endif

	int i, j, dec = 0;
	char label[4][28] = {{"Go GO GO...!!"},{"Keep it up...!!"},{"You're almost there...!!"},{"*** Bingo...!!! ***"}};
	char stop[] = {"Press ENTER to stop game"};
	char replay[] = {"Restart game(s) | Quit(q)"};
	char playagain[] = {"Lets begin..!!!"};
	char score[12];
	
	if(gameCount!=0)
	{
		werase(myWins[10]);
		werase(myWins[12]);
		wrefresh(myWins[12]);
		werase(myWins[7]);
		printJustified(7, 1, 0, (mainColG-42), playagain, COLOR_PAIR(219));
		printJustified(7, 3, 0, (mainColG-42), stop, COLOR_PAIR(219));
		wrefresh(myWins[7]);
		gameValue.score = 0;
		gameValue.lastDeci = 0;
		gameValue.enterpress = 0;
		strcpy(gameValue.userinput,"0000");
	}
	
	int no[15] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};

	echo();
	curs_set(TRUE);

	for(j=0;j<15;j++)
	{
		mvwprintw(myWins[10],0,0,"%d",no[j]);
		wrefresh(myWins[10]);
		wgetnstr(myWins[11],gameValue.userinput,4);
		
		dec = convertToDeci();
		
		if(no[j] == dec)
		{
			beep();
			gameValue.score += 1;
			sprintf(score,"Score is %d",gameValue.score);
			printJustified(12, 2, 0, (mainColG-20), score, COLOR_PAIR(228));
			wrefresh(myWins[12]);
			werase(myWins[11]);
		}
		else
		{
			attron(COLOR_PAIR(1));
			flash();
			attroff(COLOR_PAIR(1));
			gameValue.lastDeci = no[j];		//returning non zero value, a proof that game is played by user
			werase(myWins[11]);
			noecho();
			break;
		}
		wrefresh(myWins[11]);
		
		werase(myWins[7]);
		if((j>=0) && (j<=3))	i=0;
		else if((j>3) && (j<=10)) i=1;
		else i=2;

		printJustified(7, 1, 0, (mainColG-42), label[i], COLOR_PAIR(47));
		printJustified(7, 3, 0, (mainColG-42), stop, COLOR_PAIR(47));
		wrefresh(myWins[7]);	
	}

	curs_set(FALSE);

	if(gameValue.score<15)
	{
		wattron(myWins[7], A_BLINK);
		werase(myWins[7]);
		printJustified(7, 1, 0, (mainColG-42),replay, COLOR_PAIR(47));
		wrefresh(myWins[7]);	
		wattroff(myWins[7], A_BLINK);
		wrefresh(myWins[7]);	
	}
	else
	{
		werase(myWins[7]);
		wattron(myWins[7], A_BLINK);
		printJustified(7, 1, 0, (mainColG-42),label[3], COLOR_PAIR(226));
		printJustified(7, 3, 0, (mainColG-42),replay, COLOR_PAIR(47));
		wrefresh(myWins[7]);	
		wattroff(myWins[7], A_BLINK);
		wrefresh(myWins[7]);
	}
}

int convertToDeci()				//function to convert unser binary input to decimal value
{
	int j, a, p = 3, dec = 0;
	for(j=0;j<4;j++)	
	{
		a = gameValue.userinput[j] - '0';
		dec+=(a*pow(2,p));
		--p;
	}
	return dec;
}

char* calTime(double time_taken)	//function to calculate time take by user for a game play
{
	int hr = 0, min = 0,sec;
	static char usertime[8];
	sec = time_taken;

    hr = sec / 3600; 
    sec %= 3600; 
    min = sec / 60 ; 
    sec %= 60;
	sprintf(usertime,"%d:%d:%d",hr,min,sec);
	return usertime;
}

char* convertToBinary(int n)			//function to convert decimal number to binary
{
	int i,j,m,a[4];
	static char binary[4];
    
	for (i = 3; i >= 0; i--)
	{
		m = n >> i;
		if (m & 1)
			a[i] = 1;
		else
			a[i] = 0;
	}
	for(i=3,j=0;(i>=0) && (j<4);i--,j++)
	{
		sprintf(&binary[j],"%d",a[i]);
	}
	return binary;
}
