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
cd ~/cd switch_joystick_emu/js_emu
make
```

* cross compiler in your PC
> because the project and Lua project need use readline library, it dose not contain in the cross compiler, you need prepare sysroot by yourself..., let us pass for this method...

### setup to your system
copy files to your system:
```shell
cd ~/cd switch_joystick_emu
sudo cp system/bin/switch_pro_joystick /usr/bin
sudo cp system/bin/jsemu /usr/bin
sudo cp js_emu/build/out /usr/bin
sudo cp system/udev/* /etc/udev/rules.d/
sudo cp system/systemd/jsemu.service /lib/systemd/system
```

## How to use
to be continue...
