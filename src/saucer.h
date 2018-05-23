
#ifndef SAUCER_H
#define SAUCER_H

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

#include <stdio.h>
#include <semaphore.h>
#include <curses.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "settings.h"
#include "loadscr.h"
#include "ship.h"
#include "rocket.h"
#include "erocket.h"

#define COL_RED 0
#define COL_GRE 1
#define COL_YEL 2
#define COL_BLU 3
#define COL_MAG 4
#define COL_CYA 5
#define COL_WHI 6
#define FLASHON	10
#define FLASHOFF 11

struct thd_prop {
	int id;
};


pthread_mutex_t mx_r;
pthread_mutex_t mx_draw; 
pthread_mutex_t mx_board;
pthread_mutex_t mx_rockets;
pthread_mutex_t mx_ship_flag;
pthread_mutex_t mx_rocket_flag;
pthread_mutex_t mx_tur;

sem_t sm_upboard;
sem_t sm_quit;
sem_t sm_hs;
sem_t sm_rock[MAXROCK];
sem_t sm_erock[MAXEROCK];
sem_t sm_ship[MAXSHIP];

int escaped;			/* Counter for how many ships have escaped*/
int rockets;			/* Counter for rockets left */
int score;			/* Score... */
int ship_flag[MAXSHIP];		/* Flag for if the thread is active */
int ship_loc[MAXSHIP][2];	/* Location of ship */
int rocket_flag[MAXROCK];	/* Flag for if the thread is active */
int erocket_flag[MAXEROCK];	/* Flag for if the thread is active */
int mtime;			/* Time count in sec */
int move_left;			/* Movement key */
int move_right;			/* Movement key */
int move_start;			/* Movement key */
int move_end;			/* Movement key */
int load;			/* Used to cause threads to pause after load */
int tur_col;			/* Turret's location */
int k;

int	 main();

void	 setup();

void	 colouron(int cin);

void	 colouroff(int cin);

void	*animate_board(void *arg);

void	*animate_clock(void *arg);

char	*rocket_bar_filler(char * rocketbar);

#endif
