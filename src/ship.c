/*
 * Name:	Marcus Karpoff
 * Class:	Cmput-379
 * Assignment:	Assignement-3
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

/*
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 */

#include "ship.h"

/*
 * This launcher launches ships at random intervals. The max length of these
 * intervals decreases over time to make the game more challenging over time
 */
void *
ship_launcher(void *arg)
{
	int i;
	int st;

	pthread_mutex_lock(&mx_r);
	ready += 1;
	pthread_mutex_unlock(&mx_r);
	/* wait for the user to exit the loading screen */
	while(load == 1)
		pthread_yield();

	/* The sleep allows the user to get a second to prepar themselves */
	sleep(1);
	while(1){
		/* Look for a non active ship*/
		for (i=0; i < MAXSHIP; i++){
		 	if (ship_flag[i] == 0){
		 		ship_flag[i] = 1;
		 		if ( sem_post(&sm_ship[i])) {
					err(1, "Sem post failed");
					pthread_exit(NULL);
				}		
				break;
		 	}
		 }

		/* 
		 * Sleeps for a random amount calculated as:
		 * (rand) * (the max time at that point int the game) 
		 * rand is a value from 0 to 5
		 */
		st = GTUNIT - mtime*1000;
		if (st <= 0)
			st = 1;
		usleep( rand()%5 * (st));
	}
}

/*
 * These threads wait for the activation and handle all of the ship animations
 */
void *
ship_thread(void *arg)
{
	struct thd_prop *sh = arg;
	int thn = sh->id;
	pthread_mutex_lock(&mx_r);
	ready += 1;
	pthread_mutex_unlock(&mx_r);
	while(1) {
		if ( sem_wait(&sm_ship[thn])) {
			err(1, "Sem wait failed");
			pthread_exit(NULL);
		}
		animate_ship(thn);
		ship_flag[thn] = 0;
	}
}

/* 
 * This causes the ship to run as a ship. It has really long animation so when
 * optimization is turned on it will completely unfold everything.
 */
void 
animate_ship(int thn)
{

	int row = (rand()%7);
	/* 
	 * The -mtime/60 makes is so that every 60 sec the easiest ship 
	 * to hit is removed from the options
	 */
	int delay ;
	int st = 13 - mtime/60;
	if (st <= 0)
		delay = 3;
	else
		delay = 3+(rand()%(st));
	int i;
	int blownup = 0;
	int col = 0;
	int colour = (delay-3)/2;
	/* set the row and column to the start*/
	pthread_mutex_lock(&mx_ship_flag);
	ship_loc[thn][0] = row;
	ship_loc[thn][1] = -4;
	pthread_mutex_unlock(&mx_ship_flag);
	/* Draw the ship */
	pthread_mutex_lock(&mx_draw);
	move( row, col);
	colouron(colour);
	addstr( ">");
	colouroff(colour);
	move(21-21, COLS-COLS);
	refresh();
	pthread_mutex_unlock(&mx_draw);
	/* Sleep for the delay time*/
	usleep(delay*TUNIT);
	pthread_mutex_lock(&mx_ship_flag);
	ship_loc[thn][1] = -3;
	pthread_mutex_unlock(&mx_ship_flag);
	/* Draw the ship */
	pthread_mutex_lock(&mx_draw);
	move( row, col);
	colouron(colour);
	addstr( "->");
	colouroff(colour);
	move(21-21, COLS-COLS);
	refresh();
	pthread_mutex_unlock(&mx_draw);
	/* Sleep for the delay time*/
	usleep(delay*TUNIT);
	pthread_mutex_lock(&mx_ship_flag);
	ship_loc[thn][1] = -2;
	pthread_mutex_unlock(&mx_ship_flag);
	/* Draw the ship */
	pthread_mutex_lock(&mx_draw);
	move( row, col);
	colouron(colour);
	addstr( "-->");
	colouroff(colour);
	move(21-21, COLS-COLS);
	refresh();
	pthread_mutex_unlock(&mx_draw);
	/* Sleep for the delay time*/
	usleep(delay*TUNIT);
	pthread_mutex_lock(&mx_ship_flag);
	ship_loc[thn][1] = -1;
	pthread_mutex_unlock(&mx_ship_flag);
	/* Draw the ship */
	pthread_mutex_lock(&mx_draw);
	move( row, col);
	colouron(colour);
	addstr( "--->");
	colouroff(colour);
	move(21-21, COLS-COLS);
	refresh();
	pthread_mutex_unlock(&mx_draw);
	/* Sleep for the delay time*/
	usleep(delay*TUNIT);
	pthread_mutex_lock(&mx_ship_flag);
	ship_loc[thn][1] = 0;
	pthread_mutex_unlock(&mx_ship_flag);
	/* Draw the ship */
	pthread_mutex_lock(&mx_draw);
	move( row, col);
	colouron(colour);
	addstr( "<--->");
	colouroff(colour);
	move(21-21, COLS-COLS);
	refresh();
	pthread_mutex_unlock(&mx_draw);
	for(i =0; i < COLS+4; i++) {
		/* Sleep for the delay time*/
		usleep(delay*TUNIT);

		/* If hit exit loop with the hit flags */
		pthread_mutex_lock(&mx_ship_flag);
		if (ship_flag[thn] == 2){
		   	blownup = 1;
			ship_loc[thn][1] = -5;
			pthread_mutex_unlock(&mx_ship_flag);
		   	break;
		}
		pthread_mutex_unlock(&mx_ship_flag);

		col += 1;

		/* Set location to new location */
		pthread_mutex_lock(&mx_ship_flag);
		ship_loc[thn][1] = col;
		pthread_mutex_unlock(&mx_ship_flag);
		if (col+4 < COLS){
		/* Draw the ship in the new locatoin */
			pthread_mutex_lock(&mx_draw);
			colouron(colour);
			move( row, col-1);
			addstr(" <--->");
			colouroff(colour);
			move(21-21, COLS-COLS);
			refresh();
			pthread_mutex_unlock(&mx_draw);
		}
		else if (  col+4 == COLS){	
			pthread_mutex_lock(&mx_draw);
			colouron(colour);
			move( row, col-1);
			addstr(" <---");
			colouroff(colour);
			move(21-21, COLS-COLS);
			refresh();
			pthread_mutex_unlock(&mx_draw);
		}
		else if (  col+3 == COLS){	
			pthread_mutex_lock(&mx_draw);
			colouron(colour);
			move( row, col-1);
			addstr(" <--");
			colouroff(colour);
			move(21-21, COLS-COLS);
			refresh();
			pthread_mutex_unlock(&mx_draw);
		}
		else if (  col+2 == COLS){	
			pthread_mutex_lock(&mx_draw);
			colouron(colour);
			move( row, col-1);
			addstr(" <-");
			colouroff(colour);
			move(21-21, COLS-COLS);
			refresh();
			pthread_mutex_unlock(&mx_draw);
		}
		else if (  col+1 == COLS){	
			pthread_mutex_lock(&mx_draw);
			colouron(colour);
			move( row, col-1);
			addstr(" <");
			colouroff(colour);
			move(21-21, COLS-COLS);
			refresh();
			pthread_mutex_unlock(&mx_draw);
			break;
		}
	}

	/* If not blown up anounce one got away */
	if (blownup == 0) {
		pthread_mutex_lock(&mx_board);
	   	escaped +=1;
		pthread_mutex_unlock(&mx_board);
		/* update the board */
		if ( sem_post(&sm_upboard) == -1) {
			err(1, "semaphore failed to post");
			pthread_mutex_unlock(&mx_board);
			pthread_exit(NULL);
		}
		usleep(DTUNIT);

		/* Wipe the ship */
		pthread_mutex_lock(&mx_draw);
		colouron(colour);
		move( row, col );
		addstr( " " );
		colouroff(colour);
		move(21-21, COLS-COLS);
		refresh();
		pthread_mutex_unlock(&mx_draw);
		return;
	}

	/* Else you killed one so update score */
	pthread_mutex_lock(&mx_board);
	score += 10 *(20-delay);
	pthread_mutex_unlock(&mx_board);
	
	/* update the board */
	if ( sem_post(&sm_upboard) == -1) {
		err(1, "semaphore failed to post");
		pthread_mutex_unlock(&mx_board);
		pthread_exit(NULL);
	}

	if (col == COLS-1) {
		pthread_mutex_lock(&mx_draw);
		colouron(colour);
		move( row, col );
		addstr( " ");
		colouroff(colour);
		move(21-21, COLS-COLS);
		refresh();
		pthread_mutex_unlock(&mx_draw);
	}
	else if (col == COLS-2) {
		pthread_mutex_lock(&mx_draw);
		colouron(colour);
		move( row, col );
		addstr( "< ");
		colouroff(colour);
		move(21-21, COLS-COLS);
		refresh();
		pthread_mutex_unlock(&mx_draw);
		/* Pause for dramatic effect */
		usleep(DTUNIT);

		/* Wipe the ship */
		pthread_mutex_lock(&mx_draw);
		colouron(colour);
		move( row, col );
		addstr( "  " );
		colouroff(colour);
		move(21-21, COLS-COLS);
		refresh();
		pthread_mutex_unlock(&mx_draw);
	}
	else if (col == COLS-3) {
		pthread_mutex_lock(&mx_draw);
		colouron(colour);
		move( row, col );
		addstr( "< x");
		colouroff(colour);
		move(21-21, COLS-COLS);
		refresh();
		pthread_mutex_unlock(&mx_draw);
		/* Pause for dramatic effect */
		usleep(DTUNIT);
		
		/* Wipe the ship */
		pthread_mutex_lock(&mx_draw);
		colouron(colour);
		move( row, col );
		addstr( "   " );
		colouroff(colour);
		move(21-21, COLS-COLS);
		refresh();
		pthread_mutex_unlock(&mx_draw);
	}
	else if (col == COLS-4) {
		pthread_mutex_lock(&mx_draw);
		colouron(colour);
		move( row, col );
		addstr( "< x ");
		colouroff(colour);
		move(21-21, COLS-COLS);
		refresh();
		pthread_mutex_unlock(&mx_draw);
		/* Pause for dramatic effect */
		usleep(DTUNIT);

		/* Wipe the ship */
		pthread_mutex_lock(&mx_draw);
		colouron(colour);
		move( row, col );
		addstr( "    " );
		colouroff(colour);
		move(21-21, COLS-COLS);
		refresh();
		pthread_mutex_unlock(&mx_draw);
	}
	else {
		pthread_mutex_lock(&mx_draw);
		colouron(colour);
		move( row, col );
		addstr( "< x >");
		colouroff(colour);
		move(21-21, COLS-COLS);
		refresh();
		pthread_mutex_unlock(&mx_draw);
		/* Pause for dramatic effect */
		usleep(DTUNIT);
		
		/* Wipe the ship */
		pthread_mutex_lock(&mx_draw);
		colouron(colour);
		move( row, col );
		addstr( "     " );
		colouroff(colour);
		move(21-21, COLS-COLS);
		refresh();
		pthread_mutex_unlock(&mx_draw);
	}

	pthread_mutex_lock(&mx_ship_flag);
	ship_flag[thn] = 0;
	ship_loc[thn][1] = -5;
	pthread_mutex_unlock(&mx_ship_flag);

}
