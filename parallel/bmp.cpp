#include "bmp.hpp"

#include <fstream>
#include <iostream>
#include <vector>

int rows;
int cols;


bool fillAndAllocate(char*& buffer, const char* fileName, int& rows, int& cols, int& bufferSize) {
    std::ifstream file(fileName);
    if (!file) {
        std::cout << "File" << fileName << " doesn't exist!" << std::endl;
        return false;
    }

    file.seekg(0, std::ios::end);
    std::streampos length = file.tellg();
    file.seekg(0, std::ios::beg);

    buffer = new char[length];
    file.read(&buffer[0], length);

    PBITMAPFILEHEADER file_header;
    PBITMAPINFOHEADER info_header;

    file_header = (PBITMAPFILEHEADER)(&buffer[0]);
    info_header = (PBITMAPINFOHEADER)(&buffer[0] + sizeof(BITMAPFILEHEADER));
    rows = info_header->biHeight;
    cols = info_header->biWidth;
    bufferSize = file_header->bfSize;
    return true;
}

void * threadGetPixels(void * args) {
    struct Arguments * arguments;
    arguments = (struct Arguments *) args;
    int cols = arguments->cols;
    int rows = arguments->rows;
    int row = arguments->row;
    int row2 = arguments->row2;
    int col = arguments->col;
    int col2 = arguments->col2;
    int idx = arguments->extra; // index
    int extra = arguments->extra2; // col offset
    std::vector<std::vector<Pixel> >* pic = arguments->picture;

    for (int i = row; i < row2; i++) {
        idx -= extra;
        for (int j = cols - 1; j >= 0; j--) {
            for (int k = 0; k < 3; k++) {
                idx--;
                auto c = (unsigned char) arguments->buffer[idx];
                switch (k) {
                    case 0:
                        (*pic)[i][j].red = c;
                        break;
                    case 1:
                        (*pic)[i][j].green = c;
                        break;
                    case 2:
                        (*pic)[i][j].blue = c;
                        break;
                }

            }
        }
    }
    return nullptr;

}

void getPixelsFromBMP24(int end, int &rows, int &cols, char* fileReadBuffer, std::vector<std::vector <Pixel> > &picture, int threadNumber) {
   
    auto startTime = std::chrono::high_resolution_clock::now();

    int count = 1;
    int extra = cols % 4;
    int idx = end;
    int total = (3 * cols + extra) * rows;

    while(total % threadNumber != 0) {
        threadNumber--;
    }

    int partition = rows / threadNumber;

    pthread_t threads[threadNumber];
    Arguments arguments[threadNumber];

    int rowBound = 0;


    for(int i = 0; i < threadNumber; i++) {
        arguments[i].cols = cols;
        arguments[i].rows = rows;
        arguments[i].col = 0;
        arguments[i].col2 = cols - 1;
        arguments[i].row = rowBound;
        rowBound += partition;
        if(i == (threadNumber - 1)) {
            arguments[i].row2 = rows;
        } else{
            arguments[i].row2 = rowBound;
        }
        arguments[i].extra = idx;
        arguments[i].extra2 = extra;
        arguments[i].picture = &picture;
        arguments[i].buffer = fileReadBuffer;

        pthread_create(&threads[i], nullptr, threadGetPixels, &arguments[i]);
        idx -= (3*cols + extra) * partition;

    }

    for (int i = 0; i < threadNumber; i++) {
        pthread_join(threads[i], nullptr);
    }


    auto endTime = std::chrono::high_resolution_clock::now();
    std::cout << "Get pixels Execution Time:" << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count()<< "ms" << std::endl;

}

void* threadWriteOut(void* args) {
    struct Arguments *arguments;
    arguments = (struct Arguments *) args;
    int extra = arguments->extra2; // col offset
    int idx = arguments->extra; // index
    int cols = arguments->cols;
    int row = arguments->row;
    int row2 = arguments->row2;
    std::vector<std::vector<Pixel> > *image = arguments->picture;
    for (int i = row; i < row2; i++) {
        idx -= extra;
        for (int j = cols - 1; j >= 0; j--) {
            for (int k = 0; k < 3; k++) {
                idx--;
                switch (k) {
                    case 0:
                        arguments->buffer[idx] = char(arguments->picture->at(i)[j].red);
                        break;
                    case 1:
                        arguments->buffer[idx] = char(arguments->picture->at(i)[j].green);
                        break;
                    case 2:
                        arguments->buffer[idx] = char(arguments->picture->at(i)[j].blue);
                        break;
                }
            }
        }
    }
    return nullptr;
}

void writeOutBmp24(char* fileBuffer, const char* nameOfFileToCreate, int bufferSize, int &rows, int &cols, std::vector <std::vector<Pixel> > &picture, int threadNumber) {
    
    auto startTime = std::chrono::high_resolution_clock::now();
    std::ofstream write(nameOfFileToCreate);
    if (!write) {
        std::cout << "Failed to write " << nameOfFileToCreate << std::endl;
        return;
    }

    int count = 1;
    int extra = cols % 4;
    int idx = bufferSize;
    int total = (3 * cols + extra) * rows;
    int partition = rows / threadNumber;

    while(total % threadNumber != 0) {
        threadNumber--;
    }
    pthread_t threads[threadNumber];
    Arguments arguments[threadNumber];
    int rowBound = 0;

    for (int i = 0; i < threadNumber; i++) {

        arguments[i].cols = cols;
        arguments[i].rows = rows;
        arguments[i].threadNum = threadNumber;
        arguments[i].col = 0;
        arguments[i].col2 = cols - 1;
        arguments[i].row = rowBound;
        rowBound += partition;
        if(i == (threadNumber - 1)) {
            arguments[i].row2 = rows;
        } else{
            arguments[i].row2 = rowBound;
        }
        arguments[i].extra = idx;
        arguments[i].extra2 = extra;
        arguments[i].picture = &picture;
        arguments[i].buffer = fileBuffer;


        pthread_create(&threads[i], nullptr, threadWriteOut, &arguments[i]);
        idx -= (3 * cols + extra) * partition;
    }

    for (int i = 0; i < threadNumber; i++) {
        pthread_join(threads[i], nullptr);
    }
    write.write(fileBuffer, bufferSize);

    auto endTime = std::chrono::high_resolution_clock::now();
    std::cout << "Write Execution Time:" << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count()<< "ms" << std::endl;

    
}
