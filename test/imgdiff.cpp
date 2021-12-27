// g++ imgdiff.cpp -o imgdiff $(pkg-config --libs --cflags opencv4)

#include <opencv2/opencv.hpp>

using namespace cv;

int main(){
    Mat orig = imread("../files/miuda.png", IMREAD_COLOR);

    Mat rest = imread("miuda_restored.png", IMREAD_COLOR);

    Vec3b origi, resta;

    int aux = 0;

    for(int y = 0; y < orig.rows; y++){
        // aux = 0;
        for(int x = 0; x < orig.cols; x++){
            origi = orig.at<Vec3b>(y, x);
            resta = rest.at<Vec3b>(y, x);

            if (aux < 10){
                printf("original: [%d, %d, %d]\n", origi[0], origi[1], origi[2]);
                printf("restored: [%d, %d, %d]\n", resta[0], resta[1], resta[2]);
                printf("---\n");
                aux++;
            }
        }
    }

}