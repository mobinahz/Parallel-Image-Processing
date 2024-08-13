#ifndef FILTERS_HPP
#define FILTERS_HPP

#include "bmp.hpp"

void verticalMirror(std::vector<std::vector<Pixel> > &picture, int &rows, int &cols);

Pixel convolution(std::vector<std::vector<Pixel> > &picture, int &rowIndex, int &colIndex);

void purpleHaze(std::vector<std::vector<Pixel> > &picture, int &rows, int &cols);

void blurFilter(std::vector<std::vector<Pixel> > &picture, int &row, int &cols);

void hatchingFilter(std::vector<std::vector<Pixel> > &picture, int &rows, int &cols);



#endif