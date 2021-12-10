#ifndef CODECIMG_H
#define CODECIMG_H

#include<opencv.hpp>

using namespace cv;

class Codecimg{
    private:
        char* filename;

        static void transformYUV420(Mat* img);

    public:
        Codecimg();
        Codecimg(const char *filename);
};

#endif