//
// Created by Ritik on 1/1/2024.
//

#include <opencv2/opencv.hpp>
#include <opencv2/core/mat.hpp>
#include <Windows.h>

#include "mouseFuncs.h"

using namespace cv;
using namespace std;

namespace mF {

    int mouseX = -1;
    int mouseY = -1;

    bool clicked = false;
    bool justClicked = false;

    vector<int> clickPos = {-1,-1};

    int abortHotKey = 0;

    void onMouse(int event, int x, int y, int flags, void* userdata){

        if  ( event == EVENT_LBUTTONDOWN || event == EVENT_LBUTTONDBLCLK )
        {
            clicked = true;
            justClicked = true;
            clickPos = {x,y};
        }

        if( event == EVENT_LBUTTONUP){
            clicked = false;
        }

        mouseX = x;
        mouseY = y;
    }

    void checkHotKey(){
        if(GetAsyncKeyState(VK_RSHIFT))
        {
            abortHotKey++;
        }
        else{
            abortHotKey = 0;
        }
    }


    void setCursorPosBypass(int x, int y){

        POINT pos;
        GetCursorPos(&pos);

        INPUT input;
        ZeroMemory(&input,sizeof(input));

        input.type  = INPUT_MOUSE;
        input.mi.mouseData  = 0;
        input.mi.dx = (x - pos.x);//65536 - ((mx - prevX) *(65536/GetSystemMetrics(SM_CXSCREEN)));//x being coord in pixels
        input.mi.dy = (y - pos.y);//(my - prevY) *(65536/GetSystemMetrics(SM_CYSCREEN));//y being coord in pixels
        input.mi.dwFlags    =  MOUSEEVENTF_MOVE;//MOUSEEVENTF_ABSOLUTE
        SendInput(1,&input,sizeof(input));

    }

} // mF