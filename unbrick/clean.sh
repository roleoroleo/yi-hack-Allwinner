#!/bin/bash

for DIR in * ; do
    if [ -d $DIR ]; then
        FILENAME=`cat $DIR/filename`
        rm -f $DIR/home_$FILENAME.gz
    fi
done
