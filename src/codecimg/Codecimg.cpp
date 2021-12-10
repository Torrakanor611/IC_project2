#include "Codecimg.hpp"

#include<opencv2/opencv.hpp>

using namespace cv;
using namespace std;

Codecimg::Codecimg(){}

Codecimg::Codecimg(const char *filename){
    Mat img = imread(filename, IMREAD_COLOR);
}
