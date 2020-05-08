#ifndef PROCESS_H
#define PROCESS_H
#include "linux_parser.h"

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
   public:
    Process(int pid);
    int Pid();                               // TODO: See src/process.cpp
    std::string User();                      // TODO: See src/process.cpp
    std::string Command();                   // TODO: See src/process.cpp
    float CpuUtilization() const;            // TODO: See src/process.cpp
    std::string Ram();                       // TODO: See src/process.cpp
    long int UpTime();                       // TODO: See src/process.cpp
    bool operator<(Process const& a) const;  // TODO: See src/process.cpp

    // TODO: Declare any necessary private members
   private:
    int pid_;
    float utime_;      // CPU time spend in user code
    float stime_;      // CPU time spent in Kernel code
    float cutime_;     // Waited-for-children's CPU time speind in user code
    float cstime_;     // Waited-for-children's CPU time spend in Kernel code
    float startTime_;  // Time when the processes started
    float totalTime_;  // utime_ + stime_ + cutime_ + cstime_
    std::string command_;
    std::string user_;
    void SetTimeParameters();
    void SetCommand();
    void SetUser();
};

#endif