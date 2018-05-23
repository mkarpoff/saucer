#include "erocket.h"

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
 
void *
erocket_launcher(void *arg) {
	int i;
	int j;
	int st;
	int br = 0;

	pthread_mutex_lock(&mx_r);
	ready += 1;
	pthread_mutex_unlock(&mx_r);
	/* wait for the user to exit the loading screen */
	while(load == 1)
		pthread_yield();

	/* The sleep allows the user to get a second to prepar themselves */
	sleep(3);
	while(1){
		for (i=0; i < MAXSHIP; i++){
			pthread_mutex_lock(&mx_ship_flag);
		 	if (ship_flag[i] == 1 && ship_loc[i][1] > -3){
		 		pthread_mutex_unlock(&mx_ship_flag);
					/* Look for a non active ship*/
				for(j=0; j < MAXEROCK; j++){
					if (erocket_flag[j] == 0){
				 		pthread_mutex_lock(&mx_ship_flag);
				 		erocket_loc[j][1] = ship_loc[i][1] + 2;
				 		erocket_loc[j][0] = ship_loc[i][0] + 1;
				 		pthread_mutex_unlock(&mx_ship_flag);
				 		erocket_flag[j] = 1;
				 		if ( sem_post(&sm_erock[j])) {
							err(1, "Sem post failed");
							pthread_exit(NULL);
						}
						br = 1;
						break;
				 	}
				}
				
			}
			else
				pthread_mutex_unlock(&mx_ship_flag);
			if (br){
				br = 0;
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

void *
erocket_thread(void *arg)
{
	struct thd_prop *rs = arg;
	int thn = rs->id;
	
	pthread_mutex_lock(&mx_r);
	ready += 1;
	pthread_mutex_unlock(&mx_r);

	while(1) {
		if (sem_wait(&sm_erock[thn])) {
			err(1, "Sem wait failed");
			pthread_exit(NULL);
		}
		animate_erocket(thn, erocket_loc[thn][1], erocket_loc[thn][0]);
		erocket_flag[thn] = 0;

	}
	
}
/*
 * This is the thread that is executed when a rocket is launched
 */
void 
animate_erocket(int thn, int col, int row) 
{
	int i;
	char c;
	bool dead = FALSE;

	pthread_mutex_lock(&mx_draw);
	move( row, col );
	colouron(20);
	addch('v');
	colouroff(20);
	move(21-21, COLS-COLS);
	refresh();
	pthread_mutex_unlock(&mx_draw);

	row +=1;

	while( 1 ) {
		usleep(ERTUNIT);

		/* Draw the rocket 1 row up on screen row -=1 */
		pthread_mutex_lock(&mx_draw);
		move( row - 1, col );
		c = inch();
		if (c == 'v')
			addch(' ');
		move( row, col );
		colouron(20);
		addch('v');
		colouroff(20);
		move(21-21, COLS-COLS);
		refresh();
		pthread_mutex_unlock(&mx_draw);
		pthread_mutex_lock(&mx_tur);
		if (row == 19 && (
		   col == tur_col+1 ||
		   col == tur_col-1 ||
		   col == tur_col)) {
		   	pthread_mutex_unlock(&mx_tur);
		   	k = 1;
			if(sem_post(&sm_quit)){
				err(1, "Sem post failed");
				pthread_exit(NULL);
			}
		}
		else
			pthread_mutex_unlock(&mx_tur);

		/* move the rocket's spot for next iteration */
		/* check if the rocket is going of the screen */
		if ( row == 19) {
			usleep(ERTUNIT);
			break;
		}
		row += 1;
	}

	/* clear the space*/
	pthread_mutex_lock(&mx_draw);
	move( row, col);
	addch(' ');
	move(21-21, COLS-COLS);
	refresh();
	pthread_mutex_unlock(&mx_draw);
}