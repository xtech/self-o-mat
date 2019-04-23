#!/bin/sh


clean() {
    # to to build dir
    cd $2

    # clean app
    rm -rf ./app
}

echo "Building web files $1"

clean

cd $1/web

npm install
ionic build --prod

cp -r ./www $2/app
