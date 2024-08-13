#include "bmp.hpp"
#include "filters.hpp"

int main(int argc, char* argv[]) {
    char* fileBuffer;
    int bufferSize;
    int rows, cols;

    // auto startTime = std::chrono::high_resolution_clock::now();
    if (!fillAndAllocate(fileBuffer, argv[1], rows, cols, bufferSize)) {
        std::cout << "File read error" << std::endl;
        return 1;
    }

    std::vector <std::vector<Pixel> > picture(rows, std::vector<Pixel>(cols));

    auto startTime = std::chrono::high_resolution_clock::now();
    // read input file
    getPixelsFromBMP24(bufferSize, rows, cols, fileBuffer, picture);

    // apply filters
    verticalMirror(picture, rows, cols);
    blurFilter(picture, rows, cols);
    purpleHaze(picture, rows, cols);
    hatchingFilter(picture, rows, cols);

    // write output file
    writeOutBmp24(fileBuffer, "output.bmp", bufferSize, rows, cols, picture);

    auto endTime = std::chrono::high_resolution_clock::now();
    std::cout << "Total Execution Time:" << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count()<< "ms" << std::endl;

    return 0;
}