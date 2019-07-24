#include <iostream>
#include "EasyBMP_1.06/EasyBMP.h"
#include <omp.h>

using namespace std;

//double cReal, cImaginary;
double newReal, newImaginary, oldReal, oldImaginary;

//After how much iterations function should stop
int maximalIteration = 300;

//plane x[-1.5 +1.5] -> 0
double moveX = 0;
//plane y[-1.5 +1.5] -> 0
double moveY = 0;
double zoom = 1;

int x, y;

//Number of iterations
int i;

int width = 1000;
int height = 1000;

double fractalStart;
double fractalEnd;

RGBApixel rgbaPixel;

int threads;

BMP fractal;

void drawFractal(double cReal, double cImaginary);

int main() {
    //Values for the constant, determine shape for Julia Set
    double cReal;
    double cImaginary;

    do {
        cout << "\nEnter first constant for real number\n(must be between -0.99 and 0) :\n" << endl;
        cin >> cReal;
    } while (cReal > 0 || cReal < -0.99);

    do {
        cout << "\nEnter second constant for imaginary number\n(must be between 0 and 0.99) : \n" << endl;
        cin >> cImaginary;
    } while (cImaginary < 0 || cImaginary > 0.99);


    drawFractal(cReal, cImaginary);
}


void drawFractal(double cReal, double cImaginary) {
    omp_set_dynamic(0);
    omp_set_num_threads(4);

    fractalStart = omp_get_wtime();

    fractal.SetSize(width, height);
    fractal.SetBitDepth(24);

#pragma omp parallel for private(y, x, i, newReal, newImaginary, oldReal, oldImaginary)
    //Loop through each pixel
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            //Initial real and Imaginary part of z, based on pixel location, zoom and position values
            newReal = 1.5 * (x - width / 2) / (0.5 * zoom * width) + moveX;
            newImaginary = (y - height / 2) / (0.5 * zoom * height) + moveY;

            for(i = 0; i < maximalIteration; ++i) {
                //Keep previous values of iteration
                oldReal = newReal;
                oldImaginary = newImaginary;

                //Each iteration calculates newReal = oldReal * oldReal + const cReal
                //Actual iteration for real and imaginary part
                newReal = oldReal * oldReal - oldImaginary * oldImaginary + cReal;
                newImaginary = 2 * oldReal * oldImaginary + cImaginary;

                //If the point is outside the circle with radius 2 -> stop
                if((newReal * newReal + newImaginary * newImaginary) > 2) {
                    break;
                }
                threads = omp_get_num_threads();
            }

            //Set color on RGBA pixel, make brightness black if maximalIteration is reached
            rgbaPixel.Red = i % 45 * (i < maximalIteration);
            rgbaPixel.Blue = i % 64 * (i < maximalIteration);
            rgbaPixel.Green = i % 255 * (i < maximalIteration);

            //Draw pixel
            fractal.SetPixel(x, y, rgbaPixel);
        }
    }

    //Write in file
    fractal.WriteToFile("fractal.bmp");

    fractalEnd = omp_get_wtime();

    cout << "\nNb thread : " << threads << "\n";

    cout << "\nFractal process has been processed in \n" << (fractalEnd - fractalStart) * 1000 << " milliseconds\n" << endl;
}


