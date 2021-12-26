#include "Codecimg.hpp"
#include "preditor.cpp"
#include "matplotlibcpp.h"
#include "../golomb/Golomb.cpp"

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;
namespace plt = matplotlibcpp;

void showImg(const char* title, Mat m);
void showImgHist(const char* title, Mat m);
void showYUV(const char* title, Mat Y, Mat V, Mat U);
void apply(Mat src, vector<int>& res);
void restoreMat(vector<int>, vector<int>& res);
void restore(uchar* data, vector<int>& res, int nrows, int ncols);
void printij(int i, int j);
void reverse(uchar* src, uchar* dst, int size);

Codecimg::Codecimg(){}

Codecimg::Codecimg(const char *filename){
    Mat img = imread(filename, IMREAD_COLOR);
    if(img.empty()){
        throw new runtime_error("could not make mat");
    }
    this->filename = (char*) filename;
    showImg("img original entrada", img);
    transformYUV420(img);
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
            // aux[0] -> b // aux[1] -> g // aux[2] -> r
            Y.at<uchar>(i, j) = 0.299 * aux[2] + 0.587 * aux[1] + 0.114 * aux[0];
            U.at<uchar>(i, j) = 128 - 0.168736 * aux[2] - 0.331264 * aux[1] + 0.5 * aux[0];
            V.at<uchar>(i, j) = 128 + 0.5 * aux[2] - 0.418688 * aux[1] - 0.081312 * aux[2];
        }  
    resize(U, U, Size(m.cols / 2, m.rows / 2), INTER_LINEAR);
    resize(V, V, Size(m.cols / 2, m.rows / 2), INTER_LINEAR);
}

void Codecimg::compress(const char *fileDst){
    vector<int> resY, resU, resV;
    apply(Y, resY);
    apply(U, resU);
    apply(V, resV);

    int m = 5000;

    cout << "started encoding..." << endl; 

    Golomb g(fileDst, 'e', m);

    g.encodeM(m);

    printf("Y.cols = %d, Y.rows = %d\n", Y.cols, Y.rows);

    g.encode(Y.cols);
    g.encode(Y.rows);

    for(int i = 0; i < Y.cols * Y.rows; i++){
        g.encode(resY[i]);
    }
    for(int i = 0; i < U.cols * U.rows; i++){
        g.encode(resU[i]);
    }
    for(int i = 0; i < V.cols * V.rows; i++){
        g.encode(resV[i]);
    }
    g.close();
}

void apply(Mat src, vector<int>& res){
    int a, b, c;
    for(int x = src.cols - 1; x > - 1; x--){
        //printf("added to array pos1: "); printij(x, src.rows - 1); printf("\n");
        res.push_back(src.at<uchar>(src.rows - 1, x));
    }
    for(int y = src.rows - 2; y > -1 ; y--){
        for(int x = src.cols - 1; x > -1; x--){
            if(x == src.cols - 1){
                //printf("added to array pos: "); printij(x, y); printf("\n");
                res.push_back(src.at<uchar>(y, x));
                continue;
            }
            a = src.at<uchar>(y, x + 1);
            b = src.at<uchar>(y + 1, x);
            c = src.at<uchar>(y + 1, x + 1);
            // rn = xn - ^xn
            //printf("added to array pos: "); printij(x, y); printf("\n");
            res.push_back(src.at<uchar>(y, x) - preditorJLS(a, b, c));
        }
    }
}

void Codecimg::decompress(const char *fileSrc){
    cout << "started decoding..." << endl; 

    Golomb g(fileSrc, 'd', 0);

    int m = g.decodeM();

    g.setM(m);

    int ncols = g.decode();
    int nrows = g.decode();

    printf("ncols = %d, nrows = %d\n", ncols, nrows);

    vector<int> resY, resU, resV;

    for(int i = 0; i < ncols*nrows; i++){
        resY.push_back(g.decode());
    }
    for(int i = 0; i < (ncols/2)*(nrows/2); i++){
        resU.push_back(g.decode());
    }
    for(int i = 0; i < (ncols/2)*(nrows/2); i++){
        resV.push_back(g.decode());
    }

    g.close();

    Y = Mat(nrows, ncols, CV_8UC1);
    U = Mat(nrows/2, ncols/2, CV_8UC1);
    V = Mat(nrows/2, ncols/2, CV_8UC1);

    uchar dataY[Y.rows * Y.cols], fdataY[Y.rows * Y.cols];
    uchar dataU[U.rows * U.cols], fdataU[U.rows * U.cols];
    uchar dataV[V.rows * V.cols], fdataV[V.rows * V.cols];

    restore(dataY, resY, Y.rows, Y.cols);
    restore(dataU, resU, U.rows, U.cols);
    restore(dataV, resV, V.rows, V.cols);

    int aux = Y.rows * Y.cols;

    reverse(dataY, fdataY, aux);
    reverse(dataU, fdataU, aux/4);
    reverse(dataV, fdataV, aux/4);

    Y.data = fdataY;
    U.data = fdataU;
    V.data = fdataV;

    showImg("Y restored", Y);
    showImg("U restored", U);
    showImg("V restored", V);

    Mat maux(nrows, ncols, CV_8UC3);
    transformRGB(maux);

    showImg("imagem restaurada", maux);

    imwrite(filename, maux);

    printf("all ok!\n");
    exit(0);
}


void restore(uchar* data, vector<int>& res, int nrows, int ncols){
    for(int i = 0; i < ncols; i++){
        data[i] = res[i];
    }
    int a, b, c, rn, xCn;
    for(int i = ncols; i < ncols * nrows; i++){
        if (i % ncols == 0){
            data[i] = res[i];
            continue;
        }

        a = data[i - 1];
        b = data[i - ncols];
        c = data[i - ncols - 1];

        rn = res[i];
        // printf("rn = %d", rn);

        xCn = preditorJLS(a, b, c);
        // printf("xCn = %d", xCn);

        data[i] = (uchar) rn + xCn;
        // printf("data = %d", dataY[i]);
        // if(i == (20 * nrows + 26) - 1){
        //     printf("rn = %d\n", rn);
        //     printf("xCn = %d\n", xCn);
        //     printf("data = %d\n", dataY[i]);
        // } 
    }
}

void Codecimg::transformRGB(Mat &m){
    resize(U, U, Size(U.cols * 2, U.rows * 2), INTER_LINEAR);
    resize(V, V, Size(V.cols * 2, V.rows * 2), INTER_LINEAR);

    // showImg("U resized", U);
    // showImg("V resized", V);

    uchar Yp, Up, Vp;
    Vec3b bgr;

    for(int y = 0; y < Y.rows; y++)
        for(int x = 0; x < Y.cols; x++){
            Yp = Y.at<uchar>(y, x);
            Up = U.at<uchar>(y, x);
            Vp = V.at<uchar>(y, x);
            
            bgr[2] = Yp + 1.400 * (Vp - 128);
            bgr[1] = Yp - 0.343 * (Up - 128) - 0.711 * (Vp - 128);
            bgr[0] = Yp + 1.765 * (Up - 128);

            m.at<Vec3b>(y, x) = bgr;
        }
}

void reverse(uchar* src, uchar* dst, int size){
    for(int i = 0; i < size; i++){
        dst[i] = src[(size - 1) - i];
    }
}

 
void showImgHist(const char* title, Mat m){
    showImg(title, m);

    // histogram
    map<int, int> hist;
    for(int i = 0; i < m.rows; i++)
        for(int j = 0; j < m.cols; j++){
            int aux = (int) m.at<uchar>(i, j);
            hist[aux]++;
        }
    vector<int> x, y;
    for(auto i : hist) {
        x.push_back(i.first);
        y.push_back(i.second);
    }
    plt::figure(-1);
    plt::bar(x, y);
    plt::xlabel("x");
    plt::ylabel("y");
    plt::title(title);
    plt::show();
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
    // destroyAllWindows();
}

void showYUV(const char* title, Mat Y, Mat V, Mat U){
    Mat m;
    // histogram
    map<int, int> hist;
    for(int x = 0; x < 3; x++){
        if(x == 0)
            m = Y;
        else if(x == 1)
            m = U;
        else
            m = V;
        for(int i = 0; i < m.rows; i++)
            for(int j = 0; j < m.cols; j++){
                int aux = m.at<uchar>(i, j);
                hist[aux]++;
            }
    }
    vector<int> x, y;
    for(auto i: hist) {
        x.push_back(i.first);
        y.push_back(i.second);
    }
    plt::figure(-1);
    plt::bar(x, y);
    plt::xlabel("x");
    plt::ylabel("y");
    plt::title(title);
    plt::show();
}

void printij(int i, int j){
    printf("(%d, %d)", i, j);
}
