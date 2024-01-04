//
// Created by Ritik on 1/1/2024.
//

#ifndef HELLOWORLD_GUICOLORS_H
#define HELLOWORLD_GUICOLORS_H

#include <opencv2/opencv.hpp>
#include <opencv2/core/mat.hpp>

using namespace cv;

namespace gC {

    extern Scalar BACKGROUND;

    extern Scalar LAYER;

    extern Scalar BUTTON;
    extern Scalar BUTTON_HOVER;

    extern Scalar BUTTON_UNFILLED;
    extern Scalar BUTTON_FILLED;
    extern Scalar BUTTON_HOVER_FILLED;

    extern Scalar LINE;

    extern Scalar TEXT_COLOR;
    extern Scalar SOFT_TEXT_COLOR;

    extern Scalar WINDOW_OUTLINE;


    extern int BUTTON_SIZE;

} // gC

#endif //HELLOWORLD_GUICOLORS_H
