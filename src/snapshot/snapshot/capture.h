#ifndef CAPTURE__H__
#define CAPTURE__H__

#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <linux/videodev2.h>
#include <pthread.h>
#include <semaphore.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>

//#include "water_mark.h"
//#include "add_water.h"
#include "convert.h"
#include "common.h"

#define ALL_TYPE 0
#define BMP_TYPE 1
#define JPG_TYPE 2
#define YUV_TYPE 3

#define true 1
#define false 0

#define N_WIN_SIZES 20

#define ALIGN_16B(x) (((x) + (15)) & ~(15))

extern int camera_dbg_en;

int capture_main(char *filename, int par_width, int par_height, int par_photo_type, unsigned int par_pixelformat, int par_camera_index);

//for internel debug
#define camera_dbg(x,arg...) do{ \
                                if(camera_dbg_en) \
                                    fprintf(stderr,"[CAMERA_DEBUG]"x,##arg); \
                            }while(0)

//print when error happens
#define camera_err(x,arg...) do{ \
                                fprintf(stderr,"\033[1m\033[;31m[CAMERA_ERR]"x,##arg); \
                                fprintf(stderr,"\033[0m"); \
                                fflush(stderr); \
                            }while(0)

#define camera_prompt(x,arg...) do{ \
                                fprintf(stderr,"\033[1m\033[;32m[CAMERA_PROMPT]"x"\033[0m",##arg); \
                                fflush(stderr); \
                            }while(0)

#define camera_warn(x,arg...) fprintf(stderr,"[CAMERA_WARN]"x,##arg)
//print unconditional, for important info
#define camera_print(x,arg...) fprintf(stderr,"[CAMERA]"x,##arg)

struct buffer
{
    void *start[3];
    size_t length[3];
};

struct v4l2_frmsize
{
    unsigned int width;
    unsigned int height;
};

struct vfe_format
{
    unsigned char name[32];
    unsigned int fourcc;
    ConverFunc ToRGB24Func;
    struct v4l2_frmsize size[N_WIN_SIZES];
};

typedef struct camera_hal
{
    int camera_index;
    int videofd;
    unsigned char isDefault;
    int driver_type;
    int sensor_type;
    int ispId;
    int photo_type;
    int photo_num;
    char save_path[64];
    struct buffer *buffers;
    int buf_count;
    int nplanes;
    unsigned int win_width;
    unsigned int win_height;
    unsigned int pixelformat;
    ConverFunc ToRGB24;
    int use_wm;
//    WaterMarkInfo WM_info;
}camera_handle;


#endif
