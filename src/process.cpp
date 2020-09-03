#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <unistd.h>
#include <iomanip>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;



Process::Process(int pid) : pid(pid) {    }



// Return this process's ID
int Process::Pid() { 
    return pid; 
}

// Return this process's CPU utilization
float Process::CpuUtilization() { 
    return this->cpu_usage;
}

float Process::UpdateCpuUtilization() {
    // https://stackoverflow.com/a/16736599/228965
    // #0 utime - CPU time spent in user code, measured in clock ticks
    // #1 stime - CPU time spent in kernel code, measured in clock ticks
    // #2 cutime - Waited-for children's CPU time spent in user code (in clock ticks)
    // #3 cstime - Waited-for children's CPU time spent in kernel code (in clock ticks)
    // #4 starttime - Time when the process started, measured in clock ticks    
    
    vector<long> values = LinuxParser::CpuUtilization(pid);
    long sys_uptime = LinuxParser::UpTime();
    float clock = sysconf(_SC_CLK_TCK);
    if ((values[4] / clock) > sys_uptime) {
        this->uptime = sys_uptime;
    } else {
        this->uptime = sys_uptime - (values[4] / clock); 
    }

    long total_time = values[0] + values[1] + values[2] + values[3];
    this->cpu_usage = (total_time / clock) / (float)this->uptime;
    return this->cpu_usage;
}

// Return the command that generated this process
string Process::Command() { 
    if(this->cmd.compare("") == 0) {
        this->cmd = LinuxParser::Command(pid); 
    }
    return this->cmd;
}

// Return this process's memory utilization
string Process::Ram() { 
    this->ram_usage = LinuxParser::Ram(pid);
    std::ostringstream oss;
    oss << std::fixed;
    oss << std::setprecision(2);
    oss << this->ram_usage;
    return oss.str();
}

// Return the user (name) that generated this process
string Process::User() { 
    if(this->uid.compare("") == 0) {
        this->uid = LinuxParser::Uid(pid);
        this->user = LinuxParser::User(this->uid);
    }
    return this->user; 
}

// Return the age of this process (in seconds)
long int Process::UpTime() { 
    return this->uptime; 
}

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const { 
    // return this->pid < a.pid; 
    return this->cpu_usage < a.cpu_usage;
}

bool Process::comp(Process& p1, Process& p2) {
    // return p1 < p2;
    return p2 < p1;
}