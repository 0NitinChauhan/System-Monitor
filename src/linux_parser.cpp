#include <dirent.h>
#include <string>
#include <vector>
#include <sstream>

#include "linux_parser.h"

using std::stof;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
std::string LinuxParser::OperatingSystem() {
    std::string line;
    std::string key;
    std::string value;
    std::ifstream filestream(kOSPath);
    if (filestream.is_open()) {
        while (std::getline(filestream, line)) {
            std::replace(line.begin(), line.end(), ' ', '_');
            std::replace(line.begin(), line.end(), '=', ' ');
            std::replace(line.begin(), line.end(), '"', ' ');
            std::istringstream linestream(line);
            while (linestream >> key >> value) {
                if (key == "PRETTY_NAME") {
                    std::replace(value.begin(), value.end(), '_', ' ');
                    return value;
                }
            }
        }
    }
    return value;
}

// DONE: An example of how to read data from the filesystem
std::string LinuxParser::Kernel() {
    std::string os, kernel;
    std::string line;
    std::ifstream stream(kProcDirectory + kVersionFilename);
    if (stream.is_open()) {
        std::getline(stream, line);
        std::istringstream linestream(line);
        linestream >> os >> kernel;
    }
    return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
    vector<int> pids;
    DIR* directory = opendir(kProcDirectory.c_str());
    struct dirent* file;
    while ((file = readdir(directory)) != nullptr) {
        // Is this a directory?
        if (file->d_type == DT_DIR) {
            // Is every character of the name a digit?
            std::string filename(file->d_name);
            if (std::all_of(filename.begin(), filename.end(), isdigit)) {
                int pid = stoi(filename);
                pids.push_back(pid);
            }
        }
    }
    closedir(directory);
    return pids;
}

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
    std::string filePath = kProcDirectory + kMeminfoFilename;
    std::unordered_map<std::string, long> expectedKeysMap;
    expectedKeysMap["MemTotal:"] = 0;
    expectedKeysMap["MemFree:"] = 0;
    expectedKeysMap = ParsingUtils::ParseMultipleLinesGetKeys(expectedKeysMap, filePath);
    return (expectedKeysMap["MemTotal:"] - expectedKeysMap["MemFree:"]) * 1.0 / (expectedKeysMap["MemTotal:"]);
}

//  Read and return the system uptime
long LinuxParser::UpTime() {
    std::string line = ParsingUtils::ReturnFileStreamLineOutput(kProcDirectory + kUptimeFilename);
    std::stringstream lineStream(line);
    double upTime;
    double idleTime;
    lineStream >> upTime >> idleTime;
    return (long)upTime;
}

/**
NOTE: the functionality of the following 4 functions is implemented using CPUJiffyMap and
Processor::Utilization

// Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// Read and return the number of active jiffies for a PID
// long LinuxParser::ActiveJiffies(int pid [[maybe_unused]]) { return 0; }

// Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }
**/

std::unordered_map<std::string, long> LinuxParser::CPUJiffyMap() {
    std::string filePath = kProcDirectory + kStatFilename;
    std::string line = ParsingUtils::MatchKeyAndReturnSingleLine("cpu", filePath);
    std::cout << "Line: " << line << std::endl;
    std::unordered_map<std::string, long> jiffyMap;
    vector<std::string> jiffyVector = ParsingUtils::split(line, ' ');

    std::cout << "now printing values: " << std::endl;
    for (auto str : jiffyVector) {
        std::cout << str << " ";
    }
    std::cout << std::endl;

    jiffyMap["user"] = stol(jiffyVector.at(1));
    jiffyMap["nice"] = stol(jiffyVector.at(2));
    jiffyMap["system"] = stol(jiffyVector.at(3));
    jiffyMap["idle"] = stol(jiffyVector.at(4));
    jiffyMap["iowait"] = stol(jiffyVector.at(5));
    jiffyMap["irq"] = stol(jiffyVector.at(6));

    jiffyMap["softrq"] = stol(jiffyVector.at(7));
    jiffyMap["steal"] = stol(jiffyVector.at(8));
    jiffyMap["guest"] = stol(jiffyVector.at(9));
    jiffyMap["guest_nice"] = stol(jiffyVector.at(10));
    return jiffyMap;
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
    std::string filePath = kProcDirectory + kStatFilename;
    std::unordered_map<std::string, int> expectedKeysMap;
    expectedKeysMap["processes"] = -1;
    return ParsingUtils::ParseMultipleLinesGetKeys(expectedKeysMap, filePath)["processes"];
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
    std::string expectedKey("procs_running");
    std::string filePath = kProcDirectory + kStatFilename;
    std::unordered_map<std::string, int> expectedKeysMap;
    expectedKeysMap["procs_running"];
    return ParsingUtils::ParseMultipleLinesGetKeys(expectedKeysMap, filePath)["procs_running"];
}

// Read and return the command associated with a process
std::string LinuxParser::Command(int pid = -1) {
    std::stringstream commandStream;
    commandStream << kProcDirectory.substr(0, kProcDirectory.length() - 1);
    if (pid > 0) {
        commandStream << "/" << pid;
    }
    commandStream << kCmdlineFilename;
    std::string filePath = commandStream.str();
    std::string line = "-1";
    return ParsingUtils::ReturnFileStreamLineOutput(filePath);
}

// Read and return the memory used by a process
// if pid == -1 ==> return memory used by the system
std::string LinuxParser::Ram(int pid = -1) {
    std::unordered_map<std::string, long> expectedKeysMap;
    std::string expectedKey;
    std::string key;
    std::string line;
    std::string value;
    std::stringstream commandStream;
    commandStream << kProcDirectory.substr(0, kProcDirectory.length() - 1);

    if (pid <= 0) {
        commandStream << kMeminfoFilename;
        expectedKey = "MemTotal:";
    } else {
        commandStream << "/" << pid << kStatusFilename;
        expectedKey = "VmSize:";
    }
    expectedKeysMap[expectedKey] = 0;
    long ram = ParsingUtils::ParseMultipleLinesGetKeys(expectedKeysMap, commandStream.str())[expectedKey];
    std::stringstream ramStream;
    ramStream << Utils::ConvertKBtoMB(ram);
    return ramStream.str();
}

// Read and return the user ID associated with a process
std::string LinuxParser::Uid(int pid) {
    std::string line;
    std::string key;
    std::string expectedKey = "Uid:";
    std::string value;

    std::stringstream commandStream;
    if (pid <= 0) {
        return "-1";
    }
    commandStream << kProcDirectory << pid << kStatusFilename;
    line = ParsingUtils::MatchKeyAndReturnSingleLine(expectedKey, commandStream.str());
    std::vector<std::string> lineTokens = ParsingUtils::split(line, ' ');
    if (lineTokens.size() > 2) {
        return lineTokens.at(1);
    }
    return line;
}

// Read and return the user associated with a process
std::string LinuxParser::User(int pid) {
    std::string uid = LinuxParser::Uid(pid);
    std::string line;
    std::string key;
    char delimiter = ':';

    std::ifstream fileStream(kPasswordPath);
    if (fileStream.is_open()) {
        while (std::getline(fileStream, line)) {
            vector<std::string> currentLineTokens = ParsingUtils::split(line, delimiter);
            if (currentLineTokens.size() >= 3 && currentLineTokens.at(2) == uid) {
                return currentLineTokens.at(0);
            }
        }
    }
    return "-1";
}

// Read and return the uptime of a process --> seconds
// NOTE: not used in the project --> consider a utility function for future usecase
long LinuxParser::UpTime(int pid) {
    std::stringstream commandStream;
    if (pid > 0) {
        commandStream << kProcDirectory << pid << kStatFilename;
    }
    std::string line = ParsingUtils::ReturnFileStreamLineOutput(commandStream.str());
    std::vector<std::string> lineTokens = ParsingUtils::split(line, ' ');
    return LinuxParser::UpTime() - Utils::ConvertClockTickToSeconds(stof(lineTokens.at(kProcessStartTimeIndex)));
}

// returns time parameters related as output by /proc/[pid]/state
std::vector<std::string> LinuxParser::GetProcessTimeParameters(int pid) {
    std::stringstream commandStream;
    commandStream << kProcDirectory << pid << kStatFilename;
    std::string line = ParsingUtils::ReturnFileStreamLineOutput(commandStream.str());
    return ParsingUtils::split(line, ' ');
}