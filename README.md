# st

Suckless simple terminal (https://st.suckless.org/)

To be used with TMUX

## Applied patches:
* transparency (st-alpha-0.8.2.diff)
* desktop entry added (st-desktopentry-0.8.2.diff)
* select mode by keyboard (st-keyboard_select-0.8.2.diff)

## Modifications:
* Added config file (~/.strc) with the following keys: `rows, cols, alpha, border, font`.
	If statement `?` else `:` and end `-`.
	The syntax is the same as the config.h
example:
```
	# config for st
	? test "$HOSTNAME" = "mypc"
	cols=140
	:
  	cols=132
	-
	rows=35
	font=Liberation Mono:pixelsize=12:antialias=true:autohint=true
```
* Added blinking cursor
```
VT100 codes:
	\033[?12h = enable cursor blinking
	\033[?12l = disable cursor blinking
```	
* Copy-mode: Ctrl-Shift-A
* Move-mode: several
* More icons added

