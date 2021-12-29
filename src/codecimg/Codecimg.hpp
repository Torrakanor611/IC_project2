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
        bool verbose;

    public:
        Codecimg();
        /**
         * Codecaud Class Constructor
         * @param filename path to an image file.
         * @param verb variable to print histograms, images and entropys
         */
        Codecimg(const char *filename, bool verb);
        /**
         * Compress an input file using lossless
         * @param fileDst path to the image file to be compressed
         */
        void compress(const char *fileDst);
        /**
         * Compress an input file using lossy compression
         * @param fileDst path to the image file to be compressed
         * @param qy quantization step for y channel (number of bits to be removed)
         * @param qu quantization step for u channel (number of bits to be removed)
         * @param qv quantization step for v channel (number of bits to be removed)
         **/
        void compresslossy(const char *fileDst, int qy, int qu, int qv);
        /**
         * Decompress an input audio file
         * @param fileSrc path to the image file to be decompressed
         */
        void decompress(const char *fileSrc);
        /**
         * Decompress an input audio file
         * @param fileSrc path to the image file to be decompressed
         * @param fileDst name to be saved in the ouput decompressed image file
         */
        void decompress(const char *fileSrc, const char *fileDst);
};

#endif