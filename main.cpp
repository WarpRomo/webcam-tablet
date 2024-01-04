#pragma ide diagnostic ignored "EndlessLoop"
#include <opencv2/opencv.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <iostream>
#include <Windows.h>
#include <ctime>

#include "GUI/guiFuncs.h"
#include "GUI/draggableGUI.h"
#include "GUI/guiColors.h"

#include "tabletCalc.h"
#include "mouseFuncs.h"
#include "screenRes.h"

using namespace cv;
using namespace std;

vector<int> camPos = {20, 20};
vector<int> tabletCamPos = {-1,-1};

string windowName = "Webcam Mouse";

int frameCount = 0;

int lastFps = 0;
float avgFps = 0;
auto prevTime = clock();

void fpsCounter(Mat& canvas){
    auto currTime = clock();
    auto delta = currTime - prevTime;
    prevTime = currTime;

    avgFps += (float)(CLOCKS_PER_SEC / delta);

    gF::drawTextOutline(canvas, to_string(lastFps) + " FPS", camPos[0]+1, camPos[1] + 14, 0, 255, 0);

    if(frameCount % 10 == 0){
        lastFps = (int) avgFps/10;
        avgFps = 0;
    }

}

int lastValProcess = 0;
float avgProcess = 0;
auto prevProcess = clock();

void startProcess(){
    prevProcess = clock();
}

void drawProcessTime(Mat& canvas){
    auto currTime = clock();
    auto delta = currTime - prevProcess;

    avgProcess += ((float)1000*delta / CLOCKS_PER_SEC);
    gF::drawTextOutline(canvas, to_string(lastValProcess) + "ms process time", camPos[0]+1, camPos[1] + 33, 0, 255, 0);

    if(frameCount % 10 == 0){
        lastValProcess = (int) avgProcess / 10;
        avgProcess = 0;
    }

}

int main() {

    VideoCapture cam(0, CAP_DSHOW);

    cam.set(CAP_PROP_FPS, 60);
    cam.set(CAP_PROP_FRAME_WIDTH, 640);
    cam.set(CAP_PROP_FRAME_HEIGHT, 480);

    int camWidth = cam.get(CAP_PROP_FRAME_WIDTH);
    int camHeight = cam.get(CAP_PROP_FRAME_HEIGHT);

    tabletCamPos = {camPos[0] + camWidth + 20, camPos[1]};

    sR::getScreenResolution();
    namedWindow(windowName);
    setMouseCallback(windowName, mF::onMouse, NULL);

    HWND hwnd = (HWND) cvGetWindowHandle(windowName.c_str());

    Mat canvas(520, 960, CV_8UC3);
    Mat webcamImg;

    gF::initSlider();

    while (IsWindowVisible(hwnd)) {

        canvas.setTo(gC::BACKGROUND);
        uint8_t *canvasPtr = (uint8_t *) canvas.data;

        cam >> webcamImg;
        startProcess();
        resize(webcamImg, webcamImg, Size(640, 480), 0, 0, INTER_CUBIC);
        uint8_t *pixelPtr = (uint8_t *) webcamImg.data;

        dG::checkMoveTabletSurface(camWidth, camHeight);


        Mat out = tC::warpedMat(dG::tabletArea[0], dG::tabletArea[1], dG::tabletArea[2], dG::tabletArea[3], webcamImg,
                                dG::tabletCamWidth,
                                dG::tabletCamHeight,
                                gF::tabletRotIndex,
                                gF::mirrorButton[2]);

        uint8_t *tabletPtr = (uint8_t *) out.data;


        tC::moveMouse(out, tabletPtr,
                      gF::baseColor,
                      gF::baseDistance,
                      gF::tipExtra,
                      gF::leftHandedButton[2],
                      gF::mouseEnabled[2],
                      dG::screenAreaWidth, dG::screenAreaHeight, dG::screenAreaPivot);


        dG::checkMoveFullScreenSurface();
        dG::checkMoveScreenSurface();

        dG::screenSurfaceGUI(out, tabletCamPos);
        dG::tabletCameraWidthGUI(canvas, tabletCamPos);

        int windowBorder = 4;

        rectangle(canvas,
                  Point_(tabletCamPos[0] - windowBorder, tabletCamPos[1] - windowBorder),
                  Point_(tabletCamPos[0] + out.cols + windowBorder, tabletCamPos[1] + out.rows + windowBorder),
                  gC::WINDOW_OUTLINE, -1);


        out.copyTo(canvas(Rect(tabletCamPos[0], tabletCamPos[1], out.cols, out.rows)));


        dG::tabletSurfaceGUI(webcamImg, pixelPtr, camPos);


        rectangle(canvas,
                  Point_(camPos[0] - windowBorder, camPos[1] - windowBorder),
                  Point_(camPos[0] + webcamImg.cols + windowBorder, camPos[1] + webcamImg.rows + windowBorder),
                  gC::WINDOW_OUTLINE, -1);


        webcamImg.copyTo(canvas(Rect(camPos[0], camPos[1], webcamImg.cols, webcamImg.rows)));


        gF::setGUIPos(camPos, camWidth, dG::tabletCamHeight);
        gF::drawSliderGUI(canvas);
        gF::drawButtonGUI(canvas);

        fpsCounter(canvas);
        drawProcessTime(canvas);

        imshow(windowName, canvas);

        if (mF::justClicked) mF::justClicked = false;

        mF::checkHotKey();
        if (mF::abortHotKey==1) {
            gF::mouseEnabled[2] = !gF::mouseEnabled[2];
        }


        frameCount++;

        int pressed = waitKey(1) & 0xFF;

    }


    cam.release();

    return 0;

}