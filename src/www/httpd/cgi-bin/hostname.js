#!/bin/sh

printf "Content-type: text/javascript\r\n\r\n"

printf "hostname=\"%s\";" $(cat /home/yi-hack/etc/hostname)
