#include <vector>
#include <logic/filters/IImageFilter.h>
#include <logic/filters/BasicImageFilter.h>
#include <iostream>
#include <boost/filesystem.hpp>
#include <string>

using namespace selfomat::logic;
using namespace boost::filesystem;

int main(int argc, char *argv[]) {

    if(argc != 2) {
        std::cout << "Usage: ./filter_test <image in dir> <image out dir>" << std::endl;
        return 1;
    }

    std::vector<IImageFilter*> filters;
    filters.push_back(new BasicImageFilter());

    path p(argv[1]);
    directory_iterator end_itr;

    path out_dir("filter_test_output");

    system("rm -rf ./filter_test_output");

    create_directories(out_dir);



    // cycle through the directory
    int imageIndex = 0;
    for (directory_iterator itr(p); itr != end_itr; ++itr)
    {
        if (is_regular_file(itr->path())) {
            imageIndex++;
            std::string current_file = itr->path().string();
            std::cout << current_file << std::endl;

            // Load file into image magick
            Magick::Image image;


            for(auto &filter : filters) {
                for(double d = 0.0; d <= 1.0; d+=0.25) {
                    image.read(current_file);
                    image.resize(Magick::Geometry(500, 500));
                    image.write("scaled.jpg");
                    filter->processImage(image, d);
                    image.write("filtered.jpg");
                    std::string outfile =
                            out_dir.string() + "/" + std::to_string(imageIndex) + "_" + filter->getName() + "_" + std::to_string((int)(d*100)) +
                            itr->path().extension().string();
                    system((std::string("convert +append scaled.jpg filtered.jpg ") + outfile).c_str());
                    //system((std::string("mv filtered.jpg ")+outfile).c_str());
                }
            }

        }
    }
}