/*
 *	load configuration file
 *	Copyleft (C) 2019 Nicholas Christopoulos
 */

/* the file name */
#define STRC_FILE		".strc"

/* structure of keywords */
typedef struct {
	const char *key; /* the name in ~/.strc */
	void	*var;	/* pointer to the variable */
	char	type;	/* type of the variable: (i)nteger, (f)loat, (s)tring */
	} strc_keyword;

/*
 *	keyword<->variable table for ~/.strc
 */
static strc_keyword strc_keys[] = {
	{ "cols",		&cols,		'i' },
	{ "rows",		&rows,		'i' },
	{ "alpha",		&alpha,		'f' },
	{ "border",		&borderpx,	'i' },
	{ "borderpx",	&borderpx,	'i' },
	{ "font",		&font,		's' },
	{ NULL, NULL, 0 }
	};

void
load_config(void)
{
	char *p, *key, *val, *file, *home;
	char buf[LINE_MAX];
	FILE *fp;
	int  i, status = 0;

	if ( (home = (char *) getenv("HOME")) != NULL ) {
		if ( (file = (char *) malloc(strlen(home) + strlen(STRC_FILE) + 2)) != NULL ) {
			sprintf(file, "%s/%s", home, STRC_FILE);
			if ( (fp = fopen(file, "rt")) != NULL ) {
				while ( fgets(buf, LINE_MAX, fp) ) {
					p = buf;
	
					while ( *p == ' ' || *p == '\t' ) p ++;						/* skip spaces */
					if ( *p == '#' || *p == '\n' ) continue;					/* comments or empty line */
					if ( *p == '?' ) { status = system( ++ p ); continue; }		/* 'if' command, nested-ifs can be add by using push/pop status */
					if ( *p == ':' ) { status = ( status )? 0 : 1; continue; }	/* 'else' command */
					if ( *p == '-' ) { status = 0; continue;  }					/* 'end-if' command */
					if ( status != 0 ) continue;								/* do 'if-else' check */
	
					/* mark keyword */
					key = p;
					while ( *p >= 'a' && *p <= 'z'  )	p ++;
					while ( *p == ' ' || *p == '\t' )	*p ++ = '\0';
					if ( *p == '=' )					*p ++ = '\0';
	
					/* mark value */
					val = p;
					while ( *p && *p != '\n' ) p ++;
					*p = '\0'; /* actually removes the '\n' on the end of buffer */
					
					/* apply the values */
					for ( i = 0; strc_keys[i].key; i ++ ) {
						if ( strcmp(key, strc_keys[i].key) == 0 ) {
							switch ( strc_keys[i].type ) {
							case 'i': *((int   *) strc_keys[i].var) = atoi(val); break;
							case 'f': *((float *) strc_keys[i].var) = atof(val); break;
							case 's': *((char **) strc_keys[i].var) = strdup(val); break;
								}
							break;
							}
						}
					}
				fclose(fp);
				}
			free(file);
			}
		}
}
