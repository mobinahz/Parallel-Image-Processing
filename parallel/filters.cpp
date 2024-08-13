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

void* threadBlurFilter(void* arguments) {
    struct Arguments* args;
    args = (struct Arguments*) arguments;


    for(int i = args->row; i < args->row2; i++) {
        if(i == 0)
            continue;
        
        for(int j = 1; j < args->cols - 1; j++) {
            (*args->picture2)[i][j] = convolution(*args->picture, i, j);
        }
    }

    return nullptr;
    
}

void blurFilter(std::vector<std::vector<Pixel> > &picture, int &rows, int &cols, int threadNumber) {

    auto startTime = std::chrono::high_resolution_clock::now();

    pthread_t *threads = new pthread_t[threadNumber];

    int partition = rows / threadNumber;
    Arguments *args = new Arguments[threadNumber];
    std::vector<std::vector<Pixel> > bluredpicture(rows, std::vector<Pixel>(cols));

    for (int i = 0; i < threadNumber; i++) {
        args[i].picture = &picture;
        args[i].picture2 = &bluredpicture;
        args[i].cols = cols;
        int start = i * partition;

        if (start == 0) 
            start = 1;

        int end = (i + 1) * partition;

        if (i == (threadNumber - 1)) 
            end = rows - 1;

        args[i].row = start; 
        args[i].row2 = end;
        pthread_create(&threads[i], nullptr, reinterpret_cast<void *(*)(void *)>(threadBlurFilter),
                       (void *) &args[i]);
        // pthread_create(&threads[i], nullptr, threadBlurFilter, &args[i]);
    }

    for (int i = 0; i < threadNumber; i++) {
        pthread_join(threads[i], nullptr);
    }

    picture = bluredpicture;

    delete[] threads;
    delete[] args;

    auto endTime = std::chrono::high_resolution_clock::now();
    std::cout << "Blur Execution Time:" << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count()<< "ms" << std::endl;

}

void* threadHatchingFilter(void* arguments) {

    struct Arguments* args;
    args = (struct Arguments*) arguments;

    for(int i = args->row; i < args->row2; i++){
        for(int j=0; j < args->cols; j++){
            if ( args->picture2->at(i)[j].red != 255)
            {
                args->picture->at(i)[j].red = args->picture->at(i)[j].red + args->picture2->at(i)[j].red;
                args->picture->at(i)[j].green = args->picture->at(i)[j].green + args->picture2->at(i)[j].green;
                args->picture->at(i)[j].blue = args->picture->at(i)[j].blue + args->picture2->at(i)[j].blue;
            }
            else
            {
                args->picture->at(i)[j] = args->picture2->at(i)[j];
            }
        }
    }
    return nullptr;
}


void hatchingFilter2(std::vector<std::vector<Pixel> > &picture, int &rows, int &cols, int threadNumber) {

    auto startTime = std::chrono::high_resolution_clock::now();

    pthread_t *threads = new pthread_t[threadNumber];
    int partition = rows / threadNumber;
    auto *args = new Arguments[threadNumber];

    std::vector<std::vector<Pixel> > temp(rows, std::vector<Pixel>(cols));

    int grad = rows / cols;


    for (int i = 0; i < cols; i++) {
        int j = (rows * i)/cols;

        temp[rows - j -1][i].red = 255;
        temp[rows - j -1][i].blue = 255;
        temp[rows - j -1][i].green = 255;
    }

    for (int i = 0; i < cols/2; i++) {
        temp[rows - 1 - i * grad][cols/2 + i].red = 255;
        temp[rows - 1 - i * grad][cols/2 + i].green = 255;
        temp[rows - 1 - i * grad][cols/2 + i].blue = 255;
    }

    for (int i = 0; i < cols/2; i++) {
        temp[rows/2 - i * grad][i].red = 255;
        temp[rows/2 - i * grad][i].green = 255;
        temp[rows/2 - i * grad][i].blue = 255;
    }

    for (int i = 0; i < threadNumber; i++) {
        args[i].picture = &picture;
        args[i].picture2 = &temp;
        args[i].cols = cols;

        int bound = i * partition;

        bound = (!bound) ? 1 : bound;

        int end = (i + 1) * partition;

        end = (i == threadNumber - 1) ? rows : end;

        args[i].row = bound;
        args[i].row2 = end;

        pthread_create(&threads[i], nullptr, reinterpret_cast<void *(*)(void *)>(threadHatchingFilter),
                       (void *) &args[i]);
    }


    for (int i = 0; i < threadNumber; i++) {
        pthread_join(threads[i], nullptr);
    }

    delete[] threads;
    delete[] args;

    auto endTime = std::chrono::high_resolution_clock::now();
    std::cout << "Hatching Filter Execution Time:" << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count()<< std::endl;

}

void hatchingFilter(std::vector<std::vector<Pixel> > &picture, int &rows, int &cols) {

    auto startTime = std::chrono::high_resolution_clock::now();

    std::vector<std::vector<Pixel> > temp(rows, std::vector<Pixel>(cols));

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

void* threadVerticalMirror(void* arguments) {

    struct Arguments* args;
    args = (struct Arguments*) arguments;

    for (int i = 0; i < args->rows/2; i++)
    {
        for (int j = args->col; j < args->col2; j++)
        {
            Pixel temp = (*args->picture)[i][j];
            (*args->picture)[i][j] = (*args->picture)[args->rows -i -1][j];
            (*args->picture)[args->rows -i -1][j] = temp;
        }
    }

    return nullptr;
    
}



void verticalMirror(std::vector<std::vector<Pixel> > &picture, int &rows, int &cols, int threadNumber)
{

    auto startTime = std::chrono::high_resolution_clock::now();

    pthread_t *threads = new pthread_t[threadNumber];
    int partition = rows / threadNumber;
    auto *args = new Arguments[threadNumber];

    for (int i = 0; i < threadNumber; i++) {
        args[i].picture = &picture;
        args[i].rows = rows;
        int bound = i * partition;
        int end = (i + 1) * partition;

        if (i == threadNumber - 1) 
            end = cols;

        args[i].col = bound;
        args[i].col2 = end;
        args[i].cols = cols;

        pthread_create(&threads[i], nullptr, reinterpret_cast<void *(*)(void *)>(threadVerticalMirror),
                       (void *) &args[i]);
    }
    for (int i = 0; i < threadNumber; i++) {
        pthread_join(threads[i], nullptr);
    }
    delete[] threads;
    delete[] args;

    auto endTime = std::chrono::high_resolution_clock::now();
    std::cout << "Vertical Mirror Execution Time:" << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count()<< "ms" << std::endl;

}

void* threadPurpleFilter(void* arguments) {

    
    struct Arguments* args;
    args = (struct Arguments *) arguments;


    int redd, greenn, bluee;
    for(int i = args->row; i < args->row2; i++) {
        for(int j = 0; j < args->cols; j++) {
            Pixel pix;
    
            redd = (0.5 * args->picture->at(i)[j].red) + (0.3 * args->picture->at(i)[j].green) + (0.5 * args->picture->at(i)[j].blue);
            greenn = (0.16 * args->picture->at(i)[j].red) + (0.5 * args->picture->at(i)[j].green) + (0.16 * args->picture->at(i)[j].blue);
            bluee = (0.6 * args->picture->at(i)[j].red) + (0.2 * args->picture->at(i)[j].green) + (0.8 * args->picture->at(i)[j].blue);


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

            args->picture->at(i)[j].red = unsigned(redd);
            args->picture->at(i)[j].green = unsigned(greenn);
            args->picture->at(i)[j].blue = unsigned(bluee);
        }
    }
    return nullptr;

}

void purpleHaze(std::vector<std::vector<Pixel> > &picture, int &rows, int &cols, int threadNumber) {

    auto startTime = std::chrono::high_resolution_clock::now();

    pthread_t *threads = new pthread_t[threadNumber];
    int partition = rows / threadNumber;
    Arguments *args = new Arguments[threadNumber];

    for (int i = 0; i < threadNumber; i++) {
        args[i].picture = &picture;
        args[i].cols = cols;
        int start = i * partition;
        if (start == 0) {
            start = 1;
        }

        int end = (i + 1) * partition;

        

        if (i == (threadNumber - 1)) {
            end = end + (rows % threadNumber);
        }

        args[i].row = start;
        args[i].row2 = end;

        pthread_create(&threads[i], nullptr, threadPurpleFilter, &args[i]);
    }

    for (int i = 0; i < threadNumber; i++) {
        pthread_join(threads[i], nullptr);
    }

    delete[] threads;
    delete[] args;

    auto endTime = std::chrono::high_resolution_clock::now();
    std::cout << "Purple Haze Execution Time:" << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count()<< "ms" << std::endl;


}

