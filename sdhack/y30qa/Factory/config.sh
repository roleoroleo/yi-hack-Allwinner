echo "############## Starting Hack ##############"
echo "############## Starting Hack ##############"
echo "############## Starting Hack ##############"
echo "############## Starting Hack ##############"


### Remove core if it exists
rm -f /home/app/core

### Check if telnetd is enabled, and enable it it's not already
echo "### Hacking telnet"
if [ `grep telnetd /home/app/init.sh | grep -c ^` -gt 0 ]; then
    echo "Telnet already enabled"
else
    echo "Enabling telnet"
    echo '/usr/sbin/telnetd &' >> /home/app/init.sh
fi

### Check if yi-hack script is enable, and enable it it's not already
echo "### Hacking yi-hack script"
if [ `grep /home/yi-hack/script/system.sh /home/app/init.sh | grep -c ^` -gt 0 ]; then
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
sed -i 's/^.\/rmm/#.\/rmm/g' /home/app/init.sh
sed -i 's/^sleep 2/#sleep 2/g' /home/app/init.sh

### Replace /home/base/tools/extpkg.sh with a more friendly one
FILE="home/base/tools/extpkg.sh"
echo "### /$FILE"
if [ -f /tmp/sd/new$FILE ]; then
    CHK1=`md5sum /$FILE | awk '{ print $1 }')`
    CHK2=`md5sum /tmp/sd/new$FILE | awk '{ print $1 }')`
    if [ "$CHK1" != "$CHK2" ]; then
        cp -r /tmp/sd/new$FILE /$FILE
        chmod 0755 /$FILE
    fi
fi

### Replace /home/app/script/wifidhcp.sh with a more friendly one
FILE="home/app/script/wifidhcp.sh"
echo "### /$FILE"
if [ -f /tmp/sd/new$FILE ]; then
    CHK1=`md5sum /$FILE | awk '{ print $1 }')`
    CHK2=`md5sum /tmp/sd/new$FILE | awk '{ print $1 }')`
    if [ "$CHK1" != "$CHK2" ]; then
        cp -r /tmp/sd/new$FILE /$FILE
        chmod 0755 /$FILE
    fi
fi

### Disable the hack for next reboot
echo "### Disabling hack for next reboot"
if [ -e /tmp/sd/Factory.done ]; then
    rm -rf /tmp/sd/Factory.done
fi
if [ -e /tmp/sd/Factory ]; then
    mv /tmp/sd/Factory /tmp/sd/Factory.done
fi

### Check if firmware is available on SD card, and rename it if it is
if test -f /tmp/sd/home_y30qam.stage; then
    echo "/tmp/sd/home_y30qam.stage exist, renaming for firmware update"
    mv /tmp/sd/home_y30qam.stage /tmp/sd/home_y30qam
fi

reboot
