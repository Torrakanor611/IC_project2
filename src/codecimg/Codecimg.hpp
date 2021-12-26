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
        void idealM(int pM);
        void transformYUV420(Mat m);
        void transformRGB(Mat &m);

        void applyPredJLS();
        // filename -> nome do fich codificado
        void encode(const char *filename);
        // filename -> nome do fich descodificado
        void decode(const char *filename);

    public:
        Codecimg();
        Codecimg(const char *filename);
        void compress(const char *fileDst);
        void decompress(const char *fileSrc);
};

#endif