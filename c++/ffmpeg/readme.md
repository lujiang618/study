


```
git clone https://github.com/FFmpeg/FFmpeg.git ffs

cd ffs

git checkout n8.0.0

./configure --prefix=/study/c++/ffmpeg --pkg-config-flags=--static --extra-libs='-lpthread -lm' --ld=g++ \
--enable-vaapi --enable-libx264 --enable-gpl --disable-programs --disable-vdpau --disable-x86asm \
--disable-devices --enable-debug --disable-x86asm

make && make install

cd ../

mkdir build && cd build

cmake ../
make -j8


export LD_LIBRARY_PATH=/study/c++/ffmpeg/lib:$LD_LIBRARY_PATH



```