# Clone

```git clone --recursive https://github.com/xtech/self-o-mat.git```

`--recursive` is required to clone git submodules.

# Install

1. Install the following dependencies:
```sudo apt-get install build-essential cmake libmagick++-dev libboost-all-dev libopencv-dev libsfml-dev libcups2-dev libprotobuf-dev protobuf-compiler libusb-dev automake autoconf pkg-config autopoint gettext libtool nasm```

2. Get libjpeg-turbo:
```
download from: https://sourceforge.net/projects/libjpeg-turbo/files/2.0.0/libjpeg-turbo-2.0.0.tar.gz/download
tar -xf libjpeg-turbo.tar.gz
cd libjpeg-turbo-2.0.0
mkdir build && cd build && cmake ..
make
sudo make install
```

3. Get libgphoto2:
```
git clone https://github.com/gphoto/libgphoto2.git
cd libgphoto2
git checkout tags/libgphoto2-2_5_22-release
autoreconf --install --symlink
./configure
make
sudo make install
```

4. Build self-o-mat
```
cd self-o-mat
mkdir build
cd build
cmake ..
make
```

5. Run self-o-mat
```
#setup your installation
nano settings.json
cd <source dir>
build/self_o_mat
```
