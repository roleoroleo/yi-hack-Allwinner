CC=arm-openwrt-linux-gcc
prefix=/home/yi-hack/
libdir=/home/yi-hack/lib
sysconfdir=/home/yi-hack/etc
USER_CFLAGS=-Os -mcpu=cortex-a7 -mfpu=neon-vfpv4 -I/opt/yi/toolchain-sunxi-musl/toolchain/include -L/opt/yi/toolchain-sunxi-musl/toolchain/lib
USER_LDFLAGS=
AR=arm-openwrt-linux-ar
RANLIB=arm-openwrt-linux-ranlib
