#!/bin/sh
echo "--------------------------home app init.sh--------------------------"

PATH="/usr/bin:/usr/sbin:/bin:/sbin:/home/base/tools:/home/app/localbin:/home/base"
LD_LIBRARY_PATH="/lib:/usr/lib:/home/lib:/home/qigan/lib:/home/app/locallib"
export PATH
export LD_LIBRARY_PATH

ulimit -c unlimited
ulimit -s 1024
#echo /tmp/sd/core.exe[%e].pid[%p].sig[%s] > /proc/sys/kernel/core_pattern

echo 100 > /proc/sys/vm/dirty_writeback_centisecs
echo 50 > /proc/sys/vm/dirty_expire_centisecs
#echo 2048 > /proc/sys/vm/extra_free_kbytes

echo 400 > /proc/sys/vm/vfs_cache_pressure
echo 3 > /proc/sys/vm/drop_caches
echo 2048 > /proc/sys/vm/min_free_kbytes

echo 11 3 > /sys/class/hwmon/hwmon0/port_qos
echo 12 3 > /sys/class/hwmon/hwmon0/port_qos

sysctl -w vm.dirty_background_ratio=2
sysctl -w vm.dirty_expire_centisecs=500
sysctl -w vm.dirty_ratio=2
sysctl -w vm.dirty_writeback_centisecs=100

sysctl -w fs.mqueue.msg_max=256
mkdir /dev/mqueue
mount -t mqueue none /dev/mqueue

mount tmpfs /tmp -t tmpfs -o size=32m
mkdir /tmp/sd
mkdir /dev/shm
mkdir /tmp/var
mkdir /tmp/var/run
mkdir /tmp/run


checkdisk
rm -fr /tmp/sd/FSCK*.REC
umount -l /tmp/sd
mount -t vfat /dev/mmcblk0p1 /tmp/sd

#----insmod hardware cyption------

if [ -f /home/home_y25gam ]; then
    echo "---/home/home_y25gam exist, update begin---"
	dd if=/home/home_y25gam of=/tmp/newver bs=22 count=1
	newver=$(cat /tmp/newver)
	if [ -f /home/homever ]; then
		curver=$(cat /home/homever)
	else
		curver=0
	fi
	echo check version: newver=$newver, curver=$curver
	if [ $newver != $curver ]; then
		### cipher ###
		sleep 1
		mkdir /tmp/update
		cp -rf /home/base/tools/extpkg.sh /tmp/update/extpkg.sh
		/tmp/update/extpkg.sh /home/home_y25gam
		rm -rf /tmp/update
		rm -rf /home/home_y25gam
		#sync
		echo "update finish"
		reboot -f
	fi
	echo "---same version ? update fail---"
	rm -rf mv /home/home_y25gam
elif [ -f /tmp/sd/home_y25gam ]; then
	echo "---tmp/sd/home_y25gam exist, update begin---"
	dd if=/tmp/sd/home_y25gam of=/tmp/newver bs=22 count=1
	newver=$(cat /tmp/newver)
	if [ -f /home/homever ]; then
		curver=$(cat /home/homever)
	else
		curver=0
	fi
	echo check version: newver=$newver, curver=$curver
	if [ $newver != $curver ]; then
		### cipher ###
		sleep 1
		mkdir /tmp/update
		cp -rf /home/base/tools/extpkg.sh /tmp/update/extpkg.sh
		/tmp/update/extpkg.sh /tmp/sd/home_y25gam
		rm -rf /tmp/update
		mv /tmp/sd/home_y25gam	/tmp/sd/home_y25gam.done
		#sync
		echo "update finish"
		reboot -f
	fi
	echo "---same version ? update fail---"
	mv /tmp/sd/home_y25gam	/tmp/sd/home_y25gam.done
else
	echo "---update file(home_y25gam) Not exist---"
fi

echo "--------------------------insmod sound--------------------------"
insmod /home/qigan/ko/regmap-mmio.ko
insmod /home/qigan/ko/snd-soc.ko
insmod /home/qigan/ko/snd-soc-daudio.ko
insmod /home/qigan/ko/sound-codec.ko
insmod /home/qigan/ko/sound-sndcodec.ko
insmod /home/qigan/ko/sound-snddaudio.ko
echo "--------------------------insmod sensor--------------------------"
insmod /home/qigan/ko/videobuf2-core.ko
insmod /home/qigan/ko/videobuf2-memops.ko
insmod /home/qigan/ko/videobuf2-dma-contig.ko
insmod /home/qigan/ko/videobuf2-v4l2.ko
insmod /home/qigan/ko/vin_io.ko
insmod /home/qigan/ko/sensor_power.ko dvdd0_vol="1200000" mclk0="27000000"

/home/app/script/factory_test.sh

### wifi 8189 ###
insmod /home/base/wifi/8189fs.ko
#echo "MTK 7601" > /tmp/MTK
#insmod /home/base/wifi/sunxi_gmac.ko

sleep 1
ifconfig lo up
ifconfig wlan0 up
#ethmac=d2:`ifconfig wlan0 |grep HWaddr|cut -d' ' -f10|cut -d: -f2-`
#ifconfig eth0 hw ether $ethmac
#ifconfig eth0 up

cd /home/app
#./log_server &
./dispatch &

if [ -f "/tmp/sd/Factory/factory_test.sh" ]; then
	/tmp/sd/Factory/config.sh
	exit
fi

#sleep 2
#./rmm &
#sleep 2
#./mp4record &
#./cloud &
#./p2p_tnp &
#./oss &
#./watch_process &
/usr/sbin/telnetd &
/home/yi-hack/script/system.sh
