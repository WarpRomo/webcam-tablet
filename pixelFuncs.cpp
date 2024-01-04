//
// Created by Ritik on 1/1/2024.
//

#include <iostream>
#include <cmath>
#include <vector>
#include "pixelFuncs.h"

using namespace std;

namespace pF {

    extern void highlightPixel(uint8_t*& pixelPtr, vector<int>& color, double power, int pixel){

        double invPower = 1 - power;

        pixelPtr[pixel + 0] = (int)(pixelPtr[pixel + 0]*invPower);
        pixelPtr[pixel + 1] = (int)(pixelPtr[pixel + 1]*invPower);
        pixelPtr[pixel + 2] = (int)(pixelPtr[pixel + 2]*invPower);

        pixelPtr[pixel + 0] += (int)(color[0]*power);
        pixelPtr[pixel + 1] += (int)(color[1]*power);
        pixelPtr[pixel + 2] += (int)(color[2]*power);

    }

    extern int rgbDistance(uint8_t*& pixelPtr, vector<int>& color, int pixel){

        return abs(pixelPtr[pixel] - color[0]) +
            abs(pixelPtr[pixel+1] - color[1]) +
            abs(pixelPtr[pixel+2] - color[2]);

    }


} // pF