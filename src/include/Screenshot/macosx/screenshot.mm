#import <AppKit/AppKit.h>

#include <SFML/Graphics.hpp>

bool takeScreenshot(sf::Image &im)
{
    /* Make a snapshot image of the current display. */
    CGImageRef  imageRef    = CGDisplayCreateImage(CGMainDisplayID());
    CFDataRef   rawData     = CGDataProviderCopyData(CGImageGetDataProvider(imageRef));
    
    UInt8 * buf = (UInt8 *) CFDataGetBytePtr(rawData); 
    int length = CFDataGetLength(rawData);
    //if imageRef == true check needed
   

    //I wonder if it is possible to get rid of this extra buffer below.
    //I tried to modify buf, but got bus error 10.
    //Probably I am accessing a read-only part of memory.

    UInt8 * conversionBuffer = new UInt8[length];

    for(long long int i = 0; i < length; i += 4)
    {
        //need endianess surrort (maybe)
        //need to swap colors because of different pixel formats
        conversionBuffer[i + 2] = buf[i+0]; //r
        conversionBuffer[i + 1] = buf[i+1]; //g
        conversionBuffer[i + 0] = buf[i+2]; //b
        conversionBuffer[i + 3] = buf[i+3]; //alpha, likes to be remembered about
    }

    //create expects unsigned ints for dimensions
    //CGImage gets return size_t, which is also unsigned
    //So, I think it is legit to expect correct conversion
    im.create(CGImageGetWidth(imageRef), CGImageGetHeight(imageRef), conversionBuffer);

    CFRelease(rawData);
     
    if (imageRef) 
        CFRelease(imageRef);

    delete [] conversionBuffer; 

    return true; //success
}
