ARCH:=arm
CROSS_COMPILE:=arm-linux-gnueabihf-
ROOTDIR:=/home/eth/EC535/debian/linux
PWD:=$(shell pwd)
obj-m := myshape.o

default:
	$(MAKE) -C $(ROOTDIR) M=$(PWD) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) modules
clean:
	$(MAKE) -C $(ROOTDIR) M=$(PWD) ARCH=$(ARCH) clean

