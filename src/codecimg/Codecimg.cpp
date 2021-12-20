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
void apply(Mat src, vector<int> res);
void restoreMat(vector<int>, Mat src);
void printij(int i, int j);
// void encodeG(Golomb b, Mat m);

Codecimg::Codecimg(){}

Codecimg::Codecimg(const char *filename){
    Mat img = imread(filename, IMREAD_COLOR);
    if(img.empty()){
        throw new runtime_error("could not make mat");
    }
    showImg("RGB", img);
    transformYUV420(img);
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
            // aux[0] -> b 
            // aux[1] -> g 
            // aux[2] -> r
            Y.at<uchar>(i, j) = 0.299 * aux[2] + 0.587 * aux[1] + 0.114 * aux[0];
            U.at<uchar>(i, j) = 128 - 0.168736 * aux[2] - 0.331264 * aux[1] + 0.5 * aux[0];
            V.at<uchar>(i, j) = 128 + 0.5 * aux[2] - 0.418688 * aux[1] - 0.081312 * aux[2];
        }  
    resize(U, U, Size(m.cols / 2, m.rows / 2), INTER_LINEAR);
    resize(V, V, Size(m.cols / 2, m.rows / 2), INTER_LINEAR);
}

void Codecimg::applyPredJLS(){
    // apply preditor to Mats Y, U and V
    apply(Y, resY);
    apply(U, resU);
    apply(V, resV);
    // showYUV("YUV", Y, V, U);
    // showImg("resY after pred", resY);
    // showImg("resU after pred", resU);
    // showImg("resV after pred", resV);
}

void Codecimg::encode(const char *filename){

    int m = 5000;

    cout << "started encoding..." << endl; 

    Golomb g(filename, 'e', m);

    g.encodeM(m);

    g.encode(Y.cols);
    g.encode(Y.rows);

    // cout << resY.cols << "<->" << resY.rows << endl;
    // cout << resU.cols << "<->" << resU.rows << endl;
    // cout << resV.cols << "<->" << resV.rows << endl;

    // codificados Y , U , V respectivamente
    // cada 1 deles do canto inferir esq para dir e para cima

    for(int i = 0; i < Y.rows; i++)
        for(int j = 0; j < Y.cols; j++)
            g.encode(resY[i]);

    for(int i = 0; i < U.rows; i++)
        for(int j = 0; j < U.cols; j++)
            g.encode(resU[i]);

    for(int i = 0; i < V.rows; i++)
        for(int j = 0; j < V.cols; j++)
            g.encode(resV[i]);

    g.close();
}

void Codecimg::decode(const char *filename){
    Golomb g(filename, 'd', 0);

    int m = g.decodeM();

    g.setM(m);

    int cols = g.decode();
    int rows = g.decode();

    // cout << cols << "<->" << rows << endl;


    Mat Y = Mat::zeros(rows, cols, CV_8UC1);
    Mat U = Mat::zeros(rows / 2, cols / 2, CV_8UC1);
    Mat V = Mat::zeros(rows / 2, cols / 2, CV_8UC1);

    cout << "-> teste" << endl;

    for(int i = 0; i < rows; i++)
        for(int j = 0; j < cols; j++)
            resY[i] = g.decode();


    for(int i = 0; i < rows / 2; i++)
        for(int j = 0; j < cols / 2; j++)
            resU[i] = g.decode();


    for(int i = 0; i < rows / 2; i++)
        for(int j = 0; j < cols / 2; j++)
            resV[i] = g.decode();
    g.close();
}

void Codecimg::restore(const char * filename){

    restoreMat(resY, Y);
    restoreMat(resU, U);
    restoreMat(resV, V);

    // showImg("Y after restore", Y);
    // showImg("U after restore", U);
    // showImg("V after restore", V);
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

void apply(Mat src, vector<int> res){
    int a, b, c;

    for(int i = 0; i < src.rows - 1; i++)
        res[i] = src.at<uchar>(i, 0);

    for(int j = 1; j < src.cols - 1; j++)
        res[j] = src.at<uchar>(src.rows - 1, j);
    
    for(int i = src.rows - 2; i > -1 ; i--){
        for(int j = src.cols - 2 ; j > -1; j--){
            a = src.at<uchar>(i, j + 1);
            b = src.at<uchar>(i + 1, j);
            c = src.at<uchar>(i + 1, j + 1);

            // rn = xn - ^xn
            int xn = ((int)src.at<uchar>(i, j));
            int xchapeun = preditorJLS(a, b, c);

            res.at<uchar>(i, j) = xn >= xchapeun ? ((int)src.at<uchar>(i, j)) - preditorJLS(a, b, c) : ((int)src.at<uchar>(i, j)) - preditorJLS(a, b, c) - 256;

            if (i == 20 && j == 20){
                printf("(i, j) = "); printij(i, j); printf("\n");
                printf("aplicando a fórmula -> rn = xn - ^xn\n");
                printf("valor inicial (xn) para (i, j): %d\n", src.at<uchar>(i, j));
                printf("valor preditado (x^n) para (i, j): %d || valor residual para(i, j): %d\n", preditorJLS(a, b, c), res.at<uchar>(i, j));
                printf("a -> "); printij(i, j - 1);printf("\n");
                printf("b -> "); printij(i + 1, j); printf("\n");
                printf("c -> "); printij(i +1, j - 1); printf("\n");
                printf("\n");
            }
        }
    }
}

void restoreMat(vector<int> res, Mat src){
    int a, b, c, rn;

    for(int i = 0; i < res.rows - 1; i++)
        src.at<uchar>(i, 0) = res.at<uchar>(i, 0);

    for(int j = 1; j < res.cols - 1; j++)
        src.at<uchar>(src.rows - 1, j) = res.at<uchar>(src.rows - 1, j);

    for(int i = res.rows - 2; i > -1 ; i--){
        for(int j = res.cols - 2 ; j > -1; j--){
            // residual
            rn = res.at<uchar>(i, j);
            a = res.at<uchar>(i, j + 1);
            b = res.at<uchar>(i + 1, j);
            c = res.at<uchar>(i + 1, j + 1);

            int xchapeun = preditorJLS(a, b, c);

            src.at<uchar>(i, j) = rn - xchapeun;

            if (i == 20 && j == 20){
                printf("(i, j) = "); printij(i, j);
                printf("valor preditado (x^n) para (i, j): %d || valor residual para(i, j): %d\n", xchapeun, rn);
                printf("a -> "); printij(i, j - 1);printf("\n");
                printf("b -> "); printij(i + 1, j); printf("\n");
                printf("c -> "); printij(i +1, j - 1); printf("\n");
                printf("valor restaurado (xn) para (i, j): %d", src.at<uchar>(i, j));
                printf("\n");
            }
        }
    }
}

void printij(int i, int j){
    printf("(%d, %d)", j, i);
}

