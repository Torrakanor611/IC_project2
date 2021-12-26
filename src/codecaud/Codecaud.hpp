#ifndef CODECAUD_H
#define CODECAUD_H

#include <stdio.h>
#include <sndfile.h>
#include <vector>

class Codecaud{
    private:
        char* filename;
        SF_INFO sfinfo;
        int ninput;
        std::vector<short> chs = {};
        std::vector<short> rn = {};
        
    public:
        Codecaud();
        Codecaud(const char *filename);
        void compress(const char *fileDst, int num);
        void decompress(const char *fileSrc);
        void showHistEnt();
};

#endif