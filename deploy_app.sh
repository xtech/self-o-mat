#!/bin/sh


NAME=app
BINARY=self_o_mat
FILES="$BINARY settings/* assets/* libs/* firmware.hex version app/*"
TAR=update.tar
BIN_DIR=$2

clean() {
    echo "Cleaning in $BIN_DIR"

    # to to build dir
    cd $BIN_DIR

    # clean libs
    rm -rf ./libs

    # clean settings and assets
    rm -rf ./assets
    rm -rf ./settings
    rm -rf ./firmware.hex
    rm -rf ./version
    rm -rf ./app
    rm -rf $TAR
}

echo "Packing files in $1"



clean


cd $2
rm -f selfomat.update


# get the settings and assets from src dir into the build dir
cp -r $1/assets .
cp -r $1/settings .
cp $1/firmware.hex .
cp -r $1/web/www ./app

# write version info
echo -n "v_`date +'%s'`" > version
