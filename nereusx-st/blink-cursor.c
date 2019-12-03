// ndc: blinking cursor
#include <pthread.h>
#include <unistd.h>
static pthread_t t_curs;
static volatile int t_blink_mode = 1;
static volatile int curs_blink_state= 0;
static volatile int t_curs_init = 0;
static volatile int t_curs_exit = 0;

void *tcurs_xchg(void *args)
{
	while ( !t_curs_exit ) {
		curs_blink_state = !curs_blink_state;
		if ( !curs_blink_state )
			usleep(1250000);
		else
			usleep( 500000);
		}
	pthread_exit(NULL);
}

int	get_blink_value()
{
	if ( !t_blink_mode )
		return 0;
	return curs_blink_state;	
}

void tcurs_term()
{
	t_curs_exit = 1;
	pthread_join(t_curs, NULL);
	pthread_exit(NULL);
}
void tcurs_init()
{
	if ( t_curs_init ==  0 ) {
		t_curs_init = 1;
		pthread_create(&t_curs, NULL, tcurs_xchg, NULL);
		}
	atexit(tcurs_term);
}

void set_blink_cursor(int mode)
{
	t_blink_mode = mode;
}
