#!/bin/sh

mknod /dev/myshape c 61 0
insmod /home/debian/myshape.ko
/home/debian/test &
