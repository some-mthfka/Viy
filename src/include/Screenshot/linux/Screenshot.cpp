#include "../Screenshot.h"

#include <X11/Xlib.h>
#include <X11/X.h>
#include <X11/Xutil.h>

bool takeScreenshot(sf::Image &im)
{
    //need to include error checking code
    
    Display *display = XOpenDisplay(NULL);
    Window   root    = DefaultRootWindow(display);

    XWindowAttributes gwa;

    XGetWindowAttributes(display, root, &gwa);

    int width    = gwa.width;
    int height   = gwa.height;

    XImage *image = XGetImage(display, root, 0, 0, width, height, AllPlanes, ZPixmap);

    unsigned char *array = new unsigned char[width * height * 4];

    unsigned long red_mask = image->red_mask;
    unsigned long green_mask = image->green_mask;
    unsigned long blue_mask = image->blue_mask;

    for (int x = 0; x < width; x++)
        for (int y = 0; y < height ; y++)
        {
            unsigned long pixel = XGetPixel(image,x,y);

            unsigned char blue = pixel & blue_mask;
            unsigned char green = (pixel & green_mask) >> 8;
            unsigned char red = (pixel & red_mask) >> 16;

            array[(x + width * y) * 4 + 0] = red;
            array[(x + width * y) * 4 + 1] = green;
            array[(x + width * y) * 4 + 2] = blue;
            array[(x + width * y) * 4 + 3] = ~0; //negate 0 to get 255
        }

    im.create(width, height, (sf::Uint8 *)array);

    return true;
}
