#include "Codecimg.hpp"
#include "preditor.cpp"
#include "matplotlibcpp.h"

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>


using namespace cv;
using namespace std;
namespace plt = matplotlibcpp;

void showImg(const char* title, Mat x);

Codecimg::Codecimg(){}

Codecimg::Codecimg(const char *filename){
    Mat imgaux = imread(filename, IMREAD_COLOR);
    if(imgaux.empty()){
        throw new runtime_error("could not make mat");
    }
    showImg("in RGB", imgaux);
    transformYUV420(imgaux);
    showImg("Y", Y);
    showImg("U", U);
    showImg("V", V);
}

void Codecimg::transformYUV420(Mat m){
    // Y = 0.299R + 0.587G + 0.114B
    // Cb = 128 − 0.168736R − 0.331264G + 0.5B
    // Cr = 128 + 0.5R − 0.418688G − 0.081312B
    Y = Mat(m.size(), CV_8UC1);
    U = Mat(m.size(), CV_8UC1);
    V = Mat(m.size(), CV_8UC1);
    Vec3b aux;
    for(int i = 0; i < m.rows ; i++)
        for(int j = 0; j < m.cols; j++){
            aux = m.at<Vec3b>(i, j);
            // aux[0] -> b aux[1] -> g aux[2] -> r
            
            Y.at<uchar>(i, j) = 0.299 * aux[2] + 0.587 * aux[1] + 0.114 * aux[0];
            U.at<uchar>(i, j) = 128 - 0.168736 * aux[2] - 0.331264 * aux[1] + 0.5 * aux[0];
            V.at<uchar>(i, j) = 128 + 0.5 * aux[2] - 0.418688 * aux[1] - 0.081312 * aux[2];
        }  
    resize(U, U, Size(m.cols / 2, m.rows / 2), INTER_LINEAR);
    resize(V, V, Size(m.cols / 2, m.rows / 2), INTER_LINEAR);
}

void Codecimg::applyPredJLS(){
    // //Initialize pred object
    // Vec3b a, b, c, aux;
    // cout << "teste2" << endl;
    // for(int i = img.rows - 2; i > -1 ; i--){
    //     for(int j = img.cols - 2 ; j > -1; j--){
    //         a = img.at<Vec3b>(i, j - 1);
    //         b = img.at<Vec3b>(i - 1, j);
    //         c = img.at<Vec3b>(i - 1, j - 1);

    //         aux[0] = preditorJLS(a[0], b[0], c[0]);
    //         aux[1] = preditorJLS(a[1], b[1], c[1]);
    //         aux[2] = preditorJLS(a[2], b[2], c[2]);

    //         /// 3 canais rgb
    //         pred.at<Vec3b>(i, j) = aux;
    //     }
    // }
    // for(int i = 0; i < img.rows - 1; i++){
    //     pred.at<uchar>(i, 0) = img.at<uchar>(i, 0);
    // }
    // for(int j = 1; j < img.cols - 1; j++){
    //     pred.at<uchar>(img.rows - 1, j) = img.at<uchar>(img.rows - 1, j);
    // }
}

void Codecimg::encode(){

}


void Codecimg::write(const char *filename){
    ;
}

void showImg(const char* title, Mat m){
    imshow(title, m);

    // stop display
    puts("press q or ESC to stop display...");
    int key = waitKey(-1);
    while(true){
        if(key == 'q' || key == 27){
                puts("stoped!");
                break;
            }
    }
    destroyAllWindows();


    // histogram
    map<int, int> hist;
    for(int i = 0; i < m.rows; i++)
        for(int j = 0; j < m.cols; j++){
            int aux = (int) m.at<uchar>(i, j);
            hist[aux]++;
        }
    vector<int> x;
    vector<int> y;
    for(auto i : hist) {
        x.push_back(i.first);
        y.push_back(i.second);
    }
    plt::figure(1);
    plt::bar(x, y);
    plt::xlabel("x");
    plt::ylabel("y");
    plt::title(title);
    plt::show();
}
