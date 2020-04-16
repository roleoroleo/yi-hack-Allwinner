echo "############## Starting Hack ##############"
echo "############## Starting Hack ##############"
echo "############## Starting Hack ##############"
echo "############## Starting Hack ##############"


### Check if telnetd is enabled, and enable it it's not already
echo "### Hacking telnet"
if [ `grep telnetd /home/app/init.sh` ]; then
	echo "Telnet already enabled"
else
	echo "Enabling telnet"
	echo '/usr/sbin/telnetd &' >> /home/app/init.sh
fi

### Check if yi-hack script is enable, and enable it it's not already
echo "### Hacking yi-hack script"
if [ `grep /home/yi-hack/script/system.sh /home/app/init.sh` ]; then
	echo "yi-hack script already enabled"
else
	echo "Enabling yi-hack script"
	echo '/home/yi-hack/script/system.sh' >> /home/app/init.sh
fi

### Disable Yi junk in init.sh
echo "### Disabling Yi Junk"
sed -i 's/^.\/mp4record/#.\/mp4record/g' /home/app/init.sh
sed -i 's/^.\/cloud/#.\/cloud/g' /home/app/init.sh
sed -i 's/^.\/p2p_tnp/#.\/p2p_tnp/g' /home/app/init.sh
sed -i 's/^.\/oss/#.\/oss/g' /home/app/init.sh
sed -i 's/^.\/watch_process/#.\/watch_process/g' /home/app/init.sh

### Replace /home/app/script/update.sh with a more friendly one
echo "### Updating /home/base/tools/extpkg.sh"
cp -r /tmp/sd/newhome/base/tools/extpkg.sh /home/base/tools/extpkg.sh

### Disable the hack for next reboot
echo "### Disabling hack for next reboot"
mv /tmp/sd/Factory /tmp/sd/Factory.done


### Check if firmware is available on SD card, and rename it if it is
if test -f /tmp/sd/home_y20gam.stage; then
    echo "/tmp/sd/home_y20gam.stage exist, renaming for firmware update"
	mv tmp/sd/home_y20gam.stage tmp/sd/home_y20gam
fi

reboot
