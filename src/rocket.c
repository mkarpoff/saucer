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

#include "rocket.h"
/*
 * This function animates the turret and listens for the users key presses
 */
void *
animate_turret(void *arg)
{
	int c;
	int i;
	int col = COLS / 2;

	pthread_mutex_lock(&mx_tur);
	tur_col = col;
	pthread_mutex_unlock(&mx_tur);

   	pthread_mutex_lock(&mx_r);
	ready += 1;
	pthread_mutex_unlock(&mx_r);

   	/* Wait while the player is in the loading screen */
	while(load == 1)
		pthread_yield();

	/* Draw the turret in the initial location */
	pthread_mutex_lock(&mx_draw);
	move( 21-2, col-1);
	addch('/');
	addch('|');
	addch('\\');
	move( 21-21, COLS-COLS);
	refresh();
	pthread_mutex_unlock(&mx_draw);



	while(1) {
		/* Get user's command */

		pthread_mutex_lock(&mx_draw);
		c = getch();
		pthread_mutex_unlock(&mx_draw);


		if ( c == ERR)
			usleep(1000);
		/* User wants to quit */
		else if ( c == 'Q' ) {
			if ( sem_post(&sm_quit) == -1) {
				err(1, "semaphore failed to post");
				pthread_exit(NULL);
			}
			break;
		}

		/* The user uses the move left command */
		else if ( c == move_left ) {
			if (col > 1) {
				col -= 1;

				pthread_mutex_lock(&mx_draw);
				move( 21-2, col-1 );
				addch('/');
				addch('|');
				addch('\\');
				addch(' ');
				move(21-21, COLS-COLS);
				refresh();
				pthread_mutex_unlock(&mx_draw);
			}
			pthread_mutex_lock(&mx_tur);
			tur_col = col;
			pthread_mutex_unlock(&mx_tur);
		}

		/* The user uses the move right command */
		else if ( c == move_right ) {
			if (col < COLS-2) {
				col += 1;
	
				pthread_mutex_lock(&mx_draw);
				move( 21-2, col-2 );   
				addch(' ');
				addch('/');
				addch('|');
				addch('\\');
				move(21-21, COLS-COLS);
				refresh();
				pthread_mutex_unlock(&mx_draw);
			}
			pthread_mutex_lock(&mx_tur);
			tur_col = col;
			pthread_mutex_unlock(&mx_tur);
		}

		/* User uses the jump to start command */
		else if ( c == move_start ) {
		
			pthread_mutex_lock(&mx_draw);
			move( 21-2, col -1);
			addstr("   ");
			move(21-21, COLS-COLS);
			refresh();
			pthread_mutex_unlock(&mx_draw);

			col = 1;
			pthread_mutex_lock(&mx_draw);
			move( 21-2, col -1);
			addch('/');
			addch('|');
			addch('\\');
			move(21-21, COLS-COLS);
			refresh();
			pthread_mutex_unlock(&mx_draw);

			pthread_mutex_lock(&mx_tur);
			tur_col = col;
			pthread_mutex_unlock(&mx_tur);
		}

		/* User uses the jump to end command */
		else if ( c == move_end ) {
		
			pthread_mutex_lock(&mx_draw);
			move( 21-2, col -1);   
			addstr("   ");
			move(21-21, COLS-COLS);
			refresh();
			pthread_mutex_unlock(&mx_draw);

			col = COLS-2;
			pthread_mutex_lock(&mx_draw);
			move( 21-2, col );
			move( 21-2, col -1);
			addch('/');
			addch('|');
			addch('\\');
			move(21-21, COLS-COLS);
			refresh();
			pthread_mutex_unlock(&mx_draw);

			pthread_mutex_lock(&mx_tur);
			tur_col = col;
			pthread_mutex_unlock(&mx_tur);
		}

		/* User tries to fire a rocket */
		else if ( c == ' ' && rockets >= 1) {
			/* Look for an unused rocket */
			for(i=0; i < MAXROCK; i++) {
				if ( rocket_flag[i] == 0 ) {
				   	/* 
				   	 * Activates a rocket with the column
				   	 * value as that of the turret.
				   	 */
				   	
					rocket_loc[i] = col;
					rocket_flag[i] = 1;
					if ( sem_post( &sm_rock[i]) == -1) {
						err(1,"Sem post failed");
						pthread_exit(NULL);
					}

					pthread_mutex_lock(&mx_rockets);
					rockets -= 1;
					pthread_mutex_unlock(&mx_rockets);

					if ( sem_post(&sm_upboard) == -1){
						err(1, "sem post failed");
						pthread_exit(NULL);
 					}
					break;
				}
			}
		}
	}
	pthread_exit(NULL);
}

/*
 * This function is a listener that checks for a case were the user has no 
 * rockets left but there are rockets still flying
 */
void *
rocket_listenter(void *arg)
{

	int exist_flag = 0;
	int i;
	pthread_mutex_lock(&mx_r);
	ready += 1;
	pthread_mutex_unlock(&mx_r);
	while(1) {
		pthread_yield();
		while (rockets == 0) {
			for(i=0; i < MAXROCK; i++){
				if ( rocket_flag[i] == 1 ) {
					exist_flag = 0;
					break;
				}
			}
			if (exist_flag == 1){
				if ( sem_post(&sm_quit) == -1){
					err(1, "semaphore failed to post");
					pthread_exit(NULL);
				}
			}
			exist_flag = 1;
		}
	}
}

void *
rocket_thread(void *arg)
{
	struct thd_prop *rs = arg;
	int thn = rs->id;
	
	pthread_mutex_lock(&mx_r);
	ready += 1;
	pthread_mutex_unlock(&mx_r);

	while(1) {
		if (sem_wait(&sm_rock[thn])) {
			err(1, "Sem wait failed");
			pthread_exit(NULL);
		}
		animate_rocket(thn, rocket_loc[thn]);
		rocket_flag[thn] = 0;
	}
	
}

/*
 * This is the thread that is executed when a rocket is launched
 */
void 
animate_rocket(int thn, int col) 
{
	int row = 21-3;
	int i;
	char c;
	bool dead = FALSE;

	pthread_mutex_lock(&mx_draw);
	move( row, col );
	colouron(20);
	addch('^');
	colouroff(20);
	move(21-21, COLS-COLS);
	refresh();
	pthread_mutex_unlock(&mx_draw);

	row -=1;

	while( 1 ) {
		usleep(RTUNIT);

		/* Draw the rocket 1 row up on screen row -=1 */
		pthread_mutex_lock(&mx_draw);
		move( row + 1, col );
		c = inch();
		if (c == '^')
			addch(' ');
		move( row, col );
		colouron(20);
		addch('^');
		colouroff(20);
		move(21-21, COLS-COLS);
		refresh();
		pthread_mutex_unlock(&mx_draw);

		/* Check if there is a ship where the rocket is */
		for(i=0;i < MAXSHIP; i++){

			pthread_mutex_lock(&mx_ship_flag);
			if (( ship_loc[i][1] == col||
			   	ship_loc[i][1]+1== col||
			   	ship_loc[i][1]+2== col||
			   	ship_loc[i][1]+3== col||
			   	ship_loc[i][1]+4== col)&&
			   	ship_loc[i][0] == row && ship_flag[i] == 1) {
			   	
			   	ship_loc[i][0] = 0;
			   	ship_loc[i][1] = 0;
			   	ship_flag[i] = 2;
			   	dead = TRUE;
			   	pthread_mutex_unlock(&mx_ship_flag);

				pthread_mutex_lock(&mx_rockets);
				rockets += ROCKINC;
				pthread_mutex_unlock(&mx_rockets);
			}
			else
				pthread_mutex_unlock(&mx_ship_flag);
		}
		/* check if the rocket hit somethig */
		if (dead)
			break;
		/* move the rocket's spot for next iteration */
		/* check if the rocket is going of the screen */
		if ( row == 0) {
			usleep(RTUNIT);
			break;
		}
		row -= 1;
	}

	/* clear the space*/
	pthread_mutex_lock(&mx_draw);
	move( row, col);
	addch(' ');
	move(21-21, COLS-COLS);
	refresh();
	pthread_mutex_unlock(&mx_draw);
}

