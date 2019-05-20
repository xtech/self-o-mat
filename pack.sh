#!/bin/sh


NAME=app
BINARY=self_o_mat
FILES="$BINARY settings.json assets/* libs/* firmware.hex version app/*"
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
    rm -rf ./settings.json
    rm -rf ./firmware.hex
    rm -rf ./version
    rm -rf ./app
    rm -rf $TAR
}

echo "Packing files in $1"



clean


cd $2
rm -f selfomat.update
rm -f selfomat.tar.gz

# get the settings and assets from src dir into the build dir
cp -r $1/assets .
cp $1/settings.json .
cp $1/firmware.hex .
cp -r $1/web/www ./app

# write version info
echo -n "v_`date +'%s'`" > version

# get libs
if [ ! -d libs ]; then
    mkdir -p libs
fi
ldd $BINARY | grep "=> /" | while read a b c d; do cp "$c" libs/; done

# pack and sign
tar cfz $NAME.tar.gz $FILES
cp $NAME.tar.gz selfomat.tar.gz
gpg --quiet --batch --yes --output $NAME.sig --detach-sig $NAME.tar.gz
tar cf $TAR $NAME.tar.gz $NAME.sig
rm $NAME.tar.gz $NAME.sig
gpg --batch --yes --output selfomat.update --encrypt --recipient post@self-o-mat.de $TAR

clean
