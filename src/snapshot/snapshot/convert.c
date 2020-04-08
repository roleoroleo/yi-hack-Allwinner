/*************************************************************************
    > File Name: convert.c
    > Author:
    > Mail:
    > Created Time:
 ************************************************************************/

#include "convert.h"


/*************************************
*NV21:Y VUVU
*YV12:Y V U
**************************************/
int NV21ToYV12(void *save_YV12,void *NV21,int width,int height)
{
    unsigned char *src_uv = (unsigned char *)NV21 + width * height;
    unsigned char *dst_v = (unsigned char *)save_YV12 + width * height;
    unsigned char *dst_u = (unsigned char *)dst_v + (width * height/4);

    if(save_YV12 == NULL || NV21 == NULL || width <= 0 || height <= 0)
    {
        printf(" NV21ToYV12 incorrect input parameter!\n");
        return -1;
    }

    //copy Y
    memcpy(save_YV12,NV21,width * height);

    //copy U
    for(int i = 1; i < width*height/2; i += 2)
    {
        *(dst_u++) = *(src_uv + i);
    }

    //copy V
    for(int j = 0; j < width*height/2; j += 2)
    {
        *(dst_v++) = *(src_uv + j);
    }

    return 0;

}

/*************************************
*NV21:Y VUVU
*YU12:Y U V
**************************************/
int NV21ToYU12(void *save_YU12,void *NV21,int width,int height)
{
    unsigned char *src_uv = (unsigned char *)NV21 + width * height;
    unsigned char *dst_u = (unsigned char *)save_YU12 + width * height;
    unsigned char *dst_v = (unsigned char *)dst_u + (width * height/4);

    if(save_YU12 == NULL || NV21 == NULL || width <= 0 || height <= 0)
    {
        printf(" NV21ToYU12 incorrect input parameter!\n");
        return -1;
    }

    //copy Y
    memcpy(save_YU12,NV21,width * height);

    //copy U
    for(int i = 1; i < width*height/2; i += 2)
    {
        *(dst_u++) = *(src_uv + i);
    }

    //copy V
    for(int j = 0; j < width*height/2; j += 2)
    {
        *(dst_v++) = *(src_uv + j);
    }

    return 0;

}


/*************************************
*NV21:Y VUVU
*YUYV:Y U Y V
**************************************/
int YUYVToNV21(void *save_NV21, void *YUYV, int width, int height)
{
    unsigned char *src_yuyv = (unsigned char*)YUYV;
    unsigned char *dst_nv21 = (unsigned char*)save_NV21;
    unsigned char *dst_nv21_v = (unsigned char*)save_NV21 + width*height;
    unsigned char *dst_nv21_u = (unsigned char*)save_NV21 + width*height+1;

    unsigned int u_index=0,v_index=0;
    int i,j;

    if(save_NV21 == NULL || YUYV == NULL || width <= 0 || height <= 0)
    {
        printf(" YUYVToNV21 incorrect input parameter!\n");
        return -1;
    }

    /* copy Y */
    for(int i=0; i<width*height; i++)
        dst_nv21[i] = src_yuyv[2*i];


    //U
    for(i=0;i<height;i++){
        if((i%2)!=0)continue;
        for(j=0;j<(width/2);j++){
            if((4*j+1)>(2*width))break;
            dst_nv21_u[u_index*2]=src_yuyv[i*2*width+4*j+1];
            u_index++;
        }
    }

    //V
    for(i=0;i<height;i++){
        if((i%2)==0)continue;
        for(j=0;j<(width/2);j++){
            if((4*j+3)>(2*width))break;
            dst_nv21_v[2*v_index]=src_yuyv[i*2*width+4*j+3];
            v_index++;
        }
    }

    return 0;
}

int saveYUV_NV21(char *path, void *NV21, int width, int height)
{
    FILE *fp_y = NULL;
    FILE *fp_u = NULL;
    FILE *fp_v = NULL;
    unsigned char path_y[32];
    unsigned char path_u[32];
    unsigned char path_v[32];
    unsigned char *src_uv = (unsigned char *)NV21 + width * height;

    if(path == NULL || NV21 == NULL || width <= 0 || height <= 0)
    {
        printf(" saveYUV_NV21 incorrect input parameter!\n");
        return -1;
    }

    sprintf(path_y,"%s/%s", path, "source_y.yuv");
    sprintf(path_u,"%s/%s", path, "source_u.yuv");
    sprintf(path_v,"%s/%s", path, "source_v.yuv");

    /* Y */
    fp_y = fopen(path_y,"wb+");
    if(fp_y == NULL)
    {
        printf("open %s failed\n",path_y);
        return -1;
    }

    fwrite(NV21,width*height,1,fp_y);
    fclose(fp_y);

    /* U V */
    fp_u = fopen(path_u,"wb+");
    fp_v = fopen(path_v,"wb+");
    if(fp_u==NULL || fp_v==NULL)
    {
        printf(" open u/v file failed\n");
        if(fp_u != NULL)
            fclose(fp_u);
        if(fp_v != NULL)
            fclose(fp_v);

        return -1;
    }

    /* U */
    for(int i = 1; i < width*height/2; i += 2)
    {
        fwrite(src_uv+i, 1, 1, fp_u);
    }

    //copy V
    for(int j = 0; j < width*height/2; j += 2)
    {
        fwrite(src_uv+j, 1, 1, fp_v);
    }

    fclose(fp_u);
    fclose(fp_v);

    return 0;
}

int saveY_UV_NV21(char *path, void *NV21, int width, int height)
{
    FILE *fp_y = NULL;
    FILE *fp_uv = NULL;
    unsigned char path_y[32];
    unsigned char path_uv[32];
    unsigned char *src_uv = (unsigned char *)NV21 + width * height;

    if(path == NULL || NV21 == NULL || width <= 0 || height <= 0)
    {
        printf(" saveY_UV_NV21 incorrect input parameter!\n");
        return -1;
    }

    sprintf(path_y,"%s/%s", path, "source_y.bin");
    sprintf(path_uv,"%s/%s", path, "source_uv.bin");

    /* Y */
    fp_y = fopen(path_y,"wb+");
    if(fp_y == NULL)
    {
        printf("open %s failed\n",path_y);
        return -1;
    }

    fwrite(NV21,width*height,1,fp_y);
    fclose(fp_y);

    /* UV */
    fp_uv = fopen(path_uv,"wb+");
    if(fp_uv == NULL)
    {
        printf(" open uv file failed\n");
        return -1;
    }

    fwrite(src_uv, width*height/2, 1, fp_uv);
    fclose(fp_uv);

    return 0;
}

/*************************************************************************
 *RGB low byte is B
 ************************************************************************/
int RGB24ToRGB555(void *RGB555,void *RGB24,int width,int height)
{
    unsigned short *dst_555 = (unsigned short *)RGB555;
    unsigned char * src_24 = (unsigned char *)RGB24;
    int len = width*height;

    if(src_24 == NULL || dst_555 == NULL || width <= 0 || height <= 0)
    {
        printf(" RGB24ToRGB555 incorrect input parameter!\n");
        return -1;
    }

    for(int i=0;i<len;i++)
    {
        dst_555[i] = ( ((src_24[3*i+2]>>3)<<10) | ((src_24[3*i+1]>>3)<<5) | (src_24[3*i]>>3) );
    }

    return 0;
}

/*************************************************************************
 *RGB low byte is B
 ************************************************************************/
int RGB24ToRGB565(void *RGB565,void *RGB24,int width,int height)
{
    unsigned short *dst_565 = (unsigned short *)RGB565;
    unsigned char * src_24 = (unsigned char *)RGB24;
    int len = width*height;

    if(src_24 == NULL || dst_565 == NULL || width <= 0 || height <= 0)
    {
        printf(" RGB24ToRGB565 incorrect input parameter!\n");
        return -1;
    }

    for(int i=0;i<len;i++)
    {
        dst_565[i] = ( ((src_24[3*i+2]>>3)<<11) | ((src_24[3*i+1]>>2)<<5) | (src_24[3*i]>>3) );
    }

    return 0;
}

/******************************************************
 *YUV422：Y：U：V=2:1:1
 *RGB24 ：B G R
******************************************************/
int YUV422PToRGB24(void *RGB24,void *YUV422P,int width,int height)
{
    unsigned char *src_y = (unsigned char *)YUV422P;
    unsigned char *src_u = (unsigned char *)YUV422P + width * height;
    unsigned char *src_v = (unsigned char *)YUV422P + width * height * 3 / 2;

    unsigned char *dst_RGB = (unsigned char *)RGB24;

    int temp[3];

    if(RGB24 == NULL || YUV422P == NULL || width <= 0 || height <= 0)
    {
        printf(" YUV422PToRGB24 incorrect input parameter!\n");
        return -1;
    }

    for(int y = 0;y < height;y ++)
    {
        for(int x = 0;x < width;x ++)
        {
            int Y = y*width + x;
            int U = Y >> 1;
            int V = U;

            temp[0] = src_y[Y] + ((7289 * src_u[U])>>12) - 228;  //b
            temp[1] = src_y[Y] - ((1415 * src_u[U])>>12) - ((2936 * src_v[V])>>12) + 136;  //g
            temp[2] = src_y[Y] + ((5765 * src_v[V])>>12) - 180;  //r

            dst_RGB[3*Y] = (temp[0]<0? 0: temp[0]>255? 255: temp[0]);
            dst_RGB[3*Y+1] = (temp[1]<0? 0: temp[1]>255? 255: temp[1]);
            dst_RGB[3*Y+2] = (temp[2]<0? 0: temp[2]>255? 255: temp[2]);
        }
    }

    return 0;
}

/******************************************************
*YUV420:Y U V
******************************************************/
int YUV420ToRGB24(void *RGB24,void *YUV420,int width,int height)
{
    unsigned char *src_y = (unsigned char *)YUV420;
    unsigned char *src_u = (unsigned char *)YUV420 + width * height;
    unsigned char *src_v = (unsigned char *)YUV420 + width * height * 5 / 4;

    unsigned char *dst_RGB = (unsigned char *)RGB24;

    int temp[3];

    if(RGB24 == NULL || YUV420 == NULL || width <= 0 || height <= 0)
    {
        printf(" YUV420ToRGB24 incorrect input parameter!\n");
        return -1;
    }

    for(int y = 0;y < height;y ++)
    {
        for(int x = 0;x < width;x ++)
        {
            int Y = y*width + x;
            int U = (y >> 1)*(width >> 1) + (x >> 1);
            int V = U;

            temp[0] = src_y[Y] + ((7289 * src_u[U])>>12) - 228;  //b
            temp[1] = src_y[Y] - ((1415 * src_u[U])>>12) - ((2936 * src_v[V])>>12) + 136;  //g
            temp[2] = src_y[Y] + ((5765 * src_v[V])>>12) - 180;  //r

            dst_RGB[3*Y] = (temp[0]<0? 0: temp[0]>255? 255: temp[0]);
            dst_RGB[3*Y+1] = (temp[1]<0? 0: temp[1]>255? 255: temp[1]);
            dst_RGB[3*Y+2] = (temp[2]<0? 0: temp[2]>255? 255: temp[2]);
        }
    }

    return 0;
}

/******************************************************
*YVU420:Y V U
******************************************************/
int YVU420ToRGB24(void *RGB24,void *YVU420,int width,int height)
{
    unsigned char *src_y = (unsigned char *)YVU420;
    unsigned char *src_u = (unsigned char *)YVU420 + width * height* 5 / 4;
    unsigned char *src_v = (unsigned char *)YVU420 + width * height;

    unsigned char *dst_RGB = (unsigned char *)RGB24;

    int temp[3];

    if(RGB24 == NULL || YVU420 == NULL || width <= 0 || height <= 0)
    {
        printf(" YVU420ToRGB24 incorrect input parameter!\n");
        return -1;
    }

    for(int y = 0;y < height;y ++)
    {
        for(int x = 0;x < width;x ++)
        {
            int Y = y*width + x;
            int U = (y >> 1)*(width >> 1) + (x >> 1);
            int V = U;

            temp[0] = src_y[Y] + ((7289 * src_u[U])>>12) - 228;  //b
            temp[1] = src_y[Y] - ((1415 * src_u[U])>>12) - ((2936 * src_v[V])>>12) + 136;  //g
            temp[2] = src_y[Y] + ((5765 * src_v[V])>>12) - 180;  //r

            dst_RGB[3*Y] = (temp[0]<0? 0: temp[0]>255? 255: temp[0]);
            dst_RGB[3*Y+1] = (temp[1]<0? 0: temp[1]>255? 255: temp[1]);
            dst_RGB[3*Y+2] = (temp[2]<0? 0: temp[2]>255? 255: temp[2]);
        }
    }

    return 0;

}

int YUYVToRGB24(void *RGB24,void *YUYV,int width,int height)
{
    unsigned char *src_y = (unsigned char *)YUYV;
    unsigned char *src_u = (unsigned char *)YUYV + 1;
    unsigned char *src_v = (unsigned char *)YUYV + 3;

    unsigned char *dst_RGB = (unsigned char *)RGB24;

    int temp[3];

    if(RGB24 == NULL || YUYV == NULL || width <= 0 || height <= 0)
    {
        printf(" YUYVToRGB24 incorrect input parameter!\n");
        return -1;
    }

    for(int i = 0;i < width*height;i ++)
    {
            int Y = 2 * i;
            int U = (i >> 1) * 4;
            int V = U;

            temp[0] = src_y[Y] + ((7289 * src_u[U])>>12) - 228;  //b
            temp[1] = src_y[Y] - ((1415 * src_u[U])>>12) - ((2936 * src_v[V])>>12) + 136;  //g
            temp[2] = src_y[Y] + ((5765 * src_v[V])>>12) - 180;  //r

            dst_RGB[3*i] = (temp[0]<0? 0: temp[0]>255? 255: temp[0]);
            dst_RGB[3*i+1] = (temp[1]<0? 0: temp[1]>255? 255: temp[1]);
            dst_RGB[3*i+2] = (temp[2]<0? 0: temp[2]>255? 255: temp[2]);

    }
    return 0;

}

int YVYUToRGB24(void *RGB24,void *YVYU,int width,int height)
{
    const unsigned char *src_y = (unsigned char *)YVYU;
    const unsigned char *src_u = (unsigned char *)YVYU + 3;
    const unsigned char *src_v = (unsigned char *)YVYU + 1;

    unsigned char *dst_RGB = (unsigned char *)RGB24;

    int temp[3];

    if(RGB24 == NULL || YVYU == NULL || width <= 0 || height <= 0)
    {
        printf(" YVYUToRGB24 incorrect input parameter!\n");
        return -1;
    }

    for(int i = 0;i < width*height;i ++)
    {
            int Y = 2 * i;
            int U = (i >> 1) * 4;
            int V = U;

            temp[0] = src_y[Y] + ((7289 * src_u[U])>>12) - 228;  //b
            temp[1] = src_y[Y] - ((1415 * src_u[U])>>12) - ((2936 * src_v[V])>>12) + 136;  //g
            temp[2] = src_y[Y] + ((5765 * src_v[V])>>12) - 180;  //r

            dst_RGB[3*i] = (temp[0]<0? 0: temp[0]>255? 255: temp[0]);
            dst_RGB[3*i+1] = (temp[1]<0? 0: temp[1]>255? 255: temp[1]);
            dst_RGB[3*i+2] = (temp[2]<0? 0: temp[2]>255? 255: temp[2]);

    }
    return 0;
}

int UYVYToRGB24(void *RGB24,void *UYVY,int width,int height)
{
    unsigned char *src_y = (unsigned char *)UYVY + 1;
    unsigned char *src_u = (unsigned char *)UYVY;
    unsigned char *src_v = (unsigned char *)UYVY + 2;

    unsigned char *dst_RGB = (unsigned char *)RGB24;

    int temp[3];

    if(RGB24 == NULL || UYVY == NULL || width <= 0 || height <= 0)
    {
        printf(" UYVYToRGB24 incorrect input parameter!\n");
        return -1;
    }

    for(int i = 0;i < width*height;i ++)
    {
            int Y = 2 * i;
            int U = (i >> 1) * 4;
            int V = U;

            temp[0] = src_y[Y] + ((7289 * src_u[U])>>12) - 228;  //b
            temp[1] = src_y[Y] - ((1415 * src_u[U])>>12) - ((2936 * src_v[V])>>12) + 136;  //g
            temp[2] = src_y[Y] + ((5765 * src_v[V])>>12) - 180;  //r

            dst_RGB[3*i] = (temp[0]<0? 0: temp[0]>255? 255: temp[0]);
            dst_RGB[3*i+1] = (temp[1]<0? 0: temp[1]>255? 255: temp[1]);
            dst_RGB[3*i+2] = (temp[2]<0? 0: temp[2]>255? 255: temp[2]);

    }
    return 0;
}

int VYUYToRGB24(void *RGB24,void *VYUY,int width,int height)
{
    unsigned char *src_y = (unsigned char *)VYUY + 1;
    unsigned char *src_u = (unsigned char *)VYUY + 2;
    unsigned char *src_v = (unsigned char *)VYUY;

    unsigned char *dst_RGB = (unsigned char *)RGB24;

    int temp[3];

    if(RGB24 == NULL || VYUY == NULL || width <= 0 || height <= 0)
    {
        printf(" VYUYToRGB24 incorrect input parameter!\n");
        return -1;
    }

    for(int i = 0;i < width*height;i ++)
    {
            int Y = 2 * i;
            int U = (i >> 1) * 4;
            int V = U;

            temp[0] = src_y[Y] + ((7289 * src_u[U])>>12) - 228;  //b
            temp[1] = src_y[Y] - ((1415 * src_u[U])>>12) - ((2936 * src_v[V])>>12) + 136;  //g
            temp[2] = src_y[Y] + ((5765 * src_v[V])>>12) - 180;  //r

            dst_RGB[3*i] = (temp[0]<0? 0: temp[0]>255? 255: temp[0]);
            dst_RGB[3*i+1] = (temp[1]<0? 0: temp[1]>255? 255: temp[1]);
            dst_RGB[3*i+2] = (temp[2]<0? 0: temp[2]>255? 255: temp[2]);

    }
    return 0;
}

int NV12ToRGB24(void *RGB24,void *NV12,int width,int height)
{
    unsigned char *src_y = (unsigned char *)NV12;
    unsigned char *src_v = (unsigned char *)NV12 + width * height + 1;
    unsigned char *src_u = (unsigned char *)NV12 + width * height;

    unsigned char *dst_RGB = (unsigned char *)RGB24;

    int temp[3];

    if(RGB24 == NULL || NV12 == NULL || width <= 0 || height <= 0)
    {
        printf(" NV12ToRGB24 incorrect input parameter!\n");
        return -1;
    }

    for(int y = 0;y < height;y ++)
    {
        for(int x = 0;x < width;x ++)
        {
            int Y = y*width + x;
            int U = ( (y >> 1)*(width >>1) + (x >> 1) ) * 2;
            int V = U;

            temp[0] = src_y[Y] + ((7289 * src_u[U])>>12) - 228;  //b
            temp[1] = src_y[Y] - ((1415 * src_u[U])>>12) - ((2936 * src_v[V])>>12) + 136;  //g
            temp[2] = src_y[Y] + ((5765 * src_v[V])>>12) - 180;  //r

            dst_RGB[3*Y] = (temp[0]<0? 0: temp[0]>255? 255: temp[0]);
            dst_RGB[3*Y+1] = (temp[1]<0? 0: temp[1]>255? 255: temp[1]);
            dst_RGB[3*Y+2] = (temp[2]<0? 0: temp[2]>255? 255: temp[2]);
        }
    }

    return 0;

}

int NV21ToRGB24(void *RGB24,void *NV21,int width,int height)
{
    unsigned char *src_y = (unsigned char *)NV21;
    unsigned char *src_v = (unsigned char *)NV21 + width * height;
    unsigned char *src_u = (unsigned char *)NV21 + width * height + 1;

    unsigned char *dst_RGB = (unsigned char *)RGB24;

    int temp[3];

    if(RGB24 == NULL || NV21 == NULL || width <= 0 || height <= 0)
    {
        printf(" NV21ToRGB24 incorrect input parameter!\n");
        return -1;
    }

    for(int y = 0;y < height;y ++)
    {
        for(int x = 0;x < width;x ++)
        {
            int Y = y*width + x;
            int U = ( (y >> 1)*(width >>1) + (x >> 1) )<<1;
            int V = U;

            temp[0] = src_y[Y] + ((7289 * src_u[U])>>12) - 228;  //b
            temp[1] = src_y[Y] - ((1415 * src_u[U])>>12) - ((2936 * src_v[V])>>12) + 136;  //g
            temp[2] = src_y[Y] + ((5765 * src_v[V])>>12) - 180;  //r

            dst_RGB[3*Y] = (temp[0]<0? 0: temp[0]>255? 255: temp[0]);
            dst_RGB[3*Y+1] = (temp[1]<0? 0: temp[1]>255? 255: temp[1]);
            dst_RGB[3*Y+2] = (temp[2]<0? 0: temp[2]>255? 255: temp[2]);
        }
    }

    return 0;

}

/******************************************************************
*YV12:Y U V
*******************************************************************/
int YV12ToRGB24(void *RGB24,void *YV12,int width,int height)
{
    unsigned char *src_y = (unsigned char *)YV12;
    unsigned char *src_v = (unsigned char *)YV12 + width * height;
    unsigned char *src_u = (unsigned char *)YV12 + width * height * 5 / 4;

    unsigned char *dst_RGB = (unsigned char *)RGB24;

    int temp[3];

    if(RGB24 == NULL || YV12 == NULL || width <= 0 || height <= 0)
    {
        printf(" YV12ToRGB24 incorrect input parameter!\n");
        return -1;
    }

    for(int y = 0;y < height;y ++)
    {
        for(int x = 0;x < width;x ++)
        {
            int Y = y*width + x;
            int U = (y >> 1)*(width >>1) + (x >> 1);
            int V = U;

            temp[0] = src_y[Y] + ((7289 * src_u[U])>>12) - 228;  //b
            temp[1] = src_y[Y] - ((1415 * src_u[U])>>12) - ((2936 * src_v[V])>>12) + 136;  //g
            temp[2] = src_y[Y] + ((5765 * src_v[V])>>12) - 180;  //r

            dst_RGB[3*Y] = (temp[0]<0? 0: temp[0]>255? 255: temp[0]);
            dst_RGB[3*Y+1] = (temp[1]<0? 0: temp[1]>255? 255: temp[1]);
            dst_RGB[3*Y+2] = (temp[2]<0? 0: temp[2]>255? 255: temp[2]);

        }
    }

    return 0;

}

/******************************************************
 *NV16 belongs to YUV422SP，Y UV UV ,Y：U=2:1
******************************************************/
int NV16ToRGB24(void *RGB24,void *NV16,int width,int height)
{
    unsigned char *src_y = (unsigned char *)NV16;
    unsigned char *src_u = (unsigned char *)NV16 + width * height;
    unsigned char *src_v = (unsigned char *)NV16 + width * height + 1;

    unsigned char *dst_RGB = (unsigned char *)RGB24;

    int temp[3];

    if(RGB24 == NULL || NV16 == NULL || width <= 0 || height <= 0)
    {
        printf(" NV16ToRGB24 incorrect input parameter!\n");
        return -1;
    }

    for(int y = 0;y < height;y ++)
    {
        for(int x = 0;x < width;x ++)
        {
            int Y = y*width + x;
            int U = (Y >> 1) * 2; /* Divided by 2 and then multiplied by 2 is necessary */
            int V = U;

            temp[0] = src_y[Y] + ((7289 * src_u[U])>>12) - 228;  //b
            temp[1] = src_y[Y] - ((1415 * src_u[U])>>12) - ((2936 * src_v[V])>>12) + 136;  //g
            temp[2] = src_y[Y] + ((5765 * src_v[V])>>12) - 180;  //r

            dst_RGB[3*Y] = (temp[0]<0? 0: temp[0]>255? 255: temp[0]);
            dst_RGB[3*Y+1] = (temp[1]<0? 0: temp[1]>255? 255: temp[1]);
            dst_RGB[3*Y+2] = (temp[2]<0? 0: temp[2]>255? 255: temp[2]);
        }
    }

    return 0;

}

/******************************************************
 *NV61 belongs to YUV422SP，Y VU VU ,Y：U=2:1
******************************************************/
int NV61ToRGB24(void *RGB24,void *NV61,int width,int height)
{
    unsigned char *src_y = (unsigned char *)NV61;
    unsigned char *src_u = (unsigned char *)NV61 + width * height + 1;
    unsigned char *src_v = (unsigned char *)NV61 + width * height;

    unsigned char *dst_RGB = (unsigned char *)RGB24;

    int temp[3];

    if(RGB24 == NULL || NV61 == NULL || width <= 0 || height <= 0)
    {
        printf(" NV61ToRGB24 incorrect input parameter!\n");
        return -1;
    }

    for(int y = 0;y < height;y ++)
    {
        for(int x = 0;x < width;x ++)
        {
            int Y = y*width + x;
            int U = (Y >> 1) * 2; /* Divided by 2 and then multiplied by 2 is necessary */
            int V = U;

            temp[0] = src_y[Y] + ((7289 * src_u[U])>>12) - 228;  //b
            temp[1] = src_y[Y] - ((1415 * src_u[U])>>12) - ((2936 * src_v[V])>>12) + 136;  //g
            temp[2] = src_y[Y] + ((5765 * src_v[V])>>12) - 180;  //r

            dst_RGB[3*Y] = (temp[0]<0? 0: temp[0]>255? 255: temp[0]);
            dst_RGB[3*Y+1] = (temp[1]<0? 0: temp[1]>255? 255: temp[1]);
            dst_RGB[3*Y+2] = (temp[2]<0? 0: temp[2]>255? 255: temp[2]);
        }
    }

    return 0;

}

int YUVToRGBfile(const char *rgb_path,char *yuv_data,ConverFunc func,int width,int height)
{
    unsigned short *rgb_555 = NULL;
    unsigned char *rgb_24 = NULL;
    unsigned char path[128];
    FILE *fp = NULL;

    if(rgb_path == NULL || yuv_data == NULL || func == NULL || width <= 0 || height <= 0)
    {
        printf(" YUVToBMP16 incorrect input parameter!\n");
        return -1;
    }

    rgb_24 = (unsigned char *)malloc(width*height*3);
    rgb_555 = (unsigned short *)malloc(width*height*2);
    if(rgb_555 == NULL || rgb_24 == NULL)
    {
       printf(" YUVToRGB alloc failed!\n");
        if(rgb_24)
            free(rgb_24);
        if(rgb_555)
            free(rgb_555);
       return -1;
    }

    /********* rgb24 *********/
    memset(path, 0, sizeof(path));
    sprintf(path, "%s/rgb888.rgb", rgb_path);

    func(rgb_24,yuv_data,width,height);

    /* Create bmp file */
    fp = fopen(path,"wb+");
    if(!fp)
    {
        printf(" Create bmp file %s failed!\n", path);
        if(rgb_24)
            free(rgb_24);
        if(rgb_555)
            free(rgb_555);
        return -1;
    }

    fwrite(rgb_24,width*height*3,1,fp);

    fclose(fp);

    /********* rgb555 *********/
    memset(path, 0, sizeof(path));
    sprintf(path, "%s/rgb555.rgb", rgb_path);

    RGB24ToRGB555(rgb_555,rgb_24,width,height);

    /* Create bmp file */
    fp = fopen(path,"wb+");
    if(!fp)
    {
        free(rgb_24);
        free(rgb_555);
        printf(" Create bmp file %s failed!\n", path);
        return -1;
    }

    fwrite(rgb_555,width*height*2,1,fp);

    fclose(fp);

    /********* rgb565 *********/
    memset(path, 0, sizeof(path));
    sprintf(path, "%s/rgb565.rgb", rgb_path);

    memset(rgb_555,0,width*height*2);
    RGB24ToRGB565(rgb_555,rgb_24,width,height);

    /* Create bmp file */
    fp = fopen(path,"wb+");
    if(!fp)
    {
        free(rgb_24);
        free(rgb_555);
        printf(" Create bmp file %s failed!\n", path);
        return -1;
    }

    fwrite(rgb_555,width*height*2,1,fp);

    fclose(fp);

    /* free */
    free(rgb_555);
    free(rgb_24);

    return 0;
}

int YUVToBMP555(const char *bmp_path,char *yuv_data,ConverFunc func,int width,int height)

{
    unsigned short *rgb_555 = NULL;
    unsigned char *rgb_24 = NULL;
    FILE *fp = NULL;

    BITMAPFILEHEADER BmpFileHeader;
    BITMAPINFOHEADER BmpInfoHeader;

    if(bmp_path == NULL || yuv_data == NULL || func == NULL || width <= 0 || height <= 0)
    {
        printf(" YUVToBMP555 incorrect input parameter!\n");
        return -1;
    }

   /* Fill header information */
   BmpFileHeader.bfType = 0x4d42;
   BmpFileHeader.bfSize = width*height*2 + sizeof(BmpFileHeader) + sizeof(BmpInfoHeader);
   BmpFileHeader.bfReserved1 = 0;
   BmpFileHeader.bfReserved2 = 0;
   BmpFileHeader.bfOffBits = sizeof(BmpFileHeader) + sizeof(BmpInfoHeader);

   BmpInfoHeader.biSize = sizeof(BmpInfoHeader);
   BmpInfoHeader.biWidth = width;
   BmpInfoHeader.biHeight = -height;
   BmpInfoHeader.biPlanes = 0x01;
   BmpInfoHeader.biBitCount = 16;
   BmpInfoHeader.biCompression = 0;
   BmpInfoHeader.biSizeImage = 0;
   //BmpInfoHeader.biXPelsPerMeter = 0;
   //BmpInfoHeader.biYPelsPerMeter = 0;
   BmpInfoHeader.biClrUsed = 0;
   BmpInfoHeader.biClrImportant = 0;

    rgb_24 = (unsigned char *)malloc(width*height*3);
    rgb_555 = (unsigned short *)malloc(width*height*2);
    if(rgb_555 == NULL || rgb_24 == NULL)
    {
       printf(" YUVToBMP55 alloc failed!\n");
        if(rgb_24)
            free(rgb_24);
        if(rgb_555)
            free(rgb_555);
       return -1;
    }

    func(rgb_24,yuv_data,width,height);
    RGB24ToRGB555(rgb_555,rgb_24,width,height);

    /* Create bmp file */
    fp = fopen(bmp_path,"wb+");
    if(!fp)
    {
        free(rgb_24);
        free(rgb_555);
        printf(" Create bmp file:%s failed!\n", bmp_path);
        return -1;
    }

    fwrite(&BmpFileHeader,sizeof(BmpFileHeader),1,fp);

    fwrite(&BmpInfoHeader,sizeof(BmpInfoHeader),1,fp);

    fwrite(rgb_555,width*height*2,1,fp);

    free(rgb_555);
    free(rgb_24);

    fclose(fp);

    return 0;
}

int YUVToBMP565(const char *bmp_path,char *yuv_data,ConverFunc func,int width,int height)

{
    unsigned short *rgb_565 = NULL;
    unsigned char *rgb_24 = NULL;
    FILE *fp = NULL;

    BITMAPFILEHEADER BmpFileHeader;
    BITMAPINFOHEADER BmpInfoHeader;
    unsigned int rgb[3];

    if(bmp_path == NULL || yuv_data == NULL || func == NULL || width <= 0 || height <= 0)
    {
        printf(" YUVToBMP565 incorrect input parameter!\n");
        return -1;
    }

   /* Fill header information */
   BmpFileHeader.bfType = 0x4d42;
   BmpFileHeader.bfSize = width*height*2 + sizeof(BmpFileHeader) + sizeof(BmpInfoHeader) + sizeof(rgb);
   BmpFileHeader.bfReserved1 = 0;
   BmpFileHeader.bfReserved2 = 0;
   BmpFileHeader.bfOffBits = sizeof(BmpFileHeader) + sizeof(BmpInfoHeader) + sizeof(rgb);

   BmpInfoHeader.biSize = sizeof(BmpInfoHeader);
   BmpInfoHeader.biWidth = width;
   BmpInfoHeader.biHeight = -height;
   BmpInfoHeader.biPlanes = 0x01;
   BmpInfoHeader.biBitCount = 16;
   BmpInfoHeader.biCompression = 3;
   BmpInfoHeader.biSizeImage = (BmpFileHeader.bfSize + 3) & ~3;
   //BmpInfoHeader.biXPelsPerMeter = 0;
   //BmpInfoHeader.biYPelsPerMeter = 0;
   BmpInfoHeader.biClrUsed = 0;
   BmpInfoHeader.biClrImportant = 0;

    rgb[0] = 0xf800;
    rgb[1] = 0x07e0;
    rgb[2] = 0x001f;

    rgb_24 = (unsigned char *)malloc(width*height*3);
    rgb_565 = (unsigned short *)malloc(width*height*2);
    if(rgb_565 == NULL || rgb_24 == NULL)
    {
       printf(" YUVToBMP565 alloc failed!\n");
       if(rgb_24)
           free(rgb_24);
       if(rgb_565)
           free(rgb_565);

       return -1;
    }

    func(rgb_24,yuv_data,width,height);
    RGB24ToRGB565(rgb_565,rgb_24,width,height);

    /* Create bmp file */
    fp = fopen(bmp_path,"wb+");
    if(!fp)
    {
        printf(" Create bmp file:%s failed!\n", bmp_path);
        if(rgb_24)
            free(rgb_24);
        if(rgb_565)
            free(rgb_565);
        return -1;
    }

    fwrite(&BmpFileHeader,sizeof(BmpFileHeader),1,fp);

    fwrite(&BmpInfoHeader,sizeof(BmpInfoHeader),1,fp);

    fwrite(rgb,sizeof(rgb),1,fp);

    fwrite(rgb_565,width*height*2,1,fp);

    free(rgb_565);
    free(rgb_24);

    fclose(fp);

    return 0;
}

int YUVToBMP(const char *bmp_path,char *yuv_data,ConverFunc func,int width,int height)
{
    unsigned char *rgb_24 = NULL;
    FILE *fp = NULL;

    BITMAPFILEHEADER BmpFileHeader;
    BITMAPINFOHEADER BmpInfoHeader;

    if(bmp_path == NULL || yuv_data == NULL || func == NULL || width <= 0 || height <= 0)
    {
        printf(" YUVToBMP incorrect input parameter!\n");
        return -1;
    }

   /* Fill header information */
   BmpFileHeader.bfType = 0x4d42;
   BmpFileHeader.bfSize = width*height*3 + sizeof(BmpFileHeader) + sizeof(BmpInfoHeader);
   BmpFileHeader.bfReserved1 = 0;
   BmpFileHeader.bfReserved2 = 0;
   BmpFileHeader.bfOffBits = sizeof(BmpFileHeader) + sizeof(BmpInfoHeader);

   BmpInfoHeader.biSize = sizeof(BmpInfoHeader);
   BmpInfoHeader.biWidth = width;
   BmpInfoHeader.biHeight = -height;
   BmpInfoHeader.biPlanes = 0x01;
   BmpInfoHeader.biBitCount = 24;
   BmpInfoHeader.biCompression = 0;
   BmpInfoHeader.biSizeImage = 0;
   //BmpInfoHeader.biXPelsPerMeter = 0;
   //BmpInfoHeader.biYPelsPerMeter = 0;
   BmpInfoHeader.biClrUsed = 0;
   BmpInfoHeader.biClrImportant = 0;

    rgb_24 = (unsigned char *)malloc(width*height*3);
    if(rgb_24 == NULL)
    {
       printf(" YUVToBMP alloc failed!\n");
       return -1;
    }

    func(rgb_24,yuv_data,width,height);

    /* Create bmp file */
    fp = fopen(bmp_path,"wb+");
    if(!fp)
    {
        free(rgb_24);
        printf(" Create bmp file:%s faled!\n", bmp_path);
        return -1;
    }

    fwrite(&BmpFileHeader,sizeof(BmpFileHeader),1,fp);

    fwrite(&BmpInfoHeader,sizeof(BmpInfoHeader),1,fp);

    fwrite(rgb_24,width*height*3,1,fp);

    free(rgb_24);

    fclose(fp);

    return 0;
}
