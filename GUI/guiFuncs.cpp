//
// Created by Ritik on 1/1/2024.
//


#include "guiFuncs.h"
#include "guiColors.h"

#include <opencv2/opencv.hpp>
#include <opencv2/core/mat.hpp>
#include <iostream>
#include "../mouseFuncs.h"


using namespace cv;
using namespace std;

namespace gF {

    vector<int> colorPicker = {-1,-1,0};
    string colorPickerLabel = "Tablet Color";
    string colorPickerChosen = "Choose a color pixel...";
    bool choosingColor = false;

    vector<int> mouseEnabled = {-1,-1,0};
    string mouseEnabledLabel = "Enable Tablet (Right Shift)";
    string mouseEnabledChosen = "Press 'Right Shift' to disable";

    vector<int> rotateButton = {-1,-1,0};
    string rotateButtonLabel = "Rotate";

    vector<int> mirrorButton = {-1,-1,0};
    string mirrorButtonLabel = "Mirror";

    vector<int> leftHandedButton = {-1,-1,0};
    string leftHandedButtonLabel = "Left Handed";

    vector<int> baseColor = {255,255,255};
    int baseDistance = 350;
    int tipExtra = 35;
    int tabletRotIndex = 0;

    int initialGUIY = 0;

    vector<vector<int>> sliders;
    vector<bool> slidersDrag;
    vector<string> sliderLabels;
    vector<reference_wrapper<int>> sliderRefs = {ref(baseDistance), ref(tipExtra)};

    bool drawButton(Mat &canvas, int x, int y, int size, Scalar color, Scalar hoverColor, Scalar outline, String text, int mX, int mY) {

        int thickness = 2;

        bool hover = (mX >= x) && (mX <= x + 2 * size) && (mY >= y) && (mY <= y + 2 * size);
        rectangle(canvas, Point_(x - thickness, y - thickness), Point_(x + 2 * size + thickness, y + 2 * size + thickness), outline, -1);

        if (hover) {
            rectangle(canvas,
                      Point_(x, y),
                      Point_(x + 2 * size, y + 2 * size),
                      hoverColor, -1);
        } else {
            rectangle(canvas,
                      Point_(x, y),
                      Point_(x + 2 * size, y + 2 * size),
                      color, -1);
        }

        if (text.length() > 0) {

            drawText(canvas,
                    text,
                    x + 2 * size + 10, y + size + 5,
                    gC::TEXT_COLOR[0], gC::TEXT_COLOR[1], gC::TEXT_COLOR[2]);

        }

        return hover;


    }

    void drawSliderGUI(Mat &canvas) {

        int buttonSize = 10;

        for (int i = 0; i < sliders.size(); i++) {

            vector<int>& slider = sliders[i];

            if (!mF::clicked) {
                slidersDrag[i] = false;
            }

            int x = slider[0] + slider[3];
            int y = slider[1];

            line(canvas,
                 Point_(slider[0], slider[1]),
                 Point_(slider[0] + slider[2], slider[1]),
                 gC::BUTTON, 3);

            if (!slidersDrag[i]) {
                bool hover = drawButton(canvas, x - buttonSize, y - buttonSize, buttonSize,
                                        gC::BUTTON_UNFILLED,
                                        gC::BUTTON_HOVER,
                                        gC::BUTTON, "", mF::mouseX, mF::mouseY);
                if (hover && mF::justClicked) {
                    slidersDrag[i] = true;
                    slider[4] = slider[3];
                }
            } else {
                drawButton(canvas, x - buttonSize, y - buttonSize, buttonSize,
                           gC::BUTTON_FILLED,
                           gC::BUTTON_HOVER_FILLED,
                           gC::BUTTON, "", mF::mouseX, mF::mouseY);
            }


            if (slidersDrag[i]) {
                int dx = mF::mouseX - mF::clickPos[0];
                int newPos = slider[4] + dx;
                if (newPos < 0) newPos = 0;
                if (newPos > slider[2]) newPos = slider[2];
                slider[3] = newPos;
            }

            int sliderVal = ((float) slider[3] / slider[2]) * (slider[6] - slider[5]) + slider[5];

            drawText(canvas, //target image
                    sliderLabels[i] + to_string(sliderVal), //text
                    slider[0], slider[1] - 17, //top-left position
                    gC::TEXT_COLOR[0], gC::TEXT_COLOR[1], gC::TEXT_COLOR[2]); //font color

            int& val = sliderRefs[i];
            val = sliderVal;



        }

    }

    void drawButtonGUI(Mat& canvas){

        int buttonSize = 10;
        Scalar_ baseColorScalar = Scalar_(baseColor[0], baseColor[1], baseColor[2]);

        if (!choosingColor) {
            bool hover = gF::drawButton(canvas, colorPicker[0], colorPicker[1], buttonSize,
                                        baseColorScalar,
                                        baseColorScalar*0.75,
                                        gC::BUTTON, colorPickerLabel, mF::mouseX, mF::mouseY);
            if (hover && mF::justClicked) {
                choosingColor = true;
            }
        } else {
            gF::drawButton(canvas, colorPicker[0], colorPicker[1],buttonSize,
                           gC::BUTTON_FILLED,
                           gC::BUTTON_HOVER_FILLED,
                           gC::BUTTON,
                           colorPickerChosen, mF::mouseX, mF::mouseY);
        }

        bool mouseButtonHover;
        if (mouseEnabled[2] == 0) {
            mouseButtonHover = gF::drawButton(canvas, mouseEnabled[0], mouseEnabled[1], buttonSize,
                                              gC::BUTTON_UNFILLED,
                                              gC::BUTTON_HOVER,
                                              gC::BUTTON, mouseEnabledLabel, mF::mouseX, mF::mouseY);
        } else {
            mouseButtonHover = gF::drawButton(canvas, mouseEnabled[0], mouseEnabled[1], buttonSize,
                                              gC::BUTTON_FILLED,
                                              gC::BUTTON_HOVER_FILLED,
                                              gC::BUTTON, mouseEnabledChosen, mF::mouseX, mF::mouseY);
        }
        if (mouseButtonHover && mF::justClicked) {
            mouseEnabled[2] = !mouseEnabled[2];
        }

        bool rotateHover = gF::drawButton(canvas, rotateButton[0], rotateButton[1], buttonSize,
                                          gC::BUTTON_UNFILLED,
                                          gC::BUTTON_HOVER,
                                          gC::BUTTON, rotateButtonLabel, mF::mouseX, mF::mouseY);

        if (rotateHover && mF::justClicked) {
            tabletRotIndex = (tabletRotIndex + 1) % 4;
        }


        bool mirrorButtonHover;
        if (mirrorButton[2] == 0) {
            mirrorButtonHover = gF::drawButton(canvas, mirrorButton[0], mirrorButton[1], buttonSize, gC::BUTTON_UNFILLED,
                                               gC::BUTTON_HOVER,
                                               gC::BUTTON, mirrorButtonLabel, mF::mouseX, mF::mouseY);
        } else {
            mirrorButtonHover = gF::drawButton(canvas, mirrorButton[0], mirrorButton[1], buttonSize,
                                               gC::BUTTON_FILLED,
                                               gC::BUTTON_HOVER_FILLED,
                                               gC::BUTTON, mirrorButtonLabel, mF::mouseX, mF::mouseY);
        }
        if (mirrorButtonHover && mF::justClicked) {
            mirrorButton[2] = !mirrorButton[2];
        }

        bool leftHandedButtonHover;
        if (leftHandedButton[2] == 0) {
            leftHandedButtonHover = gF::drawButton(canvas, leftHandedButton[0], leftHandedButton[1], buttonSize,
                                                   gC::BUTTON_UNFILLED,
                                                   gC::BUTTON_HOVER,
                                                   gC::BUTTON,
                                                   leftHandedButtonLabel, mF::mouseX, mF::mouseY);
        } else {
            leftHandedButtonHover = gF::drawButton(canvas, leftHandedButton[0], leftHandedButton[1], buttonSize,
                                                   gC::BUTTON_FILLED,
                                                   gC::BUTTON_HOVER_FILLED,
                                                   gC::BUTTON,
                                                   leftHandedButtonLabel, mF::mouseX, mF::mouseY);
        }
        if (leftHandedButtonHover && mF::justClicked) {
            leftHandedButton[2] = !leftHandedButton[2];
        }

    }

    int guiGap(int add){
        initialGUIY+=add;
        return initialGUIY;
    }

    void initSlider(){
        sliders.push_back({-1, -1, 200, 50, -1, 1, 765});
        slidersDrag.push_back(false);
        sliderLabels.push_back("Difference Threshold: ");

        sliders.push_back({-1, -1, 200, 50, -1, 1, 400});
        slidersDrag.push_back(false);
        sliderLabels.push_back("Tip Size: ");

    }
    void setGUIPos(vector<int> camPos, int camWidth, int tabletCamHeight){
        initialGUIY = tabletCamHeight;

        sliders[0][0] = camPos[0] + camWidth + 20;
        sliders[0][1] = guiGap(75);

        sliders[1][0] = camPos[0] + camWidth + 20;
        sliders[1][1] = guiGap(55);

        colorPicker = {camPos[0] + camWidth + 20, guiGap(30), 0};

        mouseEnabled[0] = camPos[0] + camWidth + 20;
        mouseEnabled[1] = guiGap(30);

        rotateButton = {camPos[0] + camWidth + 20, guiGap(30)};

        mirrorButton[0] = camPos[0] + camWidth + 20;
        mirrorButton[1] = guiGap(30);

        leftHandedButton[0] = camPos[0] + camWidth + 20;
        leftHandedButton[1] = guiGap(30);
    }


    void drawText(Mat& img, String text, int x, int y, int r, int g, int b){

        putText(img, //target image
                text, //text
                cv::Point(x, y), //top-left position
                cv::FONT_HERSHEY_SIMPLEX,
                0.5,
                CV_RGB(r,g,b), //font color
                1, LINE_AA);


    }

    void drawTextOutline(Mat& img, String text, int x, int y, int r, int g, int b){

        putText(img, //target image
                text, //text
                cv::Point(x, y), //top-left position
                cv::FONT_HERSHEY_SIMPLEX,
                0.5,
                CV_RGB(0,0,0), //font color
                5, LINE_AA);

        putText(img, //target image
                text, //text
                cv::Point(x, y), //top-left position
                cv::FONT_HERSHEY_SIMPLEX,
                0.5,
                CV_RGB(r,g,b), //font color
                1, LINE_AA);


    }



} // gF