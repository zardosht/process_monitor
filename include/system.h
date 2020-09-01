#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

class System {
 public:
    System();

    Processor& Cpu();                   
    std::vector<Process>& Processes();  
    float MemoryUtilization();          
    long UpTime();                      
    int TotalProcesses();               
    int RunningProcesses();             
    std::string Kernel();             
    std::string OperatingSystem();      

 private:
    Processor cpu_ = {};
    std::vector<Process> processes_ = {};
    std::string kernel {"-"};
    std::string operating_system {"-"};
    long uptime {0};
};

#endif

