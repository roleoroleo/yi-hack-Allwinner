#!/bin/bash

update_init()
{
    ### Update /home/init.sh with a more friendly one
    echo "### Updating $DIR/home/init.sh"
    cp $DIR/home/app/init.sh $DIR/home/app/tmp_init.sh

    echo "### Enabling telnet"
    echo "# Running telnetd" >> $DIR/home/app/tmp_init.sh
    echo "/usr/sbin/telnetd &" >> $DIR/home/app/tmp_init.sh

    echo "### Enabling yi-hack script"
    echo '/home/yi-hack/script/system.sh' >> $DIR/home/app/tmp_init.sh

    ### Disable Yi junk in init.sh
    echo "### Disabling Yi Junk"
    sed -i 's/^.\/mp4record/#.\/mp4record/g' $DIR/home/app/tmp_init.sh
    sed -i 's/^.\/cloud/#.\/cloud/g' $DIR/home/app/tmp_init.sh
    sed -i 's/^.\/p2p_tnp/#.\/p2p_tnp/g' $DIR/home/app/tmp_init.sh
    sed -i 's/^.\/oss/#.\/oss/g' $DIR/home/app/tmp_init.sh
    sed -i 's/^.\/rtmp/#.\/rtmp/g' $DIR/home/app/tmp_init.sh
    sed -i 's/^.\/watch_process/#.\/watch_process/g' $DIR/home/app/tmp_init.sh
    sed -i 's/^.\/rmm/#.\/rmm/g' $DIR/home/app/tmp_init.sh
    sed -i 's/^sleep 2/#sleep 2/g' $DIR/home/app/tmp_init.sh

    cp -f $DIR/home/app/tmp_init.sh $DIR/home/app/init.sh
    rm $DIR/home/app/tmp_init.sh
    echo $RANDOM | md5sum | cut -d ' ' -f 1 > $DIR/home/.random

    ### Replace /home/base/tools/extpkg.sh with a more friendly one
    echo "### Updating $DIR/home/base/tools/extpkg.sh"
    FILE="home/base/tools/extpkg.sh"
    if [ -f $DIR/new$FILE ]; then
        cp -r $DIR/new$FILE $DIR/$FILE
    fi

    ### Replace /home/app/script/wifidhcp.sh with a more friendly one
    echo "### Updating $DIR/home/app/script/wifidhcp.sh"
    FILE="home/app/script/wifidhcp.sh"
    if [ -f $DIR/new$FILE ]; then
        cp -r $DIR/new$FILE $DIR/$FILE
    fi
}

TYPE=$1
if [ "$TYPE" != "factory" ] && [ "$TYPE" != "hacked" ]; then
    echo "Usage: $0 type"
    echo -e "\twhere type = \"factory\" or \"hacked\""
    exit
fi

for DIR in * ; do
    if [ -d $DIR ]; then
        FILENAME=`cat $DIR/filename`
        if [ -f $DIR/mtdblock2.bin ]; then
            mkimage -A arm -O linux -T filesystem -C none -a 0x0 -e 0x0 -n "xiaoyi-rootfs" -d $DIR/mtdblock2.bin $DIR/rootfs_$FILENAME
            gzip $DIR/rootfs_$FILENAME
        fi
        if [ -f $DIR/mtdblock3.bin ]; then
            if [ "$TYPE" == "hacked" ]; then
                mkdir -p $DIR/mnt
                ./mount.jffs2 $DIR/mtdblock3.bin $DIR/mnt 4
                cp -r $DIR/mnt $DIR/home
                umount $DIR/mnt
                rm -rf $DIR/mnt
                update_init
                if [ $FILENAME == "y501gc" ]; then
                    PAD="0x01b20000"
                else
                    PAD="0x00b80000"
                fi
                ./create.jffs2 $PAD $DIR/home $DIR/mtdblock3_hacked.bin
                rm -rf $DIR/home
            else
                cp $DIR/mtdblock3.bin $DIR/mtdblock3_hacked.bin
            fi
            mkimage -A arm -O linux -T filesystem -C none -a 0x0 -e 0x0 -n "xiaoyi-home" -d $DIR/mtdblock3_hacked.bin $DIR/home_$FILENAME
            rm -f $DIR/mtdblock3_hacked.bin
            gzip $DIR/home_$FILENAME
        fi
    fi
done
