#include <unistd.h>
#include "../include/utils.h"

double Utils::ConvertKBtoMB(double dataInKB) { return dataInKB / 1024; }

double Utils::ConvertClockTickToSeconds(double clockTicks) { return clockTicks / sysconf(_SC_CLK_TCK); };
