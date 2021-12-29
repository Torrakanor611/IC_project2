#ifndef CODECIMG_H
#define CODECIMG_H

#include <opencv2/core.hpp>
#include <vector>

using namespace cv;


class Codecimg{
    private:
        char* filename;
        Mat Y, U, V;
        // ideal m
        void transformYUV420(Mat m);
        void transformRGB(Mat &m, Mat &auxU, Mat &auxV);
        int mode;

    public:
        Codecimg();
        Codecimg(const char *filename, int mode);
        void compress(const char *fileDst);
        void compresslossy(const char *fileDst, int qy, int qu, int qv);
        void decompress(const char *fileSrc);
        void decompress(const char *fileSrc, const char *fileDst);
};

#endif