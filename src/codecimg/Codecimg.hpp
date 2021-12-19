#ifndef CODECIMG_H
#define CODECIMG_H

#include <opencv2/core.hpp>


using namespace cv;

class Codecimg{
    private:
        Mat img;
        Mat Y;
        Mat U;
        Mat V;
        // ideal m

        void idealM(int pM);
        void transformYUV420(Mat m);
    public:
        Codecimg();
        Codecimg(const char *filename);
        void applyPredJLS();
        void encode();
        void write(const char *filename);
};

#endif