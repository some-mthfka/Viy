#include "../Screenshot.h"

#include <windows.h> //need to replace with only necessary headers

//http://stackoverflow.com/questions/3688409/getdibits-and-loop-through-pixels-using-x-y
//https://msdn.microsoft.com/en-us/library/windows/desktop/dd183376(v=vs.85).aspx
//Assumptions: 32 bits per pixel 
bool takeScreenshot(sf::Image &im) 
{
    HDC     hScreen     =   GetDC(GetDesktopWindow());    //GetSystemMetrics(SM_CX(Y)SCREEN)
    int     ScreenX     =   GetDeviceCaps(hScreen, HORZRES);
    int     ScreenY     =   GetDeviceCaps(hScreen, VERTRES);
    HDC     hdcMem      =   CreateCompatibleDC(hScreen);
    HBITMAP hBitmap     =   CreateCompatibleBitmap(hScreen, ScreenX, ScreenY);
    HGDIOBJ hOld        =   SelectObject(hdcMem, hBitmap);

    bool bitBltSuccess = BitBlt(hdcMem, 0, 0, ScreenX, ScreenY, hScreen, 0, 0, SRCCOPY);
    SelectObject(hdcMem, hOld);

    BITMAPINFOHEADER bmi = {0};
    bmi.biSize = sizeof(BITMAPINFOHEADER);
    bmi.biPlanes = 1;
    bmi.biBitCount = 32;
    bmi.biWidth = ScreenX;
    bmi.biHeight = -ScreenY;    //negative means that origin is upper left corner
    bmi.biCompression = BI_RGB; //Uncompressed
    bmi.biSizeImage = 0;        //Must be set to 0 for BI_RGB 

    BYTE* screenData = new BYTE[4 * ScreenX * ScreenY];  //DO NOT FORGET TO FREE

    GetDIBits(hdcMem, hBitmap, 0, ScreenY, screenData, (BITMAPINFO*)&bmi, DIB_RGB_COLORS);

    if (!hScreen || !hdcMem || !hBitmap || !hOld || !bitBltSuccess)
        return false;  ///Failure

    //A pixel in screenData is represented by a DWORD (4 bytes long)
    //Starting from the most significant bit: unused, red, green, blue
    //DWORD on intel, due to little-endianness, is reversed: blue, green, red, unused
    //SFML expects: red, green, blue, unused
    //solution: exchange blue and red
    //
    //Instead of doing the following magic, I can simply change sf::Image::create(),
    //but then I would have to recompile the whole SFML,
    //so I will stick with this solution for now.
    //
    //Can support resolution of up to 236311254 * 236311254 pixels. Probably.

    for (long long int i = 0; i < ScreenX * ScreenY * 4 - 2; i += 4)
      std::swap(screenData[i], screenData[i+2]); //Swap red and blue

    im.create(ScreenX, ScreenY, (sf::Uint8 *)screenData);

    delete [] screenData;

    ReleaseDC(GetDesktopWindow(),hScreen);
    DeleteDC(hdcMem);
    DeleteObject(hBitmap);

    return true;  ///Success
}
