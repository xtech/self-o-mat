# The self-o-mat Project

<a href="https://www.youtube.com/watch?v=fj-s72GkQOo"/><img src="https://github.com/xtech/self-o-mat/raw/master/images/self-o-mat-youtube.png" /></a>

The self-o-mat project started with the desire to build an __inexpensive photobooth__ for personal use which is able to produce __high quality images__ using __DSLR cameras__. Since we wanted to build a solution for personal use, the project needed to run on inexpensive hardware and work with our DSLR cameras. Sadly we did not find a project which is able to deliver this - so the self-o-mat project was born.

Since then, a lot has changed: We have found that many of you share the same view: __Building a photobooth should be easy!__ Therefore we have created a photobooth kit which is now available at: [https://self-o-mat.de](https://self-o-mat.de). You can of course just use the open-source software and build your own full-featured DIY-photobox __for free__.

## Basics
_"A Raspberry Pi&reg; and a DSLR camera should be all that's necessary to build a high quality photobooth!"_ - This was the basic idea when we started designing the self-o-mat and it hasn't changed since:
__A DSLR camera and a Raspberry Pi&reg; is all you need to get started!__


<img src="https://github.com/xtech/self-o-mat/raw/master/images/banner_1.png" height="100" /><img src="https://github.com/xtech/self-o-mat/raw/master/images/banner_2.png" height="100" /><a href="https://www.raspberrypi.org"/><img src="https://github.com/xtech/self-o-mat/raw/master/images/banner_3.png" height="100" /></a><img src="https://github.com/xtech/self-o-mat/raw/master/images/banner_4.png" height="100" /><img src="https://github.com/xtech/self-o-mat/raw/master/images/banner_5.png" height="100" />


## Features
Currently the self-o-mat software is a basic but stable and reliable software with the following features:
- __Live Preview__ - get a fluid live preview of the current shot
- __Full quality images__ - even with a Raspberry Pi&reg; we're able to process large JPEG files and RAW images
- __High performance__ - no loading times or lags
- __Support for most DSL cameras and webcams__ - Just connect your existing camera
- __Live Image Printing__ - Connect any CUPS compatible printer and print each image directly. Great for giveaways at weddings!
- __Live Template Overlay__ - Want to use self-o-mat commerially? Brand your images with your own logo!
- __App Inclusive__ - update camera settings on the go. Just connect your smartphone and change settings live!

## Supported Cameras
- Cameras
  - Canon EOS 450D (no autofocus)
  - Canon EOS 5D Mark III
  - Canon EOS 600D
  - Nikon D7500
  - Nikon D7200
- Printers
  - Canon Selphy CP 1200
  - Canon Selphy CP 1300
  - Mitsubishi CP-D70DW

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

4. Tell your system which type of self-o-mat you are running. For development config use 'dev' by running:
   ``sudo bash -c 'echo "dev" > /opt/.selfomat.type'``

5. Edit settings/dev.json to match your setup. It should work with an Arduino Nano by default.

6. Run the self-o-mat photo-booth software:
```
#setup your installation
cd <source dir>/build
./self_o_mat.app
```

# Now what?
So now you're running the self-o-mat software and your photoboot is technically done. Just add a trigger mechanism and put it in a box. For better image quality, add a flash to your box. If you want to get started quickly, we have created some components for you:

## The Controller
You'll need a controller to trigger the image capture. Basically you have two options: The self-o-mat controller or a DIY solution.

### The self-o-mat Controller Board
<img src="https://github.com/xtech/self-o-mat/raw/master/images/RPi_Hat.png" height="450" />

The __self-o-mat controller board__ is a simple add-on to the Raspberry Pi&reg; which is able to control the photobooth for you:
- Connect any button to the board to __trigger the capture__. You can use an __arcade button__, a __large buzzer__, a __coin slot__ or any other switch as trigger.
- Tell the user the __exact moment__ when the photo is taken: Just connect an __Neo Pixel LED ring or strip__ for a count down animation before the trigger.
- The __LED ring flash__ connector can be used to connect our LED flash. It is triggered to illuminate pictures with a __soft__ light for perfect pictures. The flash __brightness and duration__ can be set through the self-o-mat software.
- Connect a 7-15V DC power supply to the power input of the board and it will __power the Raspberry Pi&reg; for you__. The booard has an __Arduino compatible processor__ which is able to switch the power. So the Raspberry Pi&reg; can shut down gracefully and turn its own power off.
- The board has __switched power outputs__. Just connect additional components which should be switched with your photobox. For example __a screen__ or an __LED flash__ could be connected. As soon as the booth is powered the outputs will be powered as well.

__You can [Get the controller board here!](https://shop.self-o-mat.de/zubehoer/28-controller-board.html)__

### The DIY Solution
Self-o-mat is triggered by sending commands to the serial console. In order to trigger the software, use any Arduino board and write the trigger signals to the serial port. Serial settings and protocol can be adjusted in the software.

## Flash

<img src="https://github.com/xtech/self-o-mat/raw/master/images/LED_Ring_Flash.png" height="450" />

Every photobooth should have a flash for best possible images. __If you already own a flash, you're done. Just connect it to your DSLR camera!__ If you need a customized solution which can be __configured via the self-o-mat app__ read on.

We have created a beatuiful __LED Ring flash__ which features __over 100 high CRI LEDs__ for __perfect color accuracy__. Just power the flash and connect it to the Raspberry Pi&reg; (or to our self-o-mat controller board) and the software will flash it during image capture.
__You can [Get the flash here!](https://shop.self-o-mat.de/zubehoer/32-led-flash.html)__



