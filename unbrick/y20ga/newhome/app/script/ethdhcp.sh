killall udhcpc
HN="yi-hack"
if [ -f /home/yi-hack/etc/hostname ]; then
        HN=$(cat /home/yi-hack/etc/hostname)
fi
udhcpc -i eth0 -b -s /home/app/script/default.script -x hostname:$HN
