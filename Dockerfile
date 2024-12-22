FROM debian:12 AS build
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update &&  \
    apt-get install --no-install-recommends -y build-essential cmake libmagick++-dev  \
    libboost-all-dev libopencv-dev libsfml-dev libcups2-dev  \
    libprotobuf-dev protobuf-compiler libusb-dev automake  \
    autoconf pkg-config autopoint gettext libtool nasm  \
    libturbojpeg0-dev libasio-dev git ca-certificates  \
    && rm -rf /var/lib/apt/lists/*

RUN cd /tmp &&  \
    git clone https://github.com/gphoto/libgphoto2.git && \
    cd libgphoto2 && \
    git checkout tags/libgphoto2-2_5_22-release && \
    autoreconf --install --symlink && \
    ./configure && \
    make -j$(nproc) && \
    make install && \
    ldconfig

COPY --link . /workspace
WORKDIR /workspace

RUN mkdir build && \
    cd build && \
    cmake .. && \
    make -j$(nproc)

FROM scratch AS export
COPY --from=build /workspace/build /