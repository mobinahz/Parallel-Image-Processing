#ifndef BMP_HPP
#define BMP_HPP

#include <fstream>
#include <iostream>
#include <vector>
#include <unistd.h>
#include <ctime>
#include <pthread.h>


typedef int LONG;
typedef unsigned short WORD;
typedef unsigned int DWORD;

#pragma pack(push, 1)
typedef struct tagBITMAPFILEHEADER {
    WORD bfType;
    DWORD bfSize;
    WORD bfReserved1;
    WORD bfReserved2;
    DWORD bfOffBits;
} BITMAPFILEHEADER, *PBITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER {
    DWORD biSize;
    LONG biWidth;
    LONG biHeight;
    WORD biPlanes;
    WORD biBitCount;
    DWORD biCompression;
    DWORD biSizeImage;
    LONG biXPelsPerMeter;
    LONG biYPelsPerMeter;
    DWORD biClrUsed;
    DWORD biClrImportant;
} BITMAPINFOHEADER, *PBITMAPINFOHEADER;
#pragma pack(pop)

typedef struct Pixel {
    unsigned char red;
    unsigned char green;
    unsigned char blue;

    Pixel() {
        red = 0;
        green = 0;
        blue = 0;
    }
} Pixel;

struct Arguments{
    int cols;
    int rows;

    int col, col2;
    int row, row2;
    int extra, extra2;
    int threadNum;
    std::vector<std::vector<Pixel> >* picture;
    std::vector<std::vector<Pixel> >* picture2;
    char* buffer;
};



bool fillAndAllocate(char*& buffer, const char* fileName, int& rows, int& cols, int& bufferSize);

void getPixelsFromBMP24(int end, int &rows, int &cols, char* fileReadBuffer, std::vector<std::vector <Pixel> > &picture, int threadNumber);

void writeOutBmp24(char* fileBuffer, const char* nameOfFileToCreate, int bufferSize, int &rows, int &cols, std::vector <std::vector<Pixel> > &picture, int threadNumber);

void * threadGetPixels(void * args);

void* threadWriteOut(void* args);



#endif