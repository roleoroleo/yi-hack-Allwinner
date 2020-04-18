#!/bin/sh

printf "Content-type: application/octet-stream\r\n\r\n"

TMP_DIR="/tmp/yi-temp-save"
mkdir $TMP_DIR
cd $TMP_DIR
cp /home/yi-hack/etc/*.conf .
if [ -f /home/yi-hack/etc/hostname ]; then
    cp /home/yi-hack/etc/hostname .
fi
tar cvf config.tar * > /dev/null
bzip2 config.tar config.tar.bz2
cat $TMP_DIR/config.tar.bz2
cd /tmp
rm -rf $TMP_DIR
