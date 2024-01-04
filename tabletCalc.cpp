//
// Created by Ritik on 1/1/2024.
//


#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <Windows.h>

#include "pixelFuncs.h"
#include "tabletCalc.h"
#include "screenRes.h"
#include "mouseFuncs.h"

using namespace std;
using namespace cv;

namespace tC {

    extern vector<int> tabletRot = {0, ROTATE_90_CLOCKWISE, ROTATE_180, ROTATE_90_COUNTERCLOCKWISE};
    extern vector<int> buttonColor = {0,255,0};
    extern vector<int> importantColor = {0, 0, 255};
    extern double importantPower = 1.0;

    Mat warpedMat(vector<int>& p1, vector<int>& p2, vector<int>& p3, vector<int>& p4, Mat& img, float width, float height, int rotation, bool mirror){

        if(rotation % 2 == 1){
            float temp = width;
            width = height;
            height = temp;
        }

        vector<Point2f> cameraPoints;
        cameraPoints.push_back({(float)p1[0], (float)p1[1]});
        cameraPoints.push_back({(float)p2[0], (float)p2[1]});
        cameraPoints.push_back({(float)p3[0], (float)p3[1]});
        cameraPoints.push_back({(float)p4[0], (float)p4[1]});

        vector<Point2f> worldPoints;
        worldPoints.push_back({0, 0});
        worldPoints.push_back({width, 0});
        worldPoints.push_back({width, height});
        worldPoints.push_back({0, height});

        Mat perspectiveMat_= findHomography(cameraPoints, worldPoints, RANSAC);
        perspectiveTransform(cameraPoints, worldPoints, perspectiveMat_);

        Mat out(width, height, CV_8UC3);
        warpPerspective(img, out, perspectiveMat_, Size(width, height));
        if(rotation > 0) rotate(out, out, tabletRot[rotation]);
        if(mirror) flip(out, out, 1);

        return out;
    }


    vector<float> getTabletPos(Mat& out, uint8_t*& tabletPtr, vector<int> baseColor, int baseDistance, int tipExtra, bool leftHanded){

        vector<float> tabletCoord = {0,0};

        int count = 0;


        for(int y = 0; y < out.rows; y++){

            int width = min(y, out.cols);

            int mid = width / 2;

            for(int i = 0; i < width; i++){

                int p = i / 2;

                if(i % 2 == 0) p = -p;

                int x = mid + p;

                int cX = out.cols - x;
                if(!leftHanded) cX = x;

                int nY = y - mid - p;

                int pixel = nY * out.cols * 3 + cX * 3;

                if(cX >= out.cols || cX < 0) cout << "1ripX" << endl;
                if(y >= out.rows || y < 0) cout << "1ripY" << endl;

                int distance = pF::rgbDistance(tabletPtr, baseColor, pixel);

                if(distance < baseDistance) continue;

                if(count < tipExtra){
                    tabletCoord[0] += cX;
                    tabletCoord[1] += nY;
                    count++;
                    pF::highlightPixel(tabletPtr, buttonColor, 1.0, pixel);
                }
                else{
                    pF::highlightPixel(tabletPtr, importantColor, 1.0, pixel);
                }

            }

        }

        for(int x = 0; x < out.cols; x++){

            int leftOver = out.cols - x;

            int height = min(leftOver, out.rows);

            int mid = height/2;


            for(int i = 0; i < height; i++){

                int p = i / 2;

                if(i % 2 == 0) p = -p;

                int y = out.rows - 1 - mid + p;
                int nX = x + mid - p;

                int cX = out.cols - nX;
                if(!leftHanded) cX = nX;

                int pixel = y * out.cols * 3 + cX * 3;

                if(cX >= out.cols || cX < 0) cout << "2ripX" << endl;
                if(y >= out.rows) cout << "2ripY " + to_string(y) + " " + to_string(out.rows) << endl;
                if(y < 0) cout << "2ripYn" << endl;

                int distance = pF::rgbDistance(tabletPtr, baseColor, pixel);

                if(distance < baseDistance) continue;

                if(count < tipExtra){
                    pF::highlightPixel(tabletPtr, buttonColor, 1.0, pixel);

                    tabletCoord[0] += cX;
                    tabletCoord[1] += y;
                    count++;
                }
                else{
                    pF::highlightPixel(tabletPtr, importantColor, 1.0, pixel);
                }

            }
        }

        if(count == 0) return {-999999, -999999};

        tabletCoord[0] /= count;
        tabletCoord[1] /= count;

        return tabletCoord;
    }

    void moveMouse(Mat& out, uint8_t*& tabletPtr, vector<int> baseColor, int baseDistance, int tipExtra, bool leftHanded, bool mouseEnabled, int screenAreaWidth, int screenAreaHeight, vector<int> screenAreaPivot){

        vector<float> tabletCoord = tC::getTabletPos(out, tabletPtr, baseColor, baseDistance, tipExtra, leftHanded);

        int tabletScreenX = sR::screenWidth * (tabletCoord[0] - screenAreaPivot[0]) / screenAreaWidth;
        int tabletScreenY = sR::screenHeight * (tabletCoord[1] - screenAreaPivot[1]) / screenAreaHeight;

        vector<int> tabletScreen = {tabletScreenX, tabletScreenY};

        if(mouseEnabled && tabletCoord[0] != -999999){

            if(tabletScreenX < 0) tabletScreenX = 0;
            if(tabletScreenX >= sR::screenWidth) tabletScreenX = sR::screenWidth-1;
            if(tabletScreenY < 0) tabletScreenY = 0;
            if(tabletScreenY >= sR::screenHeight) tabletScreenY = sR::screenHeight-1;

            mF::setCursorPosBypass(tabletScreenX,tabletScreenY);

        }

    }

} // tC