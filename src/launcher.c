/*
 * Name:	Marcus Karpoff
 * Class:	Cmput-379
 * Assignment:	Assignment-3
 * Due Date:	Dec. 1, 2013
 *
 * Copyright (c) 2013 Marcus Karpoff <mkarpoff@ualberta.ca>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "launcher.h"

int
main(int argc, char * argv[]){
	bool colour = FALSE;
	bool keys = FALSE;

	initscr();
	start_color();
	keypad(stdscr, TRUE);

	crmode();
	clear();

	/* This code checks the screen dimensions */
	if (COLS < 80 && LINES < 21 ) {
	 	endwin();
		printf("Terminal Dimensions: (W,H) (%d, %d)\n"
		    "The terminal MUST HAVE A WIDTH of at least 80\n"
		    "The terminal MUST HAVE A HEIGHT of at least 21\n"
		    , LINES, COLS);
		exit(0);
	}
	else if (COLS < 80) {
		endwin();
		printf("Terminal Dimensions: (W,H) (%d, %d)\n"
		    "The terminal MUST HAVE A WIDTH of at least 80\n"
		    , LINES, COLS);
		exit(0);
	}
	else if (LINES < 21 ) {
		endwin();
		printf("Terminal Dimensions: (W,H) (%d, %d)\n"
		    "The terminal MUST HAVE A HEIGHT of at least 21\n"
		    , LINES, COLS);
		exit(0);
	}
	
	if( has_colors())
		colour = TRUE;

	if( has_key(KEY_LEFT) && has_key(KEY_RIGHT) && has_key(KEY_UP) 
	    && has_key(KEY_DOWN))
		keys = TRUE;	

	/* Saucer with special keys and colours */
	if ( keys && colour){
		endwin();
		execv("./bin/saucerac", argv); 
		err(1, "exec failed");
		return 1;
	}

	/* Sacuer with special keys no colours */
	if ( keys && !colour){
		endwin();
		execv("./bin/sauceranoc", argv);
		err(1, "Exec failed");
		return 1;
	}

	/* Saucer with no special keys and colours */
	if (!keys && colour){
		endwin();
		execv("./bin/saucernoac", argv);
		err(1,"Exec failed");
		return 1;
	}

	/* Saucer with no special keys and no colours */
	if (!keys && !colour){
		endwin();
		execv("./bin/saucernoanoc", argv);
		err(1, "Exec failed");
		return 1;
	}
}
