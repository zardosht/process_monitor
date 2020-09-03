#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <vector>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
   Process(int pid);

    std::string User();                      
    std::string Command();                   
    float CpuUtilization();                  
    float UpdateCpuUtilization();                  
    std::string Ram();                       
    int Pid();                               
    long int UpTime();                       
    bool operator<(Process const& a) const;  
    static bool comp(Process& p1, Process& p2);

 private:
    int pid {-1};
    long uptime {0};
    float cpu_usage {0};
    std::string cmd {""};
    float ram_usage {0};
    std::string uid {""};
    std::string user {""};
};

#endif