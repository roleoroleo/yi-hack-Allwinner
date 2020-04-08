/*************************************************************************
> File Name: common.c
> Author:
> Mail:
> Created Time:
************************************************************************/

#include "common.h"

#define MAX_FRAME_NUM 64

long long secs_to_msecs(long secs, long usecs)
{
    long long msecs;

    msecs = usecs/1000 + secs* 1000;

    return msecs;
}

int save_frame_to_file(void *str,void *start,int length)
{
    FILE *fp = NULL;

    fp = fopen(str, "wb+"); //save more frames
    if(!fp)
    {
//        printf(" Open %s error\n", (char *)str);

        return -1;
    }

    if(fwrite(start, length, 1, fp))
    {
        fclose(fp);

        return 0;
    }
    else
    {
//        printf(" Write file fail (%s)\n",strerror(errno));
        fclose(fp);

        return -1;
    }

    return 0;
}

/****************************************************************************
*Measure the frame rate according to the interval of dqbuf.
*Take the last three times the average.
***************************************************************************/
float measure_fps(int fd)
{
    struct v4l2_buffer buf;
    struct timeval tv;
    long long timestamp = 0;
    fd_set testfds;
    int camera_type = 0;
    int nplanes;
    struct v4l2_capability cap;
    struct v4l2_format fmt;
    long long timestamp_start,timestamp_end;
    int np = 0;
    int ret = 0;
    int timeval = 0;

    if(ioctl(fd,VIDIOC_QUERYCAP,&cap) < 0){
        printf(" Query device capabilities fail!!!\n");

        return -1;
    }
    if(strcmp(cap.driver,"sunxi-vin") == 0){
        camera_type = 1;

        fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
        if (ioctl(fd, VIDIOC_G_FMT, &fmt) < 0){
            printf(" get the data format failed!\n");
            return -1;
        }
        nplanes = fmt.fmt.pix_mp.num_planes;
    }

    FD_ZERO(&testfds);
    FD_SET(fd,&testfds);

    memset(&buf,0,sizeof(struct v4l2_buffer));
    if(camera_type == 1)
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
    else
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    if(camera_type == 1){
        buf.length = nplanes;
        buf.m.planes = (struct v4l2_plane *)calloc(nplanes, sizeof(struct v4l2_plane));
    }

    while(1)
    {
        /* wait for sensor capture data */
        tv.tv_sec  = 1;
        tv.tv_usec = 0;
        ret = select(fd + 1, &testfds, NULL, NULL, &tv);
        if (ret == -1){
            printf(" select error\n");
        }else if (ret == 0){
            printf(" select timeout, measure the frame rate failed\n");
            return 0;
        }

        /* dqbuf */
        ret = ioctl(fd, VIDIOC_DQBUF, &buf);
        if(ret != 0)
            printf("****DQBUF FAIL*****\n");

        if (np == 0){
            gettimeofday(&tv, NULL);
            timestamp_start = secs_to_msecs(tv.tv_sec, tv.tv_usec);
        }

        /* qbuf */
        if (ioctl(fd, VIDIOC_QBUF, &buf) != 0)
            printf("****QBUF FAIL*****\n");

        np++;
        if(np >= MAX_FRAME_NUM){
            gettimeofday(&tv, NULL);
            timestamp_end = secs_to_msecs(tv.tv_sec, tv.tv_usec);
            break;
        }
    }

    if(camera_type == 1)
        free(buf.m.planes);

    return 1000.0/(float)((timestamp_end-timestamp_start) / MAX_FRAME_NUM);
}
