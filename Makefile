KDIR = /lib/modules/`uname -r`/build
EXTRA_CFLAGS = -Wall -g

obj-m        += happyb.o

kbuild:
	make -C $(KDIR) M=`pwd`

clean:
	make -C $(KDIR) M=`pwd` clean
