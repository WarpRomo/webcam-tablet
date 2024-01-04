//
// Created by Ritik on 1/1/2024.
//

#ifndef HELLOWORLD_GUIFUNCS_H
#define HELLOWORLD_GUIFUNCS_H

#include <opencv2/opencv.hpp>
#include <opencv2/core/mat.hpp>
#include <iostream>



using namespace cv;
using namespace std;

namespace gF {

    extern vector<int> colorPicker;
    extern string colorPickerLabel;
    extern string colorPickerChosen;
    extern bool choosingColor;

    extern vector<int> mouseEnabled;
    extern string mouseEnabledLabel;
    extern string mouseEnabledChosen;

    extern vector<int> rotateButton;
    extern string rotateButtonLabel;

    extern vector<int> mirrorButton;
    extern string mirrorButtonLabel;

    extern vector<int> leftHandedButton;
    extern string leftHandedButtonLabel;

    extern vector<int> baseColor;
    extern int baseDistance;
    extern int tipExtra;

    extern int initialGUIY;

    extern vector<vector<int>> sliders;
    extern vector<bool> slidersDrag;
    extern vector<string> sliderLabels;
    extern vector<reference_wrapper<int>> sliderRefs;

    extern int tabletRotIndex;

    bool drawButton(Mat& canvas, int x, int y, int size, Scalar color, Scalar hoverColor, Scalar outline, String text, int mX, int mY);

    void drawSliderGUI(Mat &canvas);
    void drawButtonGUI(Mat& canvas);

    void initSlider();

    void setGUIPos(vector<int> camPos, int camWidth, int tabletCamHeight);

    void drawText(Mat& img, String text, int x, int y, int r, int g, int b);
    void drawTextOutline(Mat& img, String text, int x, int y, int r, int g, int b);

} // gF

#endif //HELLOWORLD_GUIFUNCS_H
