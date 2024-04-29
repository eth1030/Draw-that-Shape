#!/bin/sh
### BEGIN INIT INFO
# Provides:          run_shape
# Required-Start:    $all
# Required-Stop:
# Default-Start:     2 3 4 5
# Default-Stop:
# Short-Description: starts myshape...
### END INIT INFO
mknod /dev/myshape c 61 0
insmod /home/debian/myshape.ko
/home/debian/test &
