/*
 * Copyright (c) 2020 roleo.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * Reads the YUV buffer, extracts the last frame and converts it to jpg.
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <dirent.h>
#include <math.h>
#include <getopt.h>

#include "capture.h"
#include "convert2jpg.h"

#define RESOLUTION_LOW 360
#define RESOLUTION_HIGH 1080

#define W_LOW 640
#define H_LOW 360
#define W_HIGH 1936
#define H_HIGH 1096
#define W_DEST_HIGH 1920
#define H_DEST_HIGH 1080

#define W_MB 16
#define H_MB 16

#define VIDEODEV 3                     /* /dev/video3 */

int camera_dbg_en = 0;

void print_usage(char *progname)
{
    fprintf(stderr, "\nUsage: %s [-r RES] [-d]\n\n", progname);
    fprintf(stderr, "\t-r RES, --resolution RES\n");
    fprintf(stderr, "\t\tset resolution: LOW or HIGH (default HIGH)\n");
    fprintf(stderr, "\t-d, --debug\n");
    fprintf(stderr, "\t\tenable debug\n");
}

int main(int argc, char **argv)
{
    int c, ret;
    int resolution = RESOLUTION_HIGH;
//    FILE *fPtr, *fLen;
//    int fMem;
//    unsigned int ivAddr, ipAddr;
//    unsigned int size;
//    unsigned char *addr;
//    unsigned char *buffer;
    int outlen;

    int fd;
    FILE* fp;

    while (1) {
        static struct option long_options[] =
        {
            {"resolution",  required_argument, 0, 'r'},
            {"debug",  no_argument, 0, 'd'},
            {"help",  no_argument, 0, 'h'},
            {0, 0, 0, 0}
        };
        /* getopt_long stores the option index here. */
        int option_index = 0;

        c = getopt_long (argc, argv, "r:dh",
                         long_options, &option_index);

        /* Detect the end of the options. */
        if (c == -1)
            break;

        switch (c) {
        case 'r':
            if (strcasecmp("low", optarg) == 0) {
                resolution = RESOLUTION_LOW;
            } else if (strcasecmp("high", optarg) == 0) {
                resolution = RESOLUTION_HIGH;
            }
            break;

        case 'd':
            fprintf (stderr, "debug on\n");
            camera_dbg_en = 1;
            break;

        case 'h':
            print_usage(argv[0]);
            return -1;
            break;

        case '?':
            /* getopt_long already printed an error message. */
            break;

        default:
            print_usage(argv[0]);
            return -1;
        }
    }

    if (camera_dbg_en) fprintf(stderr, "Resolution: %d\n", resolution);

    char filename[] = "/tmp/snap_XXXXXX";
    mktemp(filename);

    if (resolution == RESOLUTION_LOW) {
        // Todo
    } else {
        // capture image
        ret = capture_main(filename, W_HIGH, H_HIGH, YUV_TYPE, V4L2_PIX_FMT_NV12, VIDEODEV);
        if (ret == 0)
            ret = convert2jpg_lowmemory("stdout", filename, W_HIGH, H_HIGH, W_DEST_HIGH, H_DEST_HIGH);
    }

    remove(filename);

    return ret;
}
