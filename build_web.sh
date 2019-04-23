#!/bin/sh


echo "Building web files $1"

# to to build dir
cd $2

# clean app
rm -rf ./app

cd $1/web

npm install
ionic build --prod

cp -r ./www $2/app
