/*************************************************************************
	> File Name: common.h
	> Author:
	> Mail:
	> Created Time:
 ************************************************************************/

#ifndef _COMMON_H
#define _COMMON_H

#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/sysinfo.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>
#include <linux/videodev2.h>


long long secs_to_msecs(long secs, long usecs);
int save_frame_to_file(void *str,void *start,int length);
float measure_fps(int fd);

#endif
