export PATH=$PATH:/home/leif/em/cc/gcc-7.3.0-glibc-2.26-linux-3.18.118-gnueabi/bin/
export CC=arm-arm926ejs-linux-gnueabi-gcc
./configure --prefix=/home/leif/em/zlib
make 
make install
