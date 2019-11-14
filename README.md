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

* Copy-mode: Ctrl-Shift-A
* Move-mode: serveral
* More icons added

