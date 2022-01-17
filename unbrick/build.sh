#!/bin/bash

TYPE=$1
if [ "$TYPE" != "factory" ] && [ "$TYPE" != "hacked" ]; then
    echo "Usage: $0 type"
    echo -e "\twhere type = \"factory\" or \"hacked\""
    exit
fi

for DIR in * ; do
    if [ -d $DIR ]; then
        FILENAME=`cat $DIR/filename`
        if [ "$TYPE" == "hacked" ]; then
            mkdir -p $DIR/mnt
            ./mount.jffs2 $DIR/mtdblock3.bin $DIR/mnt 4
            cp -r $DIR/mnt $DIR/home
            umount $DIR/mnt
            rm -rf $DIR/mnt
            cp -f $DIR/newhome/app/init.sh $DIR/home/app/init.sh
            cp -f $DIR/newhome/app/script/update.sh $DIR/home/app/script/update.sh
            cp -f $DIR/newhome/app/script/wifidhcp.sh $DIR/home/app/script/wifidhcp.sh
            cp -f $DIR/newhome/base/tools/extpkg.sh $DIR/home/base/tools/extpkg.sh
            PAD="0x00b80000"
            ./create.jffs2 $PAD $DIR/home $DIR/mtdblock3_hacked.bin
            rm -rf $DIR/home
        else
            cp $DIR/mtdblock3.bin $DIR/mtdblock3_hacked.bin
        fi
        mkimage -A arm -O linux -T filesystem -C none -a 0x0 -e 0x0 -n "xiaoyi-home" -d $DIR/mtdblock3_hacked.bin $DIR/home_$FILENAME
        rm -f $DIR/mtdblock3_hacked.bin
        gzip $DIR/home_$FILENAME
    fi
done
