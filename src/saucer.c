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

#include "saucer.h"


/*
 * The main function
 */
int
main()
{
	int i;
	pthread_t thrd_turret;
	pthread_t thrd_board;
	pthread_t thrd_shiplauncher;
	pthread_t thrd_mtime;
	pthread_t thrd_loadscr;
	pthread_t thrd_endscr;
	pthread_t thrd_rock_list;
	pthread_t thrd_erocklauncher;

	pthread_t thrds_rock[MAXROCK];
	pthread_t thrds_ship[MAXSHIP];
	pthread_t thrds_erock[MAXEROCK];
	
	struct thd_prop rock_props[MAXROCK];
	struct thd_prop ship_props[MAXSHIP];
	struct thd_prop erock_props[MAXEROCK];

	setup();


	/*
	 * Start all the threads. Threads will run their own setup then wait for
	 * the user to enter the game mode and leave the pre-game loading screen
	 */

	if ( pthread_create(&thrd_loadscr, NULL, loadscr, NULL)){
		err(1,"error creating thread");
		endwin();
		exit(0);
	}
	if ( pthread_create(&thrd_board, NULL, animate_board, NULL)){
		err(1,"error creating thread");
		endwin();
		exit(0);
	}
	if ( pthread_create(&thrd_mtime, NULL, animate_clock, NULL)){
		err(1,"error creating thread");
		endwin();
		exit(0);
	}
	if ( pthread_create(&thrd_shiplauncher, NULL, ship_launcher, NULL)){
		err(1,"error creating thread");
		endwin();
		exit(0);
	}
	if ( pthread_create(&thrd_turret, NULL, animate_turret, NULL)){
		err(1,"error creating thread");
		endwin();
		exit(0);
	}
	if (pthread_create(&thrd_rock_list, NULL, rocket_listenter,
	    NULL) != 0 ){
		err(1, "failed to create a new thread");
		pthread_exit(NULL);
	}
	if ( pthread_create(&thrd_erocklauncher, NULL, erocket_launcher, NULL)){
		err(1,"error creating thread");
		endwin();
		exit(0);
	}

	/* Initialize the ship id's and threads */
	for(i=0; i < MAXSHIP; i++){
		ship_props[i].id = i;
		if ( pthread_create(&thrds_ship[i], NULL, ship_thread,
		    &ship_props[i]) != 0){
			err(1,"error creating thread");
			endwin();
			pthread_exit(NULL);
		}
	}

	for( i=0; i < MAXROCK;i++){
		ship_props[i].id = i;
		rocket_loc[i] = -1;
		rocket_flag[i] = 0;
		if (pthread_create(&thrds_rock[i], NULL, rocket_thread,
		    &ship_props[i]) != 0) {
			err(1, "failed to create a new "
			"thread");
			exit(0);
		}
	}
	for( i=0; i < MAXEROCK;i++){
		erock_props[i].id = i;
		erocket_loc[i][1] = -1;
		erocket_loc[i][0] = -1;
		erocket_flag[i] = 0;
		if (pthread_create(&thrds_erock[i], NULL, erocket_thread,
		    &erock_props[i]) != 0) {
			err(1, "failed to create a new "
			"thread");
			exit(0);
		}
	}


	pthread_mutex_lock(&mx_r);
	ready += 1;
	pthread_mutex_unlock(&mx_r);

	/* This thread just waits for any form of end game condition */
	if ( sem_wait(&sm_quit)){
		err(1, "Sem wait failed");
		endwin();
		exit(0);
	}

	pthread_mutex_lock(&mx_draw);
	pthread_mutex_lock(&mx_board);


	/* While the other threads are cleaning up show the end game screen */	
	if ( pthread_create(&thrd_endscr, NULL, endscr, NULL)){
		err(1,"error creating thread");
		endwin();
		exit(0);
	}

	/* cancel all the threads */
	pthread_cancel(thrd_mtime);
	pthread_cancel(thrd_shiplauncher);
	pthread_cancel(thrd_turret);
	pthread_cancel(thrd_board);
	pthread_cancel(thrd_rock_list);
	pthread_cancel(thrd_erocklauncher);
	for(i=0; i < MAXSHIP; i++) {
		pthread_cancel(thrds_ship[i]);
	}
	for(i=0; i < MAXROCK; i++) {
		pthread_cancel(thrds_rock[i]);
	}
	for(i=0; i < MAXEROCK; i++) {
		pthread_cancel(thrds_erock[i]);
	}
	
	/* Wait for the user to finish with the end game screen */
	if ( sem_wait(&sm_hs) ){
		err(1, "Sem wait failed");
		endwin();
		exit(0);
	}

	endwin();
	return 0;
}


/*
 * This function sets all the global variables to their initial values, It also
 * initializes the semaphores and checks if the screen greater than equal to 
 * the minimum function screen size.
 */
void 
setup()
{
	int i;

	/* So we get a somewhat random variables we use the pid as a seed */
	srand(getpid());
	
	 /* Global initializers for mutexs */
	pthread_mutex_init(&mx_r, NULL);
	pthread_mutex_init(&mx_draw, NULL);
	pthread_mutex_init(&mx_board, NULL);
	pthread_mutex_init(&mx_rockets, NULL);
	pthread_mutex_init(&mx_ship_flag, NULL);
	pthread_mutex_init(&mx_rocket_flag, NULL);
	pthread_mutex_init(&mx_tur, NULL);

	ready = 0;
	escaped = 0;
	score = 0;
	rockets = STROCK;
	load = 1;
	k = 0;


	/* initialize the semaphores */
	if( sem_init(&sm_upboard, 0, 0) == -1) {
		err(1, "Failure to initialize semaphore");
		exit(0);
	}

	if( sem_init(&sm_quit, 0, 0) == -1) {
		err(1, "Failure to initialize semaphore");
		exit(0);
	}

	if( sem_init(&sm_hs, 0, 0) == -1) {
		err(1, "Failure to initialize semaphore");
		exit(0);
	}
	for ( i =0; i < MAXROCK; i++){
		if( sem_init(&sm_rock[i], 0, 0) == -1) {
			err(1, "Failure to initialize semaphore");
			exit(0);
		}
	
	}
	for( i=0; i < MAXSHIP; i++) {
		if ( sem_init(&sm_ship[i], 0, 0) == -1) {
			err(1, "Failure to initialize semaphore");
			exit(0);
		}
	}
	for ( i =0; i < MAXEROCK; i++){
		if( sem_init(&sm_erock[i], 0, 0) == -1) {
			err(1, "Failure to initialize semaphore");
			exit(0);
		}
	
	}
	/* set up curses */
	initscr();

	/* 
	 * If compiled with colour mode then these variables are need to allow 
	 * there to be colours. Otherwise this code is useless and isn't 
	 * compiled.
	 */
#ifdef COLOUR
	start_color();

	init_pair(FLASHON, COLOR_BLACK, COLOR_WHITE);
	init_pair(FLASHOFF, COLOR_WHITE, COLOR_BLACK);
	
	init_pair(20, COLOR_RED, COLOR_BLACK);
	init_pair(COL_RED , COLOR_RED, COLOR_BLACK);
	init_pair(COL_GRE, COLOR_GREEN, COLOR_BLACK);
	init_pair(COL_YEL, COLOR_YELLOW, COLOR_BLACK);
	init_pair(COL_BLU, COLOR_BLUE, COLOR_BLACK);
	init_pair(COL_MAG, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(COL_CYA, COLOR_CYAN, COLOR_BLACK);
	init_pair(COL_WHI, COLOR_WHITE, COLOR_BLACK);
#endif
	/* 
	 * If compiled with arrow mode then these variables are need to allow 
	 * there to be arrow keys used. Otherwise this code is useless and isn't 
	 * compiled. It also checks if the system supports arrow keys.
	 */

#ifdef ARROWS
	keypad(stdscr, TRUE);
	move_left = KEY_LEFT;
	move_right = KEY_RIGHT;
	move_start = KEY_UP;
	move_end = KEY_DOWN;

#endif

#ifndef ARROWS
	move_left = ',';
	move_right = '.';
	move_start = 'm';
	move_end = '/';
#endif

	noecho();
	nodelay(stdscr, TRUE);
	crmode();
	clear();
	return;
}


/* 
 * This function will activate colour mode if it is compiled with colour mode 
 * on. Non-colour version of the game comes alongside the game.
 */
void
colouron(int cin)
{
#ifdef COLOUR
	attron(COLOR_PAIR(cin));
#endif
	return;
}

/* 
 * This function will deactivate colour mode if it is compiled with colour mode 
 * on. Non-colour version of the game comes alongside the game.
 */

void
colouroff(int cin)
{
#ifdef COLOUR
	attroff(COLOR_PAIR(cin));
#endif
	return;
}

/*
 * This is the code that updates the rocket count, escaped ship count and score
 */
void *
animate_board(void *arg)
{
	char *msg;

	pthread_mutex_lock(&mx_r);
	ready += 1;
	pthread_mutex_unlock(&mx_r);

	/* Wait while the player is in the loading screen */
	while(load == 1)
		pthread_yield();


	while(1) {
		/* Check if more ships escaped than are allowed to */

		if (escaped >= ESCLIM) {
			if ( sem_post(&sm_quit) == -1){
				err(1, "semaphore failed to post");
				pthread_exit(NULL);
			}
		}
		
		/* Build and print the message on the bottom of the screen */
		if (asprintf(&msg ,
			"Rockets %03d: Escaped %02d: Score %06d: ", 
			rockets ,escaped, score) == -1) {
			err(1, "Memory allocation failure");
			pthread_exit(NULL);
		}

		pthread_mutex_lock(&mx_draw);
		move(21-1, 0);
		addstr(msg);
		addch(' ');
		move(21-21, COLS-COLS);
		refresh();
		pthread_mutex_unlock(&mx_draw);

		free(msg);
		/* Wait for one of the variables to be changed */
		if ( sem_wait(&sm_upboard) == -1) {
			err(1, "semaphore failed to wait");
			pthread_exit(NULL);
		}
	}
}

/* 
 * This clock updates every second and draws a new bar on the bonus bar
 */
void *
animate_clock(void *arg)
{
	/* Initialize time to be 1 */
	mtime = 1;
	char *rocketbar;
	
	pthread_mutex_lock(&mx_r);
	ready += 1;
	pthread_mutex_unlock(&mx_r);
	/* Wait while the player is in the loading screen */
	while(load == 1)
		pthread_yield();

	while(1) {
		rocketbar = rocket_bar_filler(rocketbar);

		pthread_mutex_lock(&mx_draw);
		move(21-1, 40);
		addch('{');
		colouron(COL_CYA);
		addstr(rocketbar);
		colouroff(COL_CYA);
		addch('}');
		move(21-21, COLS-COLS);
		refresh();
		pthread_mutex_unlock(&mx_draw);

		free(rocketbar);

		usleep(990000);
		mtime += 1;
	}
	pthread_exit(NULL);
}

/*
 * a simple function to make drawing the bonus bar easy
 */
char *
rocket_bar_filler(char * rocketbar)
{
	switch (mtime %10) {
		case 0:
			if (asprintf(&rocketbar ,"%s%s", (""),
			    ("         ")) == -1) {
				err(1, "Memory allocation failure");
				pthread_exit(NULL);
			}
			
			pthread_mutex_lock(&mx_rockets);
			   rockets += 1;
			pthread_mutex_unlock(&mx_rockets);

			if ( sem_post(&sm_upboard) == -1){
			   	err(1, "semaphore failed to post");
			   	pthread_mutex_unlock(&mx_board);
			   	pthread_exit(NULL);
			}
			break;

		case 1:
			if (asprintf(&rocketbar ,"%s%s", ("="),
			    ("        ")) == -1) {
				err(1, "Memory allocation failure");
				pthread_exit(NULL);
			}
			break;

		case 2:
			if (asprintf(&rocketbar ,"%s%s", ("=="),
			    ("       ")) == -1) {
				err(1, "Memory allocation failure");
				pthread_exit(NULL);
			}
			break;

		case 3:
			if (asprintf(&rocketbar ,"%s%s", ("==="),
			    ("      ")) == -1) {
				err(1, "Memory allocation failure");
				pthread_exit(NULL);
			}
			break;

		case 4:
			if (asprintf(&rocketbar ,"%s%s", ("===="),
			    ("     ")) == -1) {
				err(1, "Memory allocation failure");
				pthread_exit(NULL);
			}
			break;

		case 5:
			if (asprintf(&rocketbar ,"%s%s", ("====="),
			    ("    ")) == -1) {
				err(1, "Memory allocation failure");
				pthread_exit(NULL);
			}
			break;

		case 6:
			if (asprintf(&rocketbar ,"%s%s", ("======"),
			    ("   ")) == -1) {
				err(1, "Memory allocation failure");
				pthread_exit(NULL);
			}
			break; 

		case 7:
			if (asprintf(&rocketbar ,"%s%s", ("======="),
			    ("  ")) == -1) {
				err(1, "Memory allocation failure");
				pthread_exit(NULL);
			}break;

		case 8:
			if (asprintf(&rocketbar ,"%s%s", ("========"),
			    (" ")) == -1) {
				err(1, "Memory allocation failure");
				pthread_exit(NULL);
			}
			break;

		case 9:
			if (asprintf(&rocketbar ,"%s%s", ("========="),
			    ("")) == -1) {
				err(1, "Memory allocation failure");
				pthread_exit(NULL);
			}
			break;

		default:
			break;
	}
	return rocketbar;
}
