#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <unistd.h>

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
    this->uptime = sys_uptime - (values[4] / clock); 

    long total_time = values[0] + values[1] + values[2] + values[3];
    this->cpu_usage = (total_time / clock) / (float)this->uptime;

    this->cpu_times = values;
    return this->cpu_usage;


// First we determine the total time spent for the process:

// total_time = utime + stime
// We also have to decide whether we want to include the time from children processes. If we do, then we add those values to total_time:

// total_time = total_time + cutime + cstime
// Next we get the total elapsed time in seconds since the process started:

// seconds = uptime - (starttime / Hertz)
// Finally we calculate the CPU usage percentage:

// cpu_usage = 100 * ((total_time / Hertz) / seconds)



    // long total_time = deltas[0] + deltas[1] + deltas[2] + deltas[3];  // utime + stime + cutime + cstime;
    // long clock_ticks_per_second = sysconf(_SC_CLK_TCK);
    // this->cpu_usage = total_time / (float)clock_ticks_per_second;
}

// TODO: Return the command that generated this process
string Process::Command() { return string(); }

// TODO: Return this process's memory utilization
string Process::Ram() { return string(); }

// TODO: Return the user (name) that generated this process
string Process::User() { return string(); }

// Return the age of this process (in seconds)
long int Process::UpTime() { 
    return this->uptime; 
}

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const { 
    return this->cpu_usage < a.cpu_usage; 
}

bool Process::comp(Process& p1, Process& p2) {
    // return p1 < p2;
    return p2 < p1;
}