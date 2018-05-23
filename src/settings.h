#ifndef SETTINGS_H
#define SETTINGS_H

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
 
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
/* 
 * These are global constants chaning one will require a full rebuild 
 * The numbers at the end are the defaults for the game
 */
#define	MAXSHIP	10	/* Maximum number of ships allowed 		10    */
#define MAXROCK	5	/* Maximum number of rockets allowed  		5     */
#define MAXEROCK 10	/* Maximum number of enemy rockets allowed	10    */
#define ROCKINC 1	/* Num of rockets add when a ship is kill 	1     */
#define STROCK	10	/* Number of rockets you start with 		10    */
#define ESCLIM	10	/* Maximum number of ships allowed to escape 	10    */
#define	TUNIT	20000	/* Timeunits in microseconds 			20000 */
#define RTUNIT	40000	/* Rocket speed 				40000 */
#define ERTUNIT	60000	/* Enemy rocket speed 				60000 */
#define DTUNIT	100000	/* Length of time for death animations 		100000*/
#define GTUNIT	1000000	/* Timeunits for random spawn of ships	       1000000*/

#endif
