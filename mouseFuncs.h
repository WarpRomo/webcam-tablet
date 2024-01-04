//
// Created by Ritik on 1/1/2024.
//

#ifndef HELLOWORLD_MOUSEFUNCS_H
#define HELLOWORLD_MOUSEFUNCS_H

using namespace cv;
using namespace std;

namespace mF {

    extern int mouseX;
    extern int mouseY;

    extern bool clicked;
    extern bool justClicked;

    extern vector<int> clickPos;

    extern int abortHotKey;

    void onMouse(int event, int x, int y, int flags, void* userdata);
    void checkHotKey();
    void setCursorPosBypass(int x, int y);
} // mF

#endif //HELLOWORLD_MOUSEFUNCS_H
