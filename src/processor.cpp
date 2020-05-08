#include <string>
#include "processor.h"
#include <sstream>

// Default constructor --> initialize values of private members to 0
Processor::Processor() {
    /**
    prevJiffyMap_["user"] = 1;
    prevJiffyMap_["nice"] = 1;
    prevJiffyMap_["system"] = 1;
    prevJiffyMap_["idle"] = 1;
    prevJiffyMap_["iowait"] = 1;
    prevJiffyMap_["irq"] = 1;

    prevJiffyMap_["softrq"] = 1;
    prevJiffyMap_["steal"] = 1;
    prevJiffyMap_["guest"] = 1;
    prevJiffyMap_["guest_nice"] = 1;**/
}

/**
 *
 Used idea from the following URL
 https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux/23376195#23376195

 TODO: better calculation of CPU Utilization

 **/

float Processor::Utilization() {
    float cpu, user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
    std::string line;
    std::ifstream lineStream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
    if (lineStream.is_open()) {
        std::getline(lineStream, line);
        std::istringstream linestream(line);
        linestream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
        float idletime = idle + iowait;
        float nonidletime = user + nice + system + irq + softirq + steal;
        float totaltime = idletime + nonidletime;

        return (totaltime - idletime) / totaltime;
    }
    return -1;
}

// TODO: Return the aggregate CPU utilization
/**
float Processor::Utilization2() {
    std::unordered_map<std::string, long> prevJiffyMap = prevJiffyMap_;
    std::unordered_map<std::string, long> currentJiffyMap;

    long prevIdle = prevJiffyMap["idle"] + prevJiffyMap["iowait"];
    long currentIdle = currentJiffyMap["idle"] + currentJiffyMap["iowait"];

    long prevNonIdle = prevJiffyMap["user"] + prevJiffyMap["nice"] + prevJiffyMap["system"] + prevJiffyMap["irq"] + prevJiffyMap["softirq"] + prevJiffyMap["steal"];

    long currentNonIdle = currentJiffyMap["user"] + currentJiffyMap["nice"] + currentJiffyMap["system"] + currentJiffyMap["irq"] + currentJiffyMap["softirq"] + currentJiffyMap["steal"];

    long prevTotal = prevIdle + prevNonIdle;
    long currentTotal = currentIdle + currentNonIdle;

    long totalTime = currentTotal - prevTotal;
    long idleTime = currentIdle - prevIdle;

    // update value of prevJiffyMap
    prevJiffyMap_ = currentJiffyMap;

    return (totalTime - idleTime) / totalTime;
}
**/
