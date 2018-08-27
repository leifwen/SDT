export PATH=$PATH:/home/leif/em/cc/gcc-7.3.0-glibc-2.26-linux-3.18.118-gnueabi/bin/
./configure shared --prefix=/home/leif/em/openssl --openssldir=/home/leif/em/openssl --cross-compile-prefix=arm-arm926ejs-linux-gnueabi- linux-armv4
make 
make install
