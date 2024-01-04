//
// Created by Ritik on 1/1/2024.
//

#ifndef HELLOWORLD_TABLETCALC_H
#define HELLOWORLD_TABLETCALC_H

#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <iostream>
#include <cmath>
#include <Windows.h>
#include <ctime>

using namespace std;
using namespace cv;

namespace tC {

    extern vector<int> tabletRot;
    extern vector<int> buttonColor;
    extern vector<int> importantColor;
    extern double importantPower;

    Mat warpedMat(vector<int>& p1, vector<int>& p2, vector<int>& p3, vector<int>& p4, Mat& img, float width, float height, int rotation, bool mirror=false);

    vector<float> getTabletPos(Mat& out, uint8_t*& tabletPtr, vector<int> baseColor, int baseDistance, int tipExtra, bool leftHanded);

    void moveMouse(Mat& out, uint8_t*& tabletPtr, vector<int> baseColor, int baseDistance, int tipExtra, bool leftHanded, bool mouseEnabled, int screenAreaWidth, int screenAreaHeight, vector<int> screenAreaPivot);

} // tC

#endif //HELLOWORLD_TABLETCALC_H
