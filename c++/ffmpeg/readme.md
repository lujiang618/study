


```
git clone https://github.com/FFmpeg/FFmpeg.git ffs
cd ffs
./configure --prefix=/study/c++/ffmpeg --pkg-config-flags=--static --extra-libs='-lpthread -lm' --ld=g++ \
--enable-vaapi --enable-libx264 --enable-gpl --disable-programs --disable-vdpau --disable-x86asm \
--disable-devices --enable-debug --disable-x86asm

make && make install

export LD_LIBRARY_PATH=/home/dx/data/study/github.com/lujiang618/study/c++/ffmpeg/lib:$LD_LIBRARY_PATH



```