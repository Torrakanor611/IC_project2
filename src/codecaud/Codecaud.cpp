#include "Codecaud.hpp"
#include "../golomb/Golomb.cpp"

#include <iostream>
#include <stdio.h>
#include <sndfile.h>
#include <math.h>
#include <vector>
#include "matplotlibcpp.h"
#include <map>

using namespace std;
namespace plt = matplotlibcpp;

Codecaud::Codecaud(){}

Codecaud::Codecaud(const char *filename){

    SNDFILE *infile;
    int readcount;
    short ch[2];

	if (! (infile = sf_open (filename, SFM_READ, &sfinfo))) {
        throw runtime_error("File doesn't exists");
	}
    
    while ((readcount = (int) sf_readf_short (infile, ch, 1)) > 0) {
        chs.push_back(ch[0]);
        chs.push_back(ch[1]);
    }
    
	sf_close (infile) ;  
}

void Codecaud::compress(const char *fileDst, int num) {

    ninput = num;
    
    // predictor
    vector<int> xn;
    
    if(ninput == 1) {
        for(int i = 0; i < chs.size(); i++) {
            if(i == 0) {
                xn.push_back(0);
            }
            else {
                xn.push_back(chs[i-1]);
            }
            rn.push_back(chs[i]-xn[i]);
        }
    }
    else if(ninput == 2) {
        for(int i = 0; i < chs.size(); i++) {
            if(i == 0 || i == 1) {
                xn.push_back(0);
            }
            else {
                xn.push_back((int) (2*chs[i-1] - chs[i-2]));
            }
            rn.push_back(chs[i]-xn[i]);
        }
    }
    else {
        for(int i = 0; i < chs.size(); i++) {
            if(i == 0 || i == 1 || i == 2) {
                xn.push_back(0);
            }
            else {
                xn.push_back((int) (3*chs[i-1] - 3*chs[i-2] + chs[i-3]));
            }
            rn.push_back(chs[i]-xn[i]);
        }
    }

    cout << "started encoding..." << endl; 

    // Golomb
    Golomb g(fileDst, 'e', 0);

    double m = 0;
    for(int i = 0; i < rn.size(); i++) {
        m += g.fold(rn[i]);
    }
    m = m/rn.size();

    m = (int) ceil(-1/(log2(m/(m+1))));

    g.setM(m);
    g.encodeM(m);

    g.encodeHeaderSound(sfinfo.frames, sfinfo.samplerate, sfinfo.channels, sfinfo.format);
    
    for(int i = 0; i < rn.size(); i++) {
        g.encode(rn[i]);
    } 
    g.close(); 
}

void Codecaud::decompress(const char *fileSrc) {
    cout << "started decoding..." << endl; 

    Golomb g(fileSrc, 'd', 0);

    int m = g.decodeM();

    g.setM(m);

    int infoDeco[4]; 
    g.decodeHeaderSound(infoDeco);

    vector<int> resChs;

    for(int i = 0; i < infoDeco[0]*infoDeco[3]; i++) {
        resChs.push_back(g.decode());
    }

    g.close();

    // preditor
    vector<short> resXN;
    vector<int> resHatXN; 
    
    if(ninput == 1) {
        resXN.push_back(resChs[0]);
        for(int i = 1; i < resChs.size(); i++) {
            resXN.push_back((short) resChs[i] + resXN[i-1]);
        }
    }
    else if(ninput == 2) {
        for(int i = 0; i < 2; i++) {
            resHatXN.push_back(0);
            resXN.push_back((short) resChs[i] + resHatXN[i]);
        }
        for(int i = 2; i < resChs.size(); i++) {
            resHatXN.push_back((int) (2*resXN[i-1] - resXN[i-2]));
            resXN.push_back((short) resChs[i] + resHatXN[i]);
        }
    }
    else {
        for(int i = 0; i < 3; i++) {
            resHatXN.push_back(0);
            resXN.push_back((short) resChs[i] + resHatXN[i]);
        }
        for(int i = 3; i < resChs.size(); i++) {
            resHatXN.push_back((int) (3*resXN[i-1] - 3*resXN[i-2] + resXN[i-3]));
            resXN.push_back((short) resChs[i] + resHatXN[i]);
        }
    }

    SF_INFO sfinfoOut ;
    sfinfoOut.channels = infoDeco[3];
    sfinfoOut.samplerate = infoDeco[1];
    sfinfoOut.format = infoDeco[2];
    sfinfoOut.frames = infoDeco[0];

    SNDFILE * outfile = sf_open("out.wav", SFM_WRITE, &sfinfoOut);
    sf_count_t count = sf_write_short(outfile, &resXN[0], resXN.size()) ;
    sf_write_sync(outfile);
    sf_close(outfile);
}

void Codecaud::showHistEnt() {

    // Calculate the entropy
    map<short, int> map_ori, map_afterPred;
    for(int i = 0; i < (int) chs.size(); i++) {
        map_ori[chs[i]]++;
        map_afterPred[rn[i]]++;
    }
    
    double entropy = 0;
    double p;
    for(auto i : map_ori) {
        p = (double) i.second / (double) chs.size();
        entropy += p * (-log2(p));
    }
    cout << "Entropy Original: " << entropy << endl;

    entropy = 0;
    for(auto i : map_afterPred) {
        p = (double) i.second / (double) rn.size();
        entropy += p * (-log2(p));
    }
    cout << "\nEntropy of the residuals obtained after prediction: " << entropy << endl;

    // show Histo
    map<short, int> map_pred;

    for(int i = 0; i < (int) rn.size()/2; i+=2) {
        map_pred[rn[i]]++;
    }
    plt::figure(1);
    vector<int> samples;
    vector<int> occur;

    for(auto i : map_pred) {
        samples.push_back(i.first);
        occur.push_back(i.second);
    }
    plt::bar(samples, occur);
    plt::xlabel("Samples");
    plt::ylabel("Number of occurences");
    plt::title("Residuals obtained after prediction");
    plt::show();
}