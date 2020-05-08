#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) : pid_(pid) {
    Process::SetTimeParameters();
    Process::SetCommand();
    Process::SetUser();
}

// Setting parameters in clock-ticks
// can be later converted to seconds if required
void Process::SetTimeParameters() {
    vector<string> timeParameters = LinuxParser::GetProcessTimeParameters(pid_);
    utime_ = stof(timeParameters.at(LinuxParser::kProcessUTimeIndex));
    stime_ = stof(timeParameters.at(LinuxParser::kProcessSTimeIndex));
    cutime_ = stof(timeParameters.at(LinuxParser::kProcessCUTimeIndex));
    cstime_ = stof(timeParameters.at(LinuxParser::kProcessSTimeIndex));

    totalTime_ = utime_ + stime_ + cutime_ + cstime_;
    startTime_ = stof(timeParameters.at(LinuxParser::kProcessStartTimeIndex));
};

void Process::SetCommand() { user_ = LinuxParser::Command(pid_); };

void Process::SetUser() { user_ = LinuxParser::Command(pid_); };

// Return this process's ID
// pid set at the time of creating process object
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() const {
    double usageInSeconds = LinuxParser::UpTime() - Utils::ConvertClockTickToSeconds(startTime_);
    return Utils::ConvertClockTickToSeconds(totalTime_) / usageInSeconds;
}

// Return the command that generated this process
// command set at the time of creating process object
string Process::Command() { return command_; }

// Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// Return the user (name) that generated this process
// user set at the time of creating process object
string Process::User() { return user_; }

// Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime() - (Utils::ConvertClockTickToSeconds(startTime_)); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { return CpuUtilization() < a.CpuUtilization(); }