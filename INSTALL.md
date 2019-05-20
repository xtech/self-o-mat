# INSTALL
To get the software running on a clean raspbian-lite image, do the following:
```
sudo apt-get install libgphoto2-dev libmagick++-dev
tar -xf selfomat.tar.gz
./start.sh
```

The images are stored to the current folder. In order to trigger the capture send a POST request to localhost:9080/trigger.
This can be done on the command line using curl:

```
curl -X POST localhost:9080/trigger
```

The destination directory for taken images can be set in the settings.json file.
