#include "bmp.hpp"
#include "filters.hpp"
#include <chrono>
#include <iostream>

#define THREAD_NUMBER 8

int main(int argc, char* argv[]) {
    char* fileBuffer;
    int bufferSize;
    int rows, cols;

    auto startTime = std::chrono::high_resolution_clock::now();
    if (!fillAndAllocate(fileBuffer, argv[1], rows, cols, bufferSize)) {
        std::cout << "File read error" << std::endl;
        return 1;
    }

    std::vector <std::vector<Pixel> > picture(rows, std::vector<Pixel>(cols));

    // read input file
    getPixelsFromBMP24(bufferSize, rows, cols, fileBuffer, picture, THREAD_NUMBER);

    // apply filters
    verticalMirror(picture, rows, cols, THREAD_NUMBER);
    blurFilter(picture, rows, cols, THREAD_NUMBER);
    purpleHaze(picture, rows, cols, THREAD_NUMBER);
    hatchingFilter(picture, rows, cols);

    // write output file
    writeOutBmp24(fileBuffer, "output.bmp", bufferSize, rows, cols, picture, THREAD_NUMBER);

    auto endTime = std::chrono::high_resolution_clock::now();
    std::cout << "Execution Time:" << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count()<< std::endl;


    return 0;
}