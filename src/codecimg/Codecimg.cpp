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
void res2Mat(vector<int> src, Mat dst);

Codecimg::Codecimg(){}

Codecimg::Codecimg(const char *filename){
    Mat img = imread(filename, IMREAD_COLOR);
    if(img.empty()){
        throw new runtime_error("could not make mat");
    }
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

    g.encode(Y.cols);
    g.encode(Y.rows);

    for(int i = 0; i < (int) resY.size(); i++){
        g.encode(resY[i]);
    }
    for(int i = 0; i < (int) resU.size(); i++){
        g.encode(resU[i]);
    }
    for(int i = 0; i < (int) resV.size(); i++){
        g.encode(resV[i]);
    }
    g.close();
}

void apply(Mat src, vector<int> res){
    int a, b, c;
    for(int x = src.cols - 1; x > - 1; x--){
        // printf("added to array pos1: "); printij(x, src.rows - 1); printf("\n");
        res.push_back(src.at<uchar>(src.rows - 1, x));
    }
    for(int y = src.rows - 2; y > -1 ; y--){
        for(int x = src.cols - 1; x > -1; x--){
            if(x == src.cols - 1){
                // printf("added to array pos: "); printij(x, y); printf("\n");
                res.push_back(src.at<uchar>(y, x));
                continue;
            }
            a = src.at<uchar>(y, x + 1);
            b = src.at<uchar>(y + 1, x);
            c = src.at<uchar>(y + 1, x + 1);
            // rn = xn - ^xn
            // printf("added to array pos: "); printij(x, y); printf("\n");
            res.push_back(src.at<uchar>(y, x) - preditorJLS(a, b, c));
            // if (y == 20 && x == 26){
            //     printf("(y, x) = "); printij(y, x); printf("\n");
            //     printf("aplicando a fórmula -> rn = xn - ^xn\n");
            //     printf("valor inicial (xn) para (y, x): %d\n", src.at<uchar>(y, x));
            //     printf("valor preditado (x^n) para (y, x): %d || valor residual para(y, x): %d\n", preditorJLS(a, b, c), res[res.size() - 1]);
            //     printf("a -> "); printij(y, x - 1); printf("\n");
            //     printf("b -> "); printij(y + 1, x); printf("\n");
            //     printf("c -> "); printij(y + 1, x - 1); printf("\n");
            //     printf("\n");
            // }
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

    vector<int> resY, resU, resV;

    for(int i = 0; i < ncols*nrows; i++){
        resY.push_back(g.decode());
    }
    for(int i = 0; i < (ncols*3/2)*(nrows*3/2); i++){
        resU.push_back(g.decode());
    }
    for(int i = 0; i < (ncols*3/2)*(nrows*3/2); i++){
        resV.push_back(g.decode());
    }

    g.close();

    Y = Mat(nrows, ncols, CV_8UC1);
    U = Mat(nrows*3/2, ncols*3/2, CV_8UC1);
    V = Mat(nrows*3/2, ncols*3/2, CV_8UC1);

    int a, b, c, xCn;
    for(int i = ncols + 1; i < resY.size(); i++){
        a = resY[i - 1];
        b = resY[i - ncols];
        c = resY[i - ncols - 1];

        xCn = preditorJLS(a, b, c);

        
    }

    // falta calcular a "imagem"
    // colocar num Mat e escrever

    printf("all ok!");
    exit(0);
}

void res2Mat(vector<int> src, Mat dst){
    ;
}


void Codecimg::restore(const char * filename){
    ;
}


void restoreMat(vector<int> res, Mat src){
    ;
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

