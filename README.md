# USB Gadget simulate game pad for nintendo switch
**target device: raspberry pi zero w**

**target operating system: raspbian**

very thanks https://github.com/mumumusuc/pi-joystick, https://github.com/milador/XAC-Virtual-Joystick

## Prepare you system
### Download raspbian from raspberry pi official site
please go to the site: https://www.raspberrypi.org/downloads/raspbian/ download the raspbian image

### create SD card for rpi zero w
* use linux command:
```shell
sudo dd if=xxxxx.img of=/dev/sdx
```

### Set USB2 and USB gadget environment
to use the command on your raspbian system (after boot the rpi zero w)
```shell
cd ~
git clone https://github.com/fusigi0930/switch_joystick_emu
cd switch_joystick_emu
sudo ./setup_zero_w.sh
sudo reboot
```

after reboot the system, the USB2 and gadget environment shall be setup.

### Building joystick emulator
this project use C++ and lua to build a js_emu daemon to emulate game pad and communicate with nintendo switch, there has 2 method to build the project:
* building in rpi zero w (preferred)
```shell
cd ~/switch_joystick_emu/js_emu
make
```

* cross compiler in your PC
> because the project and Lua project need use readline library, it dose not contain in the cross compiler, you need prepare sysroot by yourself..., let us pass for this method...

### setup to your system
copy files to your system:
```shell
cd ~/switch_joystick_emu
sudo cp system/bin/switch_pro_joystick /usr/bin
sudo cp system/bin/jsemu /usr/bin
sudo cp js_emu/build/out /usr/bin
sudo cp system/udev/* /etc/udev/rules.d/
sudo cp system/systemd/jsemu.service /lib/systemd/system
```

then reboot the device.

## How to use
**CAUTION!!,  before you start the service or test tools, you have to connect your raspberry pi to Nintendo Switch first!!**

### test the daemon tool
you can test the daemon and usb gadget config first, so you have to connect your raspberry pi zero w to Nintendo Switch, then run the commands:
```shell
switch_pro_joystick create
js_emu --test "js_button(\"a\",1); wait(100); js_button(\"a\", 0);"
```
after run the test command, rpi should help you "PRESS" button A once.

### start service
to start the switch game pad service by this command:
```shell
sudo systemctl enable jsemu
sudo systemctl start jsemu
```
after the servcie is started, it can load the lua script to play switch by your command.

### control your raspberry pi zero w
there are 3 methods to connect your raspberry pi zero w
* use linux command -- nc, connect to the raspberry pi zero w over WiFi (TCP/IP)
* use linux command -- ssh, connect to the raspberry pi zero w over WiFi (SSH)
* use programming language and connect to raspberry pi zero w over WiFi (socket programming)

that means you need connect to WiFi in your raspberry pi zeor w first, we assume this part is done, and the raspberry pi ip address....x.x.x.x

the commands to run lua script are:
* part 1:
```shell
nc x.x.x.x 9090
run <absolute path for a lua file>
```

* part 2:
```shell
ssh x.x.x.x
nc 127.0.0.1 9090
run <absolute path for a lua file>
```

* part 3:
create a socket to connect x.x.x.x with port 9090 in your program, then write "run <absolute path for a lua file>" string to the socket file description.

### api for Lua
please refer the test.lua file in this project folder "example"
