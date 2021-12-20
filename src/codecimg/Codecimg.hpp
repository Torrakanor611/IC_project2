#ifndef CODECIMG_H
#define CODECIMG_H

#include <opencv2/core.hpp>


using namespace cv;

class Codecimg{
    private:
        Mat Y, U, V;
        // ideal m
        void idealM(int pM);
        void transformYUV420(Mat m);

    public:
        Codecimg();
        Codecimg(const char *filename);
        void applyPredJLS();
        // filename -> nome do fich codificado
        void encode(const char *filename);
        // filename -> nome do fich descodificado
        void decode(const char *filename);
        void restore(const char * filename);
};

#endif