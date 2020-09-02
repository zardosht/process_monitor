#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;


System::System() {
    this->kernel = LinuxParser::Kernel();
    this->operating_system = LinuxParser::OperatingSystem();
}


// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { return processes_; }

// Return the system's kernel identifier (string)
std::string System::Kernel() { 
    return this->kernel;
}

// Return the system's memory utilization ratio of used memory to total memory.
float System::MemoryUtilization() { 
    this->mem_utilization = LinuxParser::MemoryUtilization();
    return this->mem_utilization;    
}

// Return the operating system name
std::string System::OperatingSystem() { 
    return this->operating_system;
}

// Return the number of processes actively running on the system
int System::RunningProcesses() {     
    this->running_processes = LinuxParser::RunningProcesses();
    return this->running_processes;   
}

// Return the total number of processes on the system
int System::TotalProcesses() { 
    this->total_processes = LinuxParser::TotalProcesses();
    return this->total_processes;   
}

// Return the number of seconds since the system started running
long int System::UpTime() { 
    this->uptime = LinuxParser::UpTime();
    return this->uptime;    
}