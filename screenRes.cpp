//
// Created by Ritik on 1/1/2024.
//

#include "screenRes.h"
#include <Windows.h>

namespace sR {

    int screenWidth;
    int screenHeight;

    void getScreenResolution() {
        screenWidth  = (int) GetSystemMetrics(SM_CXSCREEN);
        screenHeight = (int) GetSystemMetrics(SM_CYSCREEN);
    }


} // sR