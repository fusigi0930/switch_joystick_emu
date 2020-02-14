mkdir temp
sudo mount -t vfat /dev/mmcblk0p1 temp
echo "dtoverlay=dwc2" >> temp/config.txt
sudo umount temp
rmdir temp

sudo echo "dwc2" >> /etc/modules
sudo echo "libcomposite" >> /etc/modules
sudo echo "g_multi" >> /etc/modules

sudo apt install libreadline-dev git gcc g++
