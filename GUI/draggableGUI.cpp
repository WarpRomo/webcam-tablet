//
// Created by Ritik on 1/1/2024.
//

#include <opencv2/opencv.hpp>
#include <opencv2/core/mat.hpp>


#include "guiFuncs.h"
#include "guiColors.h"

#include "../mouseFuncs.h"

#include "draggableGUI.h"
#include "../screenRes.h"

using namespace std;
using namespace cv;

namespace dG {

    vector<vector<int>> tabletArea = {{100, 100}, {200, 100}, {200, 200}, {100, 200}};
    vector<vector<int>> tabletAreaPrev = {{100, 100}, {200, 100}, {200, 200}, {100, 200}};
    vector<bool> movingTabletArea = {false,false,false,false};

    vector<int> screenAreaPivot = {10, 10};
    vector<int> prevScreenAreaPivot = {10, 10};

    int tabletCamHeight = 120;
    int tabletCamWidth = tabletCamHeight;

    int tabletCamVolume = tabletCamHeight * tabletCamWidth;
    int minTabletCamSize = (int)((double)tabletCamWidth * 0.65);

    int prevTabletCamWidth = 20;

    int minScreenAreaSize = 30;

    bool movingScreenPivot = false;
    bool movingScreenWidth = false;

    bool movingTabletCamWidth = false;

    int screenAreaWidth = 70;
    int screenAreaHeight = 20;
    int prevScreenAreaWidth = 20;

    void checkMoveTabletSurface(int tabletCamWidth, int tabletCamHeight){

        int dx = mF::mouseX - mF::clickPos[0];
        int dy = mF::mouseY - mF::clickPos[1];

        for(int i = 0; i < tabletArea.size(); i++){
            if(movingTabletArea[i] && !mF::clicked) movingTabletArea[i] = false;

            if(!movingTabletArea[i]) continue;

            if(dx + tabletAreaPrev[i][0] < 0 || dx + tabletAreaPrev[i][0] >= tabletCamWidth) continue;
            if(dy + tabletAreaPrev[i][1] < 0 || dy + tabletAreaPrev[i][1] >= tabletCamHeight) continue;

            tabletArea[i] = {dx + tabletAreaPrev[i][0], dy + tabletAreaPrev[i][1]};
        }

    }

    void tabletSurfaceGUI(Mat& img, uint8_t*& pixelPtr, vector<int> camPos){

        vector<int> mouseCamera = {mF::mouseX - camPos[0], mF::mouseY - camPos[1]};

        if (gF::choosingColor) {

            circle(img, Point_(mouseCamera[0], mouseCamera[1]), 10, Scalar_(0, 255, 0), 2);
            if (mF::justClicked) {

                if (!(mouseCamera[0] < 0 || mouseCamera[0] >= img.cols || mouseCamera[1] < 0 ||
                      mouseCamera[1] >= img.rows)) {
                    int b = pixelPtr[mouseCamera[1] * img.cols * 3 + mouseCamera[0] * 3 + 0];
                    int g = pixelPtr[mouseCamera[1] * img.cols * 3 + mouseCamera[0] * 3 + 1];
                    int r = pixelPtr[mouseCamera[1] * img.cols * 3 + mouseCamera[0] * 3 + 2];

                    gF::baseColor = {b, g, r};
                    gF::choosingColor = false;
                }

            }
        }

        Point tabletP1 = Point_(tabletArea[0][0], tabletArea[0][1]);
        Point tabletP2 = Point_(tabletArea[1][0], tabletArea[1][1]);
        Point tabletP3 = Point_(tabletArea[2][0], tabletArea[2][1]);
        Point tabletP4 = Point_(tabletArea[3][0], tabletArea[3][1]);

        vector<Point> tabletPoints = {tabletP1, tabletP2, tabletP3, tabletP4};

        polylines(img, tabletPoints, true, gC::LINE, 2, LINE_AA);

        vector<int> tabletAreaCenter = {0, 0};

        for (int i = 0; i < tabletArea.size(); i++) {
            tabletAreaCenter[0] += tabletArea[i][0];
            tabletAreaCenter[1] += tabletArea[i][1];
        }

        tabletAreaCenter[0] /= 4;
        tabletAreaCenter[1] /= 4;

        int tabletButtonRadius = 9;

        for (int i = 0; i < 4; i++) {

            vector<int> point = tabletArea[i];

            int x = point[0];
            int y = point[1];

            if (movingTabletArea[i]) {
                gF::drawButton(img, x - tabletButtonRadius, y - tabletButtonRadius, tabletButtonRadius,
                               gC::BUTTON_FILLED,
                               gC::BUTTON_FILLED,
                               gC::BUTTON,
                               "", mouseCamera[0], mouseCamera[1]);
            } else {

                bool hover = gF::drawButton(img, x - tabletButtonRadius, y - tabletButtonRadius, tabletButtonRadius,
                                            gC::BUTTON_UNFILLED,
                                            gC::BUTTON_HOVER,
                                            gC::BUTTON,
                                             "", mouseCamera[0], mouseCamera[1]);

                if (hover && mF::justClicked) {
                    movingTabletArea[i] = true;
                    tabletAreaPrev[i] = tabletArea[i];
                    mF::justClicked = false;
                }

            }

        }

        int tabletTextX = tabletAreaCenter[0] - 44;
        int tabletTextY = tabletAreaCenter[1] + 4;

        gF::drawTextOutline(img, //target image
                "Tablet Area", //text
                tabletTextX, tabletTextY, //top-left position
                255,255,255);


    }

    void checkMoveFullScreenSurface(){

        if(movingTabletCamWidth && !mF::clicked) movingTabletCamWidth = false;

        if(movingTabletCamWidth){


            int dx = mF::mouseX - mF::clickPos[0];
            int newWidth = prevTabletCamWidth + dx;

            if(newWidth < minTabletCamSize) newWidth = minTabletCamSize;

            int newHeight = tabletCamVolume / newWidth;

            if(newHeight < minTabletCamSize){
                newHeight = minTabletCamSize;
                newWidth = tabletCamVolume / newHeight;
            }

            if(screenAreaWidth + screenAreaPivot[0] > newWidth){
                newWidth = screenAreaWidth + screenAreaPivot[0];
                newHeight = tabletCamVolume / newWidth;
            }


            if(screenAreaHeight + screenAreaPivot[1] > newHeight){
                newHeight = screenAreaHeight + screenAreaPivot[1];
                newWidth = tabletCamVolume / newHeight;
            }

            tabletCamWidth = newWidth;
            tabletCamHeight = (float) tabletCamVolume / tabletCamWidth;

        }

    }

    void checkMoveScreenSurface(){

        tabletCamHeight = (int) ((float) tabletCamVolume / tabletCamWidth);
        screenAreaHeight = (int) (screenAreaWidth * ((float) sR::screenHeight / sR::screenWidth));

        if(movingScreenPivot && !mF::clicked) movingScreenPivot = false;
        if(movingScreenWidth && !mF::clicked) movingScreenWidth = false;

        int dx = mF::mouseX - mF::clickPos[0];
        int dy = mF::mouseY - mF::clickPos[1];

        double ratio = (double)sR::screenHeight / sR::screenWidth;

        if(movingScreenWidth){

            int newScreenWidth = dx + prevScreenAreaWidth;
            int newScreenHeight = newScreenWidth * ratio;
            
            if(newScreenWidth + screenAreaPivot[0] > tabletCamWidth){
                newScreenWidth = tabletCamWidth - screenAreaPivot[0];
                newScreenHeight = newScreenWidth * ratio;
            }
            
            if(newScreenHeight + screenAreaPivot[1] > tabletCamHeight){
                newScreenHeight = tabletCamHeight - screenAreaPivot[1];
                newScreenWidth = newScreenHeight / ratio;
            }

            if(newScreenWidth < minScreenAreaSize){
                newScreenWidth = minScreenAreaSize;
                newScreenHeight = newScreenWidth * ratio;
            }

            if(newScreenHeight < minScreenAreaSize){
                newScreenHeight = minScreenAreaSize;
                newScreenWidth = newScreenHeight / ratio;
            }

            screenAreaWidth = newScreenWidth;
            screenAreaHeight = newScreenHeight;

        }

        if(movingScreenPivot){

            screenAreaPivot[0] = dx + prevScreenAreaPivot[0];
            screenAreaPivot[1] = dy + prevScreenAreaPivot[1];

            if(screenAreaPivot[0] < 0) screenAreaPivot[0] = 0;
            if(screenAreaPivot[0] + screenAreaWidth > tabletCamWidth) screenAreaPivot[0] = tabletCamWidth - screenAreaWidth;

            if(screenAreaPivot[1] < 0) screenAreaPivot[1] = 0;
            if(screenAreaPivot[1] + screenAreaHeight > tabletCamHeight) screenAreaPivot[1] = tabletCamHeight - screenAreaHeight;

        }

    }

    void screenSurfaceGUI(Mat& out, vector<int> tabletCamPos){

        vector<vector<int>> tabletCamPoints = {screenAreaPivot, {screenAreaPivot[0] + screenAreaWidth, screenAreaPivot[1] + screenAreaHeight}};

        int tRadius = 5;

        vector<int> mouseTablet = {mF::mouseX - tabletCamPos[0], mF::mouseY - tabletCamPos[1]};

        vector<bool> movingTablet = {movingScreenPivot, movingScreenWidth};

        rectangle(out,
                  Point_(screenAreaPivot[0], screenAreaPivot[1]),
                  Point_(screenAreaPivot[0] + screenAreaWidth, screenAreaPivot[1] + screenAreaHeight),
                  gC::LINE, 2);

        for (int i = 0; i < 2; i++) {

            vector<int> point = tabletCamPoints[i];

            int x = point[0];
            int y = point[1];

            if (movingTablet[i]) {
                gF::drawButton(out, x - tRadius, y - tRadius, tRadius,
                               gC::BUTTON_FILLED,
                               gC::BUTTON_FILLED,
                               gC::BUTTON, "", mouseTablet[0], mouseTablet[1]);
            } else {
                bool hover = gF::drawButton(out, x - tRadius, y - tRadius, tRadius,
                                            gC::BUTTON_UNFILLED,
                                            gC::BUTTON_HOVER,
                                            gC::BUTTON, "", mouseTablet[0], mouseTablet[1]);

                if (hover && mF::justClicked) {

                    if (i == 0) {
                        movingScreenPivot = true;
                        prevScreenAreaPivot = {screenAreaPivot[0], screenAreaPivot[1]};
                    }

                    if (i == 1) {
                        movingScreenWidth = true;
                        prevScreenAreaWidth = screenAreaWidth;
                    }

                    mF::justClicked = false;

                }
            }

        }

        int screenTextX = screenAreaPivot[0] + screenAreaWidth / 2 - 27;
        int screenTextY = screenAreaPivot[1] + screenAreaHeight / 2 + 5;

        screenAreaHeight = (int) (screenAreaWidth * (double) sR::screenHeight / sR::screenWidth);

        gF::drawTextOutline(out, //target image
                "Screen", //text
                screenTextX, screenTextY, //top-left position
                255,255,255);


    }

    void tabletCameraWidthGUI(Mat& canvas, vector<int> tabletCamPos){
        int buttonRadius = 9;

        int tabletCamCornerX = tabletCamPos[0] + tabletCamWidth;
        int tabletCamCornerY = tabletCamPos[1] + tabletCamHeight;

        if (movingTabletCamWidth) {
            gF::drawButton(canvas, tabletCamCornerX - buttonRadius, tabletCamCornerY - buttonRadius, buttonRadius,
                           gC::BUTTON_FILLED,
                           gC::BUTTON_FILLED,
                           gC::BUTTON, "", mF::mouseX, mF::mouseY);
        } else {
            bool hover = gF::drawButton(canvas, tabletCamCornerX - buttonRadius, tabletCamCornerY - buttonRadius, buttonRadius,
                                        gC::BUTTON_UNFILLED,
                                        gC::BUTTON_HOVER,
                                        gC::BUTTON, "", mF::mouseX, mF::mouseY);

            if (hover && mF::justClicked) {
                prevTabletCamWidth = tabletCamWidth;
                movingTabletCamWidth = true;
                mF::justClicked = false;
            }
        }
    }

} // dG