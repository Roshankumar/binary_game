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

int main()
{    
	/* Initialize curses */
	initCurses();

    /* Initialize windows */
	initWindows();

    /* Panel and pointers operations */
	initPanelOps();

	/*Hide about, help, gamestartmsg window */
	initPanelHide();

	/* Update the stacking order. 2nd panel will be on top */
	update_panels();
    
    /* Show it on the screen */
	doupdate();

	/* Main menu operation */
	frontMenu();

	/* Deallocate memory and Stop curses */
    endwin();

    return 0;
}
