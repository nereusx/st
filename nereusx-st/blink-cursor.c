/*
 *	blink cursor support
 *	Copyleft (C) 2019 Nicholas Christopoulos
 */

#include <pthread.h>
#include <unistd.h>
static pthread_t t_curs;
static volatile int t_blink_mode = 1;
static volatile int curs_blink_state= 0;
static volatile int t_curs_init = 0;
static volatile int t_curs_exit = 0;

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
static volatile int t_curs_vis_ms = 1250;
static volatile int t_curs_inv_ms =  500;

/*
 *	change visible values
 */
void *tcurs_xchg(void *args)
{
	while ( !t_curs_exit ) {
		curs_blink_state = !curs_blink_state;
		if ( !curs_blink_state )
			usleep(t_curs_vis_ms * 1000);
		else
			usleep(t_curs_inv_ms * 1000);
		}
	pthread_exit(NULL);
}

/*
 *	returns 0 if cursor is no visible
 */
int	get_blink_value()
{
	if ( !t_blink_mode )
		return 0;
	return curs_blink_state;	
}

/*
 *	terminate (atexit)
 */
void tcurs_term()
{
	t_curs_exit = 1;
	pthread_join(t_curs, NULL);
	pthread_exit(NULL);
}

/*
 *	enable/disable blinking
 */
void set_blink_cursor(int mode)
{
	t_blink_mode = mode;
}

/*
 *	initialize mode
 */
void tcurs_init()
{
	if ( t_curs_init ==  0 ) {
		t_curs_init = 1;
		pthread_create(&t_curs, NULL, tcurs_xchg, NULL);
		}
	atexit(tcurs_term);
}

