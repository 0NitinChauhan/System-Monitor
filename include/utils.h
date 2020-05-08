#ifndef UTILS_H
#define UTILS_H

#include <iostream>

class Utils {
   public:
    static double ConvertKBtoMB(double sizeInKB);

    static double ConvertClockTickToSeconds(double clockTicks);
};

#endif