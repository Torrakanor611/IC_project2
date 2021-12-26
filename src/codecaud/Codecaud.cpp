// Coding
    // Calcular X e Y
    // Passar preditor (1 valor n interessa guardar no golomb)
    // Golomb?

// Decoding
    // Golomb
    // Preditor
    // X e Y -> L e R

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
    SF_INFO sfinfo;
    int readcount;
    short ch[2];

	if (! (infile = sf_open (filename, SFM_READ, &sfinfo))) {
        throw runtime_error("File doesn't exists");
	}
    
    while ((readcount = (int) sf_readf_short (infile, ch, 1)) > 0) {
        chs.push_back(ch[0]);
        chs.push_back(ch[1]);
    }

    cout<< "Channels: " << sfinfo.channels << endl;
    cout<< "Format: " << sfinfo.format << endl;
    cout<< "Sample Rate: " << sfinfo.samplerate << endl;

    // for(int i = 0; i< chs.size(); i++) {
    //     cout << chs[i] << endl;
    // }
    
	// sf_close (infile) ;  
}

void Codecaud::compress(const char *fileDst) {
    
    // predictor
    vector<int> xn1,xn2,xn3;
    vector<int> rn1,rn2,rn3;
    int ninput = 0;


    for(int i = 0; i < chs.size(); i++) {
        
        if(i == 0) {
            xn1.push_back(0);
        }
        else {
            xn1.push_back(chs[i-1]);
        }

        if(i == 0 || i == 1) {
            xn2.push_back(0);
        }
        else {
            xn2.push_back((int) (2*chs[i-1] - chs[i-2]));
        }

        if(i == 0 || i == 1 || i == 2) {
            xn3.push_back(0);
        }
        else {
            xn3.push_back((int) (3*chs[i-1] - 3*chs[i-2] + chs[i-3]));
        }
        rn1.push_back(chs[i]-xn1[i]);
        rn2.push_back(chs[i]-xn2[i]);
        rn3.push_back(chs[i]-xn3[i]);

// ninput
    // else if (ninput == 1) {
    //     xn.push_back(chs[i-1]);
    //     rn.push_back(chs[i]-xn[i]);
    //     ninput++;
    // }
    // else if (ninput == 2) {
    //     xn.push_back((int) (2*chs[i-1] - chs[i-2]));
    //     rn.push_back(chs[i]-xn[i]);
    //     ninput++;
    // }
    // else {
    //     xn.push_back((int) (3*chs[i-1] - 3*chs[i-2] + chs[i-3]));
    //     rn.push_back(chs[i]-xn[i]);
    // }
    
    }

    // Calculate the entropy
    map<short, int> map_aud, map_aud1, map_aud2, map_aud3;
    double mean = 0;
    for(int i = 0; i < (int) chs.size(); i++) {
        map_aud[chs[i]]++;
        map_aud1[rn1[i]]++;
        map_aud2[rn2[i]]++;
        map_aud3[rn3[i]]++;
        mean += chs[i];
    }
    
    double entropy = 0;
    double p;
    for(auto i : map_aud) {
        p = (double) i.second / (double) chs.size();
        entropy += p * (-log2(p));
    }
    cout << "\nEntropyOri: " << entropy << endl;

    double entropy1 = 0;
    p=0;
    for(auto i : map_aud1) {
        p = (double) i.second / (double) rn1.size();
        entropy1 += p * (-log2(p));
    }
    cout << "\nEntropy Pred1: " << entropy1 << endl;

    double entropy2 = 0;
    for(auto i : map_aud2) {
        p = (double) i.second / (double) rn2.size();
        entropy2 += p * (-log2(p));
    }
    cout << "\nEntropy Pred2: " << entropy2 << endl;

    double entropy3 = 0;
    for(auto i : map_aud3) {
        p = (double) i.second / (double) rn3.size();
        entropy3 += p * (-log2(p));
    }
    cout << "\nEntropy Pred3: " << entropy3 << endl;

    if(entropy1 < entropy2) {
        if(entropy1 < entropy3) {
            cout << "Small: rn1" << endl;
        }
        else {
            cout << "Small: rn3" << endl;
        }
    }
    else if (entropy2 < entropy3) {
        cout << "Small: rn2" << endl;
    }
    else {
        cout << "Small: rn3" << endl;
    }

    // // show Histo
    // map<short, int> map_ch0;
    // map<short, int> map_0_pred;

    // for(int i = 0; i < (int) chs.size()/2; i+=2) {
    //     map_ch0[chs[i]]++;
    // }

    // for(int i = 0; i < (int) xn.size()/2; i+=2) {
    //     map_0_pred[chs[i] - xn[i]]++;
    // }
    // plt::figure(1);
    // vector<int> samples;
    // vector<int> occur;

    // for(auto i : map_ch0) {
    //     samples.push_back(i.first);
    //     occur.push_back(i.second);
    // }
    // plt::bar(samples, occur);
    // plt::xlabel("Samples ");
    // plt::ylabel("Number of occurences");
    // plt::title("Channel 0");

    // plt::figure(2);
    // vector<int> samples1;
    // vector<int> occur1;

    // for(auto i : map_0_pred) {
    //     samples1.push_back(i.first);
    //     occur1.push_back(i.second);
    // }
    // plt::bar(samples1, occur1);
    // plt::xlabel("Samples ");
    // plt::ylabel("Number of occurences");
    // plt::title("Channel pred");

    // plt::show();

    // Golomb
    // mean = mean / chs.size();
    // cout << "Mean: " << mean << endl;
    // int m = ceil(-1/(log2(mean/(mean+1))));

    int m = 5000;

    cout << "started encoding..." << endl; 

    Golomb g(fileDst, 'e', m);

    g.encodeHeaderSound(sfinfo.frames, sfinfo.samplerate, sfinfo.channels, sfinfo.format);
    
    for(int i = 0; i < rn1.size(); i++) {
        g.encode(rn1[i]);
    }
}