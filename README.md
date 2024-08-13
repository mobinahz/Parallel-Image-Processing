Image filters and effects are applied to a BMP24 image file both sequentially and in parallel using POSIX threads. The program accepts an image as input, reads it through the fully implemented Bmp class `bmp.hpp`, and applies the desired filters, which are defined within the filter namespace `filter.hpp`.

Each filter operates on a BmpView, which is a custom view of the original Bmp (this could be the entire image by default), and modifies the original image pixels.

In the provided example `main.cpp`, three filters—vertical flip, Gaussian blur, and purple haze—are applied, and the result is saved as output.bmp.

For the parallel version, a thread pool is implemented using pthreads. Tasks are added to a mutex-protected queue, which are then executed by threads. In the example `main.cpp`, the image is divided into 8 BmpViews (one for each thread), and the filter tasks are executed concurrently by the threads.

To run the program:
```CPP
./ImageFilters.out <Input_Image>
```
