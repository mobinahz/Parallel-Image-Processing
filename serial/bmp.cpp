#include "bmp.hpp"

#include <fstream>
#include <iostream>
#include <vector>

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

void getPixelsFromBMP24(int end, int &rows, int &cols, char* fileReadBuffer, std::vector<std::vector <Pixel> > &picture) {
    
    auto startTime = std::chrono::high_resolution_clock::now();
    int count = 1;
    int extra = cols % 4;

    for (int i = 0; i < rows; i++) {
        count += extra;
        for (int j = cols - 1; j >= 0; j--) {
            for (int k = 0; k < 3; k++) {
                unsigned char channel = fileReadBuffer[end - count];
                switch (k) {
                case 0:
                    // fileReadBuffer[end - count] is the red value
                    picture[i][j].red = channel;
                    break;
                case 1:
                    // fileReadBuffer[end - count] is the green value
                    picture[i][j].green = channel;
                    break;
                case 2:
                    // fileReadBuffer[end - count] is the blue value
                    picture[i][j].blue = channel;
                    break;
                }
                // go to the next position in the buffer
                count++;
                
            }
        }
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    std::cout << "Get pixels Execution Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count()<< "ms" << std::endl;

    
}

void writeOutBmp24(char* fileBuffer, const char* nameOfFileToCreate, int bufferSize, int &rows, int &cols, std::vector <std::vector<Pixel> > picture) {
    
    auto startTime = std::chrono::high_resolution_clock::now();
    std::ofstream write(nameOfFileToCreate);
    if (!write) {
        std::cout << "Failed to write " << nameOfFileToCreate << std::endl;
        return;
    }

    int count = 1;
    int extra = cols % 4;
    for (int i = 0; i < rows; i++) {
        count += extra;
        for (int j = cols - 1; j >= 0; j--) {
            for (int k = 0; k < 3; k++) {
                switch (k) {
                case 0:
                    // write red value in fileBuffer[bufferSize - count]
                    fileBuffer[bufferSize - count] = char(picture[i][j].red);
                    break;
                case 1:
                    // write green value in fileBuffer[bufferSize - count]
                    fileBuffer[bufferSize - count] = char(picture[i][j].green);
                    break;
                case 2:
                    // write blue value in fileBuffer[bufferSize - count]
                    fileBuffer[bufferSize - count] = char(picture[i][j].blue);
                    break;
                }
                // go to the next position in the buffer
                count++;
            }
        }
    }
    write.write(fileBuffer, bufferSize);
    auto endTime = std::chrono::high_resolution_clock::now();
    std::cout << "Write Execution Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count()<< "ms" << std::endl;

    
}
