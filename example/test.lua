--[[
support functions
* js_button(string, number);
*	control joystick (game pad) buttons api
	+ string: buttons list --
		a:	button A
		b:	button B
		x:	button X
		y:	button Y
		l:	button L
		r:	button R
		zl:	button ZL
		zr:	button ZR
		3l:	button LClick
		3r:	button RClick
		+:	button plus
		-:	button minus
		h:	button HOME
		c:	button CAPTURE
	+ number: press or release
		press:		1
		release:	0

* js_direct(number);
*	control joystick (game pad) HAT direction api
	+ number: axis constant
		UP:			direction top
		UPRIGHT:	direction top+right
		RIGHT:		direction right
		DOWNRIGHT:	direction bottom+right
		DOWN:		direction bottom
		DOWNLEFT:	direction bottom+left
		LEFT:		direction left
		UPLEFT:		direction top+left
		NONE:		no direction, center

* js_analog(lx, ly, rx, ry);
*	control joystick (game pad) left and right analog axis
	+ lx: left X axis (-127~128)
	+ ly: left Y axis (-127~128)
	+ rx: right X axis (-127~128)
	+ ry: right Y axis (-127~128)

* wait(milliseconds);
*	wait for millisecond
	+ milliseconds: as title.....

* run_thread(func_name);
*	run a lua function as thread
	+ func_name: lua function name without arguments

--]]

js_direct(UP); wait(300); js_direct(NONE); wait(300);
js_direct(UP); wait(300); js_direct(NONE); wait(300);
js_direct(DOWN); wait(300); js_direct(NONE); wait(300);
js_direct(DOWN); wait(300); js_direct(NONE); wait(300);
js_direct(LEFT); wait(300); js_direct(NONE); wait(300);
js_direct(RIGHT); wait(300); js_direct(NONE); wait(300);
js_direct(LEFT); wait(300); js_direct(NONE); wait(300);
js_direct(RIGHT); wait(300); js_direct(NONE); wait(300);
js_button("b", 1); wait(200); js_button("b", 0); wait(300);
js_button("a", 1); wait(200); js_button("a", 0); wait(300);
js_button("b", 1); wait(200); js_button("b", 0); wait(300);
js_button("a", 1); wait(200); js_button("a", 0); wait(300);
