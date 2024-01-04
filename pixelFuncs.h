//
// Created by Ritik on 1/1/2024.
//

#ifndef HELLOWORLD_PIXELFUNCS_H
#define HELLOWORLD_PIXELFUNCS_H

using namespace std;

namespace pF {

    void highlightPixel(uint8_t*& pixelPtr, vector<int>& color, double power, int pixel);
    int rgbDistance(uint8_t*& pixelPtr, vector<int>& color, int pixel);

} // pF

#endif //HELLOWORLD_PIXELFUNCS_H
