#ifndef CODECAUD_H
#define CODECAUD_H

#include <stdio.h>
#include <sndfile.h>
#include <vector>

class Codecaud{
    private:
        char* filename;
        SF_INFO sfinfo;
        std::vector<short> chs = {};
        // filename -> nome do fich codificado
        void encode(const char *filename);

    public:
        Codecaud();
        Codecaud(const char *filename);
        void compress(const char *fileDst);
        void decompress(const char *fileSrc);
};

#endif