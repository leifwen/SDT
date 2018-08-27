export PATH=$PATH:/home/leif/em/cc/gcc-7.3.0-glibc-2.26-linux-3.18.118-gnueabi/bin/
make ISSTATIC=yes ARCH=arm CROSS_COMPILE=arm-arm926ejs-linux-gnueabi-
cp sdt /home/leif/em/nfsroot/i.mx28_EasyARM280A_busybox-2.27_linux-3.18.118/root/
