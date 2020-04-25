
function run_circle(time)
	delay = math.floor(time / 360)
	for degree=0,359 do
		x = math.floor(126 * math.cos(math.rad(degree)))
		y = math.floor(126 * math.sin(math.rad(degree)))
		wait(delay)
		js_analog(x,y, 0, 0)
	end
	js_analog(0, 0, 0, 0)
	wait(500)
end

function press_button(buttons)
	wait(50)
	js_button(buttons, 1)
	wait(100)
	js_button(buttons, 0)
	wait(300)
end

function attack()
	press_button("a")
	press_button("a")
	press_button("x")
	wait(900)
end

function running_thread()
	while true do
		run_circle(8000)
	end
end

run_thread("running_thread")

while true do
	attack()
	wait(500)
end

