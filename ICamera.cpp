//
// Created by clemens on 28.02.19.
//

#include "ICamera.h"


void ICamera::setImageDir(string image_dir) {
    this->image_dir = image_dir;
}

void ICamera::saveImage(void *data, size_t size) {
    if(image_dir.empty()) {
        cerr << "No image dir specified" << endl;
        return;
    }

    std::time_t time = std::time(nullptr);

    std::string filename = image_dir;

    filename+="/";
    filename+="img_";
    filename+=to_string((long)time);
    filename+=".jpg";

    cout << "Writing image to:" << filename << endl;

    FILE *fp;

    fp = fopen(filename.c_str(), "wb");
    if(fp == nullptr) {
        cerr << "Error opening output file" << endl;
        return;
    }

    fwrite(data, size, 1, fp);

    fclose(fp);

    cout << "File written to: " << filename << endl;
}
