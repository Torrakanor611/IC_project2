#include "Codecimg.hpp"
#include "preditor.cpp"
#include "matplotlibcpp.h"
#include "../golomb/Golomb.cpp"

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;
namespace plt = matplotlibcpp;

// visualization
void showImg(const char* title, Mat m);
void showImgHist(const char* title, Mat m);
void showYUVHist(const char* title, vector<int>& Y, vector<int>& V, vector<int>& U);
// logic
void apply(Mat src, vector<int>& res);
void applylossy(Mat src, vector<int>& res, int q);
void restore(uchar* data, vector<int>& res, int nrows, int ncols);
void reverse(uchar* src, uchar* dst, int size);
int idealM(Golomb& g, vector<int>& resY, vector<int>& resU, vector<int>& resV);
// aux
void printij(int i, int j);

Codecimg::Codecimg(){}

Codecimg::Codecimg(const char *filename, bool verb){
    verbose = verb;
    Mat img = imread(filename, IMREAD_COLOR);
    if(img.empty()){
        throw new runtime_error("could not make mat");
    }
    this->filename = (char*) filename;
    if(verbose) showImgHist("image original(input)", img);
    transformYUV420(img);
}

void Codecimg::transformYUV420(Mat m){
    Y = Mat(m.size(), CV_8UC1);
    Mat auxU = Mat(m.size(), CV_8UC1);
    Mat auxV = Mat(m.size(), CV_8UC1);
    Vec3b aux;
    for(int i = 0; i < m.rows ; i++)
        for(int j = 0; j < m.cols; j++){
            aux = m.at<Vec3b>(i, j);
            // Y = 0.299R + 0.587G + 0.114B
            // Cb = 128 − 0.168736R − 0.331264G + 0.5B
            // Cr = 128 + 0.5R − 0.418688G − 0.081312B
            // aux[0] -> b // aux[1] -> g // aux[2] -> r
            double y, u ,v;
            y = 0.299 * aux[2] + 0.587 * aux[1] + 0.114 * aux[0];
            u = 128 + (-0.169) * aux[2] - 0.331 * aux[1] + 0.5 * aux[0];
            v = 128 + 0.5 * aux[2] - 0.419 * aux[1] - 0.081 * aux[2];

            Y.at<uchar>(i, j) = y;
            auxU.at<uchar>(i, j) = u;
            auxV.at<uchar>(i, j) = v;
        }  

    U = Mat(m.rows/2, m.cols/2, CV_8UC1);
    V = Mat(m.rows/2, m.cols/2, CV_8UC1);

    uchar bl, br, tr, tl;
    int sum, med;

    for(int y = 0; y < auxU.rows - 1; y += 2)
        for(int x = 0; x < auxU.cols - 1; x += 2){
            bl = auxU.at<uchar>(y, x);
            br = auxU.at<uchar>(y + 1, x);
            tr = auxU.at<uchar>(y, x + 1);
            tl = auxU.at<uchar>(y + 1 , x + 1);

            sum = bl + br + tl + tr;

            med = round((double) sum / 4);

            U.at<uchar>(y/2, x/2) = med;

            bl = auxV.at<uchar>(y, x);
            br = auxV.at<uchar>(y + 1, x);
            tr = auxV.at<uchar>(y, x + 1);
            tl = auxV.at<uchar>(y + 1 , x + 1);

            sum = bl + br + tl + tr;

            med = round((double) sum / 4);

            V.at<uchar>(y/2, x/2) = med;
        }
}

void Codecimg::compress(const char *fileDst){
    // printf("started compress...\n");
    
    vector<int> resY, resU, resV;
    apply(Y, resY);
    apply(U, resU);
    apply(V, resV);

    if(verbose) showYUVHist("histogram of residuals", resY, resU, resV);

    int m = 0;
    Golomb g = Golomb(fileDst, 'e', m);
    m = idealM(g, resY, resU, resV);

    g.setM(m);

    g.encodeMode(0);

    g.encodeM(m);
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

void Codecimg::compresslossy(const char *fileDst, int qy, int qu, int qv){
    // printf("started lossy compress...\n");
    
    vector<int> resY, resU, resV;
    applylossy(Y, resY, qy);
    applylossy(U, resU, qu);
    applylossy(V, resV, qv);

    if(verbose) showYUVHist("histogram of residuals", resY, resU, resV);

    int m = 0;
    Golomb g = Golomb(fileDst, 'e', m);
    m = idealM(g, resY, resU, resV);

    // printf("ideal m = %d\n", m);
    g.setM(m);

    g.encodeMode(1);

    g.encondeShamt(qy);
    g.encondeShamt(qu);
    g.encondeShamt(qv);

    g.encodeM(m);
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
        res.push_back(src.at<uchar>(src.rows - 1, x));
    }
    for(int y = src.rows - 2; y > -1 ; y--){
        for(int x = src.cols - 1; x > -1; x--){
            if(x == src.cols - 1){
                res.push_back(src.at<uchar>(y, x));
                continue;
            }
            a = src.at<uchar>(y, x + 1);
            b = src.at<uchar>(y + 1, x);
            c = src.at<uchar>(y + 1, x + 1);
            // rn = xn - ^xn
            res.push_back(src.at<uchar>(y, x) - preditorJLS(a, b, c));
        }
    }
}

void applylossy(Mat src, vector<int>& res, int q){
    int a, b, c;
    for(int x = src.cols - 1; x > - 1; x--){
        res.push_back(src.at<uchar>(src.rows - 1, x) >> q);
        src.at<uchar>(src.rows - 1, x) = res[(res.size())-1] << q;
    }
    for(int y = src.rows - 2; y > -1 ; y--){
        for(int x = src.cols - 1; x > -1; x--){
            if(x == src.cols - 1){
                res.push_back((src.at<uchar>(y, x)) >> q);
                src.at<uchar>(y, x) = res[(res.size())-1] << q;
                continue;
            }
            a = src.at<uchar>(y, x + 1);
            b = src.at<uchar>(y + 1, x);
            c = src.at<uchar>(y + 1, x + 1);
            // rn = xn - ^xn
            res.push_back((src.at<uchar>(y, x) - preditorJLS(a, b, c)) >> q);
            src.at<uchar>(y, x) = preditorJLS(a, b, c) + (res[(res.size())-1] << q);
        }
    }
}

void Codecimg::decompress(const char *fileSrc){
    decompress(fileSrc, filename);
}

void Codecimg::decompress(const char *fileSrc, const char *fileDst){
    // printf("started decompress...\n");
    Golomb g = Golomb(fileSrc, 'd', 0);

    int mode = g.decodeMode();

    int qy = 0, qu = 0, qv = 0;
    if(mode == 1){
        qy = g.decodeShamt();
        qu = g.decodeShamt();
        qv = g.decodeShamt();
    }

    int m = g.decodeM();
    g.setM(m);

    int ncols = g.decode();
    int nrows = g.decode();

    vector<int> resY, resU, resV;

    for(int i = 0; i < ncols*nrows; i++){
        resY.push_back(g.decode() << qy);
    }
    for(int i = 0; i < (ncols/2)*(nrows/2); i++){
        resU.push_back(g.decode() << qu);
    }
    for(int i = 0; i < (ncols/2)*(nrows/2); i++){
        resV.push_back(g.decode() << qv);
    }

    g.close();

    Y = Mat(nrows, ncols, CV_8UC1);
    Mat auxU = Mat(nrows/2, ncols/2, CV_8UC1);
    Mat auxV = Mat(nrows/2, ncols/2, CV_8UC1);

    uchar dataY[Y.rows * Y.cols], fdataY[Y.rows * Y.cols];
    uchar dataU[U.rows * U.cols], fdataU[U.rows * U.cols];
    uchar dataV[V.rows * V.cols], fdataV[V.rows * V.cols];

    restore(dataY, resY, Y.rows, Y.cols);
    restore(dataU, resU, auxU.rows, auxU.cols);
    restore(dataV, resV, auxV.rows, auxV.cols);

    int aux = Y.rows * Y.cols;

    reverse(dataY, fdataY, aux);
    reverse(dataU, fdataU, aux/4);
    reverse(dataV, fdataV, aux/4);

    Y.data = fdataY;
    auxU.data = fdataU;
    auxV.data = fdataV;

    Mat maux(nrows, ncols, CV_8UC3);
    transformRGB(maux, auxU, auxV);

    if(verbose) showImg("imagem restored(output)", maux);

    imwrite(fileDst, maux);
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
        xCn = preditorJLS(a, b, c);
        data[i] = (uchar) rn + xCn;
    }
}

void Codecimg::transformRGB(Mat &m, Mat &auxU, Mat &auxV){
    U = Mat(m.rows, m.cols, CV_8UC1);
    V = Mat(m.rows, m.cols, CV_8UC1);

    uchar u, v;
    int xx = 0, yy = 0;

    for(int y = 0; y < auxU.rows; y++){
        for(int x = 0; x < auxU.cols; x++){
            u = auxU.at<uchar>(y, x);
            v = auxV.at<uchar>(y, x);

            xx = x*2;
            yy = y*2;

            U.at<uchar>(yy, xx) = u;
            U.at<uchar>(yy + 1, xx) = u;
            U.at<uchar>(yy, xx + 1) = u;
            U.at<uchar>(yy + 1, xx + 1) = u;

            V.at<uchar>(yy, xx) = v;
            V.at<uchar>(yy + 1, xx) = v;
            V.at<uchar>(yy, xx + 1) = v;
            V.at<uchar>(yy + 1, xx + 1) = v;
        }
    }
    uchar Yp;
    Vec3b bgr;

    for(int y = 0; y < Y.rows; y++)
        for(int x = 0; x < Y.cols; x++){
            Yp = Y.at<uchar>(y, x);
            u = U.at<uchar>(y, x);
            v = V.at<uchar>(y, x);
            
            bgr[2] = Yp + 1.400 * (v - 128);
            bgr[1] = Yp - 0.343 * (u - 128) - 0.711 * (v - 128);
            bgr[0] = Yp + 1.765 * (u - 128);

            m.at<Vec3b>(y, x) = bgr;
        }
}

// media ponderada
int idealM(Golomb& g, vector<int>& resY, vector<int>& resU, vector<int>& resV){
    map<int, int> aux;
    double med;
    for(auto i : resY) aux[i]++;
    for(auto i : resU) aux[i]++;
    for(auto i : resV) aux[i]++;

    int samples = 0;
    for(auto i : aux)
        samples += i.second;

    for(auto i : aux)
        med += g.fold(i.first) * ((double)i.second / samples);

    return ceil(-1 / log2(med / (med + 1)));
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
    double entropy = 0;
    int total = m.rows * m.cols;
    double prob;
    for(auto i : hist){
        prob = ((double) i.second) / total;
        entropy += prob * -log2(prob);
    }
    printf("Entropy of \"%s\": %f bits\n", title, entropy);

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

void showYUVHist(const char* title, vector<int>& Y, vector<int>& V, vector<int>& U){
    vector<int> m;
    // histogram
    map<int, int> hist;
    for(int x = 0; x < 3; x++){
        if(x == 0)
            m = Y;
        else if(x == 1)
            m = U;
        else
            m = V;
        for(int i = 0; i < m.size(); i++)
            hist[m[i]]++;
    }
    double entropy = 0;
    int total = Y.size() + V.size() + U.size();
    double prob;
    for(auto i : hist){
        prob = ((double) i.second) / total;
        entropy += prob * -log2(prob);
    }
    printf("Entropy of \"%s\": %f bits\n", title, entropy);

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