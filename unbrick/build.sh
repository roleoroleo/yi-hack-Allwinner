#!/bin/bash

for DIR in * ; do
    if [ -d $DIR ]; then
        FILENAME=`cat $DIR/filename`
        if [ -f $DIR/mtdblock3.bin ]; then
            cp $DIR/mtdblock3.bin $DIR/mtdblock3_hacked.bin
            mkimage -A arm -O linux -T filesystem -C none -a 0x0 -e 0x0 -n "xiaoyi-home" -d $DIR/mtdblock3_hacked.bin $DIR/home_$FILENAME
            rm -f $DIR/mtdblock3_hacked.bin
            gzip $DIR/home_$FILENAME
        fi
    fi
done
