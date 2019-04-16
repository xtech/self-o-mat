#!/bin/sh


clean() {
    # to to build dir
    cd $2

    # remove old update
    rm -rf update.tar

    # clean libs
    rm -rf ./libs

    # clean settings and assets
    rm -rf ./assets
    rm -rf ./settings.json
}

echo "Packing files in $1"

NAME=self_o_mat
BINARY=self_o_mat
FILES="$BINARY settings.json assets/* libs/*"
TAR=update.tar


clean

cd $2

# get the settings and assets from src dir into the build dir
cp -r $1/assets .
cp -r $1/settings.json .

# get libs
if [[ ! -d libs ]]; then
    mkdir libs
fi
ldd $BINARY | grep "=> /" | while read a b c d; do cp "$c" libs/; done

# pack and sign
tar cfz $NAME.tar.gz $FILES
gpg --quiet --batch --yes --output $NAME.sig --detach-sig $NAME.tar.gz
tar cf $TAR $NAME.tar.gz $NAME.sig
rm $NAME.tar.gz $NAME.sig
rm -rf $2/libs