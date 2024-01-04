//
// Created by Ritik on 1/1/2024.
//

#include "guiColors.h"

#include <opencv2/core/mat.hpp>

using namespace cv;

namespace gC {

    Scalar BACKGROUND = Scalar(30, 30, 30);

    Scalar LAYER = Scalar(20, 20, 20);

    Scalar BUTTON = Scalar(70,70,70);
    Scalar BUTTON_HOVER =  Scalar(60, 60, 60);

    Scalar BUTTON_UNFILLED = Scalar(30, 30, 30);
    Scalar BUTTON_FILLED = Scalar(0,255,0);
    Scalar BUTTON_HOVER_FILLED = Scalar(0, 170, 0);

    Scalar LINE = Scalar(0, 255, 0);

    Scalar TEXT_COLOR = Scalar(255, 255, 255);
    Scalar SOFT_TEXT_COLOR = Scalar(40,40,40);

    Scalar WINDOW_OUTLINE = Scalar(50, 50, 50);

    int BUTTON_SIZE = 10;

} // gC