//
// Created by Ritik on 1/1/2024.
//

#ifndef HELLOWORLD_DRAGGABLEGUI_H
#define HELLOWORLD_DRAGGABLEGUI_H

namespace dG {

    extern vector<vector<int>> tabletArea;
    extern vector<vector<int>> tabletAreaPrev;
    extern vector<bool> movingTabletArea;

    extern vector<int> screenAreaPivot;
    extern vector<int> prevScreenAreaPivot;

    extern int tabletCamHeight;
    extern int tabletCamWidth;

    extern int tabletCamVolume;
    extern int minTabletCamSize;

    extern int prevTabletCamWidth;

    extern int minTabletSize;

    extern bool movingScreenPivot;
    extern bool movingScreenWidth;

    extern bool movingTabletCamWidth;

    extern int screenAreaWidth;
    extern int screenAreaHeight;
    extern int prevScreenAreaWidth;

    void checkMoveTabletSurface(int width, int height);

    void tabletSurfaceGUI(Mat& img, uint8_t*& pixelPtr, vector<int> camPos);

    void checkMoveFullScreenSurface();

    void checkMoveScreenSurface();

    void screenSurfaceGUI(Mat& out, vector<int> tabletCamPos);

    void tabletCameraWidthGUI(Mat& canvas, vector<int> tabletCamPos);

} // dG

#endif //HELLOWORLD_DRAGGABLEGUI_H
