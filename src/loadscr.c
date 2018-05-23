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
 * This site helped me to generate the asci art for the main screen
 * http://patorjk.com/software/taag/
 */

#include "loadscr.h"
/*
 * This displays the loading screens
 */
void *
loadscr(void *arg)
{

	int c;
	int st;
	char *msg1;
	char *left_char;
	char *right_char;
	char *start_char;
	char *end_char;

	st = 0;
	if (COLS > 70)
		st = COLS/2-35;
	pthread_mutex_lock(&mx_draw);
	colouron(COL_CYA);

	/* This pile of junk looking stuff is the good looking opening screen */
	move(0, st);
	addstr("       ___          ___          ___          ___          ___ "
		"");
	move(1, st);
	addstr("      /\\  \\        /\\  \\        /\\  \\        /\\  \\     "
	    "   /\\  \\");
	move(2, st);
	addstr("     /::\\  \\      /::\\  \\      /::\\  \\      /::\\  \\    "
	    "  /::\\  \\");
	move(3, st);
	addstr("    /:/\\ \\  \\    /:/\\:\\  \\    /:/\\:\\  \\    /:/\\:\\  "
	    "\\    /:/\\:\\  \\");
	move(4, st);
	addstr("   _\\:\\~\\ \\  \\  /::\\~\\:\\  \\  /::\\~\\:\\  \\  /:/  \\:"
	    "\\  \\  /::\\~\\:\\  \\");
	move(5, st);
	addstr("  /\\ \\:\\ \\ \\__\\/:/\\:\\ \\:\\__\\/:/\\:\\ \\:\\__\\/:/__/"
	    " \\:\\__\\/:/\\:\\ \\:\\__\\");
	move(6, st);
	addstr("  \\:\\ \\:\\ \\/__/\\/__\\:\\/:/  /\\/__\\:\\/:/  /\\:\\  \\  "
	    "\\/__/\\:\\~\\:\\ \\/__/");
	move(7, st);
	addstr("   \\:\\ \\:\\__\\       \\::/  /      \\::/  /  \\:\\  \\     "
	    "  \\:\\ \\:\\__\\");
	move(8, st);
	addstr("    \\:\\/:/  /        \\/__/       /:/  /    \\:\\  \\       "
	    "\\:\\ \\/__/");
	move(9, st);
	addstr("     \\::/  /                    /:/  /      \\:\\__\\       \\"
	    ":\\__\\");
	move(10, st);
	addstr("      \\/__/    ___      ___     \\/__/___     \\/__/___     \\"
	    "/__/___");
	move(11, st);
	addstr("     /\\  \\    /\\  \\    /\\  \\        /\\  \\        /\\  "
	    "\\        /\\__\\");
	move(12, st);
	addstr("    /::\\  \\   \\:\\  \\   \\:\\  \\      /::\\  \\      /::\\"
	    "  \\      /:/  /");
	move(13, st);
	addstr("   /:/\\:\\  \\   \\:\\  \\   \\:\\  \\    /:/\\:\\  \\    /:/"
	    "\\:\\  \\    /:/__/");
	move(14, st);
	addstr("  /::\\~\\:\\  \\  /::\\  \\  /::\\  \\  /::\\~\\:\\  \\  /:/  "
	    "\\:\\  \\  /::\\__\\____");
	move(15, st);
	addstr(" /:/\\:\\ \\:\\__\\/:/\\:\\__\\/:/\\:\\__\\/:/\\:\\ \\:\\__\\/:"
	    "/__/ \\:\\__\\/:/\\:::::\\__\\");
	move(16, st);
	addstr(" \\/__\\:\\/:/  /:/  \\/__/:/  \\/__/\\_/_\\:\\/:/  /\\:\\  \\ "
	    " \\/__/\\/_|:|~~|~");
	move(17, st);
	addstr("      \\::/  /:/  /   /:/  /          \\::/  /  \\:\\  \\      "
	    "   |:|  |");
	move(18, st);
	addstr("      /:/  /\\/__/    \\/__/           /:/  /    \\:\\  \\     "
	    "   |:|  |");
	move(19, st);
	addstr("     /:/  /                         /:/  /      \\:\\__\\      "
	    " |:|  |");
	move(20, st);
	addstr("     \\/__/                          \\/__/        \\/__/      "
	    "  \\|__|");
	colouroff(COL_CYA);
	move(21-2, COLS/2 - 6);
	addstr("{          }");
	int last = (ready*9)/(MAXROCK+MAXSHIP+MAXEROCK+7)+1;
	move(21-2, COLS/2-5);
	colouron(COL_CYA);
	addch('=');
	while(ready < MAXSHIP+MAXROCK+MAXEROCK+7){
		pthread_yield();
		pthread_mutex_lock(&mx_r);
		if ((ready*9)/(MAXROCK+MAXSHIP+MAXEROCK+7)+1 > last){
			addch('=');
			last +=1;
		}
		pthread_mutex_unlock(&mx_r);
	}
	colouroff(COL_CYA);
		
	move(21-2, COLS/2 - 12);
	colouron(COL_YEL);
	addstr( "--Press ENTER to start!--");
	colouroff(COL_YEL);

	while(1){
		c = getch();
		if (c == '\n')
			break;
	}

	erase();

	/*
	 * This code check what form of movement keys the use will be using 
	 * and print the right instructions
	 */
	if ( move_left == ',') {
		if (asprintf(&left_char, "','") == -1) {
			err(1, "Memory allocation failure");
			pthread_exit(NULL);
		}
	}
	
	else {
		if (asprintf(&left_char, "LEFT ARROW") == -1) {
			err(1, "Memory allocation failure");
			pthread_exit(NULL);
		}
	}
	
	if ( move_right == '.') {
		if (asprintf(&right_char, "'.'") == -1) {
			err(1, "Memory allocation failure");
			pthread_exit(NULL);
		}
	}
	
	else {
		if (asprintf(&right_char, "RIGHT ARROW") == -1) {
			err(1, "Memory allocation failure");
			pthread_exit(NULL);
		}
	}

	if ( move_start == 'm') {
		if (asprintf(&start_char, "'m'") == -1) {
			err(1, "Memory allocation failure");
			pthread_exit(NULL);
		}
	}
	
	else {
		if (asprintf(&start_char, "UP ARROW") == -1) {
			err(1, "Memory allocation failure");
			pthread_exit(NULL);
		}
	}

	if ( move_end == ',') {
		if (asprintf(&end_char, "'/'") == -1) {
			err(1, "Memory allocation failure");
			pthread_exit(NULL);
		}
	}
	
	else {
		if (asprintf(&end_char, "DOWN ARROW") == -1) {
			err(1, "Memory allocation failure");
			pthread_exit(NULL);
		}
	}

	if (asprintf(&msg1, "Use %s to move left\n"
	    "Use %s to move right\n"
	    "Use %s to jump to far left\n"
	    "Use %s to jump to far right\n"
	    "Use SPACE to fire\n"
	    "Use 'Q' to Quit", 
	    left_char, right_char, start_char, end_char) == -1) {
		err(1, "Memory allocation failure");
		pthread_exit(NULL);
	}


	/* Print the instruction page */
	move(21/2 - 4, 0);
	addstr(msg1);
	colouron(COL_YEL);
	move(1, COLS/2 - 11);
	addstr("\\|||||||||||||||||||||/");
	move(2, COLS/2 - 12);
	addstr( "--Press ENTER to start!--");
	move(3, COLS/2 - 11);
	addstr("/|||||||||||||||||||||\\");
	colouroff(COL_YEL);
	move(21-5, 9);
	addstr("If you run out of rockets you lose and the world is destroyed");
	move(21-4, 9);
	addstr("|         When this reaches 10 you lose and the world is destroyed");
	move(21-3, 9);
	addstr("|           |        When this fills you get a bonus rocket");
	move(21-2, 9);
	addstr("V           V                      V");
	move(21-1, 0);
	addstr("Rockets 010: Escaped 03: Score 000130:  {====     }");
	move(21-1, 41);
	colouron(COL_CYA);
	addstr("====     ");
	colouroff(COL_CYA);
	move(21-21, COLS-COLS);
	refresh();

	free(left_char);
	free(right_char);
	free(start_char);
	free(end_char);
	free(msg1);
	

	while(1) {

		c = getch();
		if (c == '\n')
			break;
	}
	erase();
	pthread_mutex_unlock(&mx_draw);
	/* Let all the threads know the user is out of the loading screens */
	load = 0;

	pthread_exit(NULL);
}

/*
 * This displays the screen after the user loses the game
 */
void *
endscr(void *arg)
{
	char *msg;
	char c;
	int i;
	int s;
	FILE * sfp;
	char *(hsname[10]);
	int hsscore[10];
	char * smsg;



	erase();
	
	move( 2, COLS/2-25);
	addstr("YOU FAILED TO PREVENT THE DESTRUCTION OF THE WORLD!");
	if(k){
		move( 3, COLS/2-12);
		addstr("You got hit by a rocket!");
	}
	else if(rockets == 0){
		move( 3, COLS/2-12);
		addstr("You ran out of rockets!");
	}
	if(escaped == 10) {
		if (rockets == 0)
			move( 4, COLS/2-20);
		else
			move( 3, COLS/2-20);
		addstr("You let 10 ships get past your defenses!");
	}

	if (asprintf(&msg, "SCORE:  %6d", score) == -1) {
		err(1, "Memory allocation failure");
		endwin();
		exit(0);
	}
	move(6, COLS/2-7);
	colouron(COL_CYA);
	addstr(msg);
	move(5, COLS/2-18);
	free(msg);
	if (asprintf(&msg, "YOU SURVIVED FOR:  %3d:%02d", mtime/60,
	    mtime%60) == -1) {
		err(1, "Memory allocation failure");
		endwin();
		exit(0);
	}
	addstr(msg);
	free(msg);
	colouroff(COL_CYA);

	/* Load the high scores */
	sfp = fopen("scores.sinv", "r");
	for (i = 0; i < 10; i++){
		hsname[i] = malloc(40);
		if (hsname[i] == NULL){
			err(1, "Memory allocation failed");
			pthread_exit(NULL);
		}
	}
	if (sfp == NULL){
		move(21-13, COLS/2-14);
		addstr("No score file found making new!");
		for (i = 0; i < 10; i++){
			if (asprintf(&hsname[i], "Nobody yet") == -1) {
				err(1, "Memory allocation failure");
				endwin();
				exit(0);
			}
			hsscore[i] = 0;
		}
	}
	else {
		for (i = 0; i < 10; i++){
			if (fscanf(sfp, "%d||%[^||]||%d", &s, (hsname[i]), 
			    &(hsscore[i])) == -1){
				err(1, "File read failed");
				pthread_exit(NULL);
			}
		}
		fclose(sfp);
		
	}
	/* Check you beat any high scores */
	for (s = 0; s < 10; s++){
		if (score > hsscore[s]) 
			break;
	}
	
	if (s < 10){
		free(hsname[9]);
		for(i = 9;i > s; i--){
			hsscore[i] = hsscore[i-1];
			hsname[i] = hsname[i-1];
		}
		hsscore[s] = score;
		hsname[s] = malloc(40);
		if (hsname[s] == NULL){
			err(1, "Memory allocation failed");
			pthread_exit(NULL);
		} 

	}

	move(7, COLS/2-29);
	addstr("_______________________________________________________");
	move(8, COLS/2-30);
	addch('|');
	attron(A_UNDERLINE);
	addstr("RANK                NAME                        SCORE  ");
	attroff(A_UNDERLINE);
	addch('|');
	for (i = 0; i < 10; i++){
		if ( asprintf(&smsg, "|%2d : %-40s : %6d |", 1+i, hsname[i],
		    hsscore[i]) == -1) {
			err(1, "Memory allocation failure");
			pthread_exit(NULL);
		}
		move(9+i, COLS/2-30);
		addstr(smsg);
		free(smsg);
	}
	move(19, COLS/2-30);
	addstr("|_______________________________________________________|");
	if (s < 10){
		while(strlen(hsname[s]) < 1) {
			move(9+s, COLS/2 -24);
			echo();
			nodelay(stdscr, FALSE);
			getnstr(hsname[s], 40);
			noecho();
		}
		sfp = fopen("scores.sinv", "w");
		if (sfp == NULL){
			err(1, "File load failed");
			pthread_exit(NULL);
		}
		for( i = 0; i < 10; i++){
			fprintf(sfp, "%d||%s||%d\n", i+1, hsname[i], hsscore[i]);
		}
		fclose(sfp);
	}

	move(20, COLS/2-10);
	colouron(COL_YEL);
	addstr("Press ENTER to exit");
	colouroff(COL_YEL);
	move(21-21, COLS-COLS);

	while(1) {
		c = getch();
		if ( c == '\n')
			break;

	}
	for (i = 0; i < 5; i++){
		free(hsname[i]);

	}
	erase();


	if ( sem_post(&sm_hs) == -1){
		err(1, "semaphore failed to post");
		pthread_exit(NULL);
	};

	pthread_exit(NULL);
}
