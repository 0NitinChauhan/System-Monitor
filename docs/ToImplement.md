# Functions To Implement

### system.cpp

- `Processor& System::Cpu() { return cpu_; }` // system's CPU

- `vector<Process>& System::Processes() { return processes_; }` // system's all processes

- `std::string System::Kernel() { return string(); }` system's kernel identifier (string)

- `float System::MemoryUtilization() { return 0.0; }` system's memory utilization

- `std::string System::OperatingSystem() { return string(); }` the operating system name

- `int System::RunningProcesses() { return 0; }` // number of processes actively running on the system

- `int System::TotalProcesses() { return 0; }` // total number of processes on the system

- `long int System::UpTime() { return 0; }` // the number of seconds since the system started running

### process.cpp

- `int Process::Pid() { return 0; }` // returns process's ID

- `float Process::CpuUtilization() { return 0; }` // this process's CPU utilization

- `string Process::Command() { return string(); }` // the command that generated this process

- `string Process::Ram() { return string(); }` // this process's memory utilization

- `string Process::User() { return string(); }` // the user (name) that generated this process

- `long int Process::UpTime() { return 0; }` // the age of this process (in seconds)

- `bool Process::operator<(Process const& a[[maybe_unused]]) const { return true; }` Overload '<'
