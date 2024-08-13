#include "filters.hpp"

int GaussianBlurKernel[3][3] = {
        {1, 2, 1},
        {2, 4, 2},
        {1, 2, 1}
};

Pixel convolution(std::vector<std::vector<Pixel> > &picture, int &rowIndex, int &colIndex) {
    int red = 0, green = 0, blue = 0;
    for(int i = -1; i < 2; i++) {
        for(int j = -1; j < 2; j++) {
            red += picture[rowIndex + i][colIndex + j].red * GaussianBlurKernel[i + 1][j + 1] / 16;
            green += picture[rowIndex + i][colIndex + j].green * GaussianBlurKernel[i + 1][j + 1] / 16;
            blue += picture[rowIndex + i][colIndex + j].blue * GaussianBlurKernel[i + 1][j + 1] / 16;
        }
    }
    Pixel temp;
    temp.red = unsigned(red);
    temp.green = unsigned(green);
    temp.blue = unsigned(blue);
    return temp;
}

void blurFilter(std::vector<std::vector<Pixel> > &picture, int &row, int &cols) {

    auto startTime = std::chrono::high_resolution_clock::now();

    std::vector<std::vector<Pixel> > bluredpicture(row, std::vector<Pixel>(cols));

    for(int i = 0; i < cols; i++) {
        bluredpicture[0][i] = picture[0][i];
        bluredpicture[row - 1][i] = picture[row - 1][i];
    }

    for(int j = 0; j < row; j++) {
        bluredpicture[j][0] = picture[j][0];
        bluredpicture[j][cols - 1] = picture[j][cols - 1];
    }

    for(int i = 1; i < row - 1; i++) {
        for(int j = 1; j < cols - 1; j++) {
            bluredpicture[i][j] = convolution(picture, i, j);
        }
    }

    picture = bluredpicture;

    auto endTime = std::chrono::high_resolution_clock::now();
    std::cout << "Blur Execution Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count()<< "ms" << std::endl;

}

void hatchingFilter(std::vector<std::vector<Pixel> > &picture, int &rows, int &cols) {

    auto startTime = std::chrono::high_resolution_clock::now();


    float grad = (float)rows / cols;


    for (int i = 0; i < cols; i++) {
        int j = (rows * i)/cols;

        picture[rows - j -1][i].red = 255;
        picture[rows - j -1][i].blue = 255;
        picture[rows - j -1][i].green = 255;
    }

    for (int i = 0; i < cols/2; i++) {
        picture[rows - 1 - i * grad][cols/2 + i].red = 255;
        picture[rows - 1 - i * grad][cols/2 + i].green = 255;
        picture[rows - 1 - i * grad][cols/2 + i].blue = 255;
    }

    for (int i = 0; i < cols/2; i++) {
        picture[rows/2 - i * grad][i].red = 255;
        picture[rows/2 - i * grad][i].green = 255;
        picture[rows/2 - i * grad][i].blue = 255;
    }

    

    auto endTime = std::chrono::high_resolution_clock::now();
    std::cout << "Hatching Filter Execution Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count()<< "ms" << std::endl;

}



void verticalMirror(std::vector<std::vector<Pixel> > &picture, int &rows, int &cols)
{
    auto startTime = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < rows /2; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            Pixel temp = picture[i][j];
            picture[i][j] = picture[rows -i -1][j];
            picture[rows -i -1][j] = temp;
        }
    }
    auto endTime = std::chrono::high_resolution_clock::now();
    std::cout << "Vertical Mirror Execution Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count()<< "ms" << std::endl;
}

void purpleHaze(std::vector<std::vector<Pixel> > &picture, int &rows, int &cols) {

    auto startTime = std::chrono::high_resolution_clock::now();

    int redd, greenn, bluee;

    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            Pixel pix;
            redd = (0.5 * picture[i][j].red) + (0.3 * picture[i][j].green) + (0.5 * picture[i][j].blue);
            greenn = (0.16 * picture[i][j].red) + (0.5 * picture[i][j].green) + (0.16 * picture[i][j].blue);
            bluee = (0.6 * picture[i][j].red) + (0.2 * picture[i][j].green) + (0.8 * picture[i][j].blue);
            // redd = (0.5 * args->picture->at(i)[j].red) + (0.3 * args->picture->at(i)[j].green) + (0.5 * args->picture->at(i)[j].blue);
            // greenn = (0.16 * args->picture->at(i)[j].red) + (0.5 * args->picture->at(i)[j].green) + (0.16 * args->picture->at(i)[j].blue);
            // bluee = (0.6 * args->picture->at(i)[j].red) + (0.2 * args->picture->at(i)[j].green) + (0.8 * args->picture->at(i)[j].blue);



            if(redd > 255) {
                redd = 255;
            }
            if(redd < 0) {
                redd = 0;
            }
            if(greenn > 255) {
                greenn = 255;
            }
            if(greenn < 0) {
                greenn = 0;
            }
            if(bluee > 255) {
                bluee = 255;
            }
            if(bluee < 0) {
                bluee = 0;
            }

            picture[i][j].red = unsigned(redd);
            picture[i][j].blue = unsigned(bluee);
            picture[i][j].green = unsigned(greenn);
            // picture[i][j].blue = uint(blue);
        }
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    std::cout << "Purple Haze Execution Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count()<< "ms" << std::endl;

}

