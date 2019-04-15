# Clone

```git clone --recursive https://github.com/xtech/self-o-mat.git```

`--recursive` is required to clone git submodules.

# Install

1. Install the following dependencies:
```sudo apt-get install build-essential cmake libmagick++-dev libboost-all-dev libopencv-dev libsfml-dev libcups2-dev libprotobuf-dev protobuf-compiler libusb-dev automake autoconf pkg-config autopoint gettext libtool nasm libturbojpeg0-dev```

2. Get libgphoto2:
```
git clone https://github.com/gphoto/libgphoto2.git
cd libgphoto2
git checkout tags/libgphoto2-2_5_22-release
autoreconf --install --symlink
./configure
make
sudo make install
```

3. Build self-o-mat
```
cd self-o-mat
mkdir build
cd build
cmake ..
make
```

4. Run self-o-mat
```
#setup your installation
cd <source dir>
nano settings.json
build/self_o_mat
```
