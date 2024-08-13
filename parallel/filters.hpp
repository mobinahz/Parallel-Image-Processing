#ifndef FILTERS_HPP
#define FILTERS_HPP

#include "bmp.hpp"

void verticalMirror(std::vector<std::vector<Pixel> > &picture, int &rows, int &cols, int threadNumber);

Pixel convolution(std::vector<std::vector<Pixel> > &picture, int &rowIndex, int &colIndex);

void purpleHaze(std::vector<std::vector<Pixel> > &picture, int &rows, int &cols, int threadNumber);

void blurFilter(std::vector<std::vector<Pixel> > &picture, int &row, int &cols, int threadNumber);

void hatchingFilter2(std::vector<std::vector<Pixel> > &picture, int &rows, int &cols, int threadNumber);

void hatchingFilter(std::vector<std::vector<Pixel> > &picture, int &rows, int &cols);

void* threadBlurFilter(void* arguments);

void* threadHatchingFilter(void* arguments);

void* threadVerticalMirror(void* arguments);

void* threadPurpleFilter(void* arguments);


#endif