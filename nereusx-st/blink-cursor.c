/*
 *	blink cursor support
 *	Copyleft (C) 2019 Nicholas Christopoulos
 */

#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

static int t_blink_mode = 1;
static int curs_blink_state= 0;
static int t_curs_init = 0;
static int t_curs_exit = 0;
static struct sigaction sa;
static struct itimerval timer;

/*
 *	timing
 *	VT100 1/4 of sec (special mode)
 *	CRT: min 20 (1/50Hz * 1000)
 * 
 *	\033[?12h = enable cursor blinking
 *	\033[?12l = disable cursor blinking
 * 
 *	Linux 4.2+
 *  \033[16;n] Set the cursor blink interval in milliseconds.
 */
static int t_curs_vis_ms = 1250;
static int t_curs_inv_ms =  500;

/*
 *	change visible values
 */
void timer_handler(int signum)
{
	static int count = 0;

	if ( signum == SIGALRM ) {
		count ++;
		if ( curs_blink_state == 0 ) {
			if ( count * 50 >= t_curs_vis_ms ) {
				count = 0;
				curs_blink_state = 1;
				}
			}
		else {
			if ( count * 50 >= t_curs_inv_ms ) {
				count = 0;
				curs_blink_state = 0;
				}
			}
		}
}

// returns 0 if cursor is no visible
int	get_blink_value()			{ if ( !t_blink_mode ) return 0; return curs_blink_state; }
// enable/disable cursor blinking
void set_blink_cursor(int mode)	{ t_blink_mode = mode; }
// 
int get_blink_cursor_mode()		{ return t_blink_mode; }

/*
 *	initialize mode
 */
void tcurs_init()
{
	if ( t_curs_init ==  0 ) {
		/* Install timer_handler as the signal handler for SIGVTALRM. */
		memset (&sa, 0, sizeof (sa));
		sa.sa_handler = &timer_handler;
		sigaction(SIGALRM, &sa, NULL);
		
		/* Configure the timer to expire after 50 msec... */
		timer.it_value.tv_sec = 0;
		timer.it_value.tv_usec = 50 * 1000;
		/* ... and every 50 msec after that. */
		timer.it_interval.tv_sec = 0;
		timer.it_interval.tv_usec = 50 * 1000;
		/* Start a virtual timer. It counts down whenever this process is executing. */
		setitimer(ITIMER_REAL, &timer, NULL);
		//
		t_curs_init = 1;
		}
}

