#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <algorithm>

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


// Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// Return a container composed of the system's processes
vector<Process>& System::Processes() { 
    vector<int> pids = LinuxParser::Pids();
    for (int pid : pids) {
        bool new_proc = true;
        for (auto proc : processes_) {
            if (proc.Pid() == pid) {
                new_proc = false;
            }
        }
        if (new_proc) {
            Process process(pid);
            process.UpdateCpuUtilization();
            processes_.push_back(process);   
        }
        
    }    

    for(auto iter = processes_.begin(); iter != processes_.end(); iter++) {
        bool remove_proc = true;
        for (int pid : pids) {
            if((*iter).Pid() == pid) {
                remove_proc = false;
            }
        }
        if (remove_proc) {
            processes_.erase(iter--);
        }
    }

    for (auto proc : processes_) {
        proc.UpdateCpuUtilization();
    }

    std::sort(processes_.begin(), processes_.end(), Process::comp);
    return processes_; 
}

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