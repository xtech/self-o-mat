# The self-o-mat Project
The self-o-mat project started with the desire to build an __inexpensive photobooth__ for personal use which is able to produce __high quality images__ using __DSLR cameras__. Since we wanted to build a solution for personal use, the project needed to run on inexpensive hardware and work with our DSLR cameras. Sadly we did not find a project which is able to deliver this - so the self-o-mat project was born.

Since then, a lot has changed: We have found that many of you share the same view: __Building a photobooth should be easy!__ Therefore we have created a photobooth kit which is now available at: [https://self-o-mat.de](https://self-o-mat.de). You can of course just use the open-source software and build your own full-featured photobooth __for free__.

## Basics
_"A Raspberry Pi and a DSLR camera should be all that's necessary to build a high quality photobooth!"_ - This was the basic idea when we started designing the self-o-mat and it hasn't changed since:
__A DSLR camera and a Raspberry Pi is all you need to get started!__




## Features
Currently the self-o-mat software is a basic but stable and reliable software with the following features:
- __Live Preview__ - get a fluid live preview of the current shot
- __Full quality images__ - even with a Raspberry Pi we're able to process large JPEG files and RAW images
- __High performance__ - no loading times or lags
- __Support for most DSL cameras and webcams__ - Just connect your existing camera
- __Live Image Printing__ - Connect any CUPS compatible printer and print each image directly. Great for giveaways at weddings!
- __Live Template Overlay__ - Want to use self-o-mat commerially? Brand your images with your own logo!
- __App Inclusive__ - update camera settings on the go. Just connect your smartphone and change settings live!

# How to get started?
Getting started with self-o-mat is simple. You just need to clone the git repository, install the dependencies, build the software and run it.
__If you're not sure how to do this, we provide a hardware kit which includes the precompiled software fully installed. Just plug and play: [Get it here!](https://shop.self-o-mat.de/fotobox-bausatz/37-hardware-kit.html)__
## Clone

```git clone --recursive https://github.com/xtech/self-o-mat.git```

_Hint: `--recursive` is required to clone git submodules._

## Installation
Installation is simple - just follow these steps:

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
sudo ldconfig
```

3. Build self-o-mat
```
cd self-o-mat
mkdir build
cd build
cmake ..
make
```

4. Edit settings.json to match your setup

5. Run self-o-mat
```
#setup your installation
cd <source dir>
build/self_o_mat
```

## Now what?
So now you're running the self-o-mat software and your photoboot is technically done. Just add a trigger mechanism and put it in a box. For better image quality, add a flash to your box. If you want to get started quickly, we have created some components for you:

# The Controller
The __self-o-mat controller board__ is a simple add-on to the Raspberry Pi. You can just plug it on top of the Pi and connect a power supply. It will then power the Raspberry Pi and trigger the capture. Additionally it can show a count down using an Neo Pixel LED ring or strip.


<img src="https://github.com/xtech/self-o-mat/raw/master/images/RPi_Hat.png" height="450" />

