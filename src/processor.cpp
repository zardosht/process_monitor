#include "processor.h"
#include "linux_parser.h"

using std::vector;

// Return the aggregate CPU utilization
float Processor::Utilization() { 
    // Idle = idle + iowait
    // NonIdle = user + nice + system + irq + softirq + steal
    // Total = Idle + NonIdle
    // CPU_Percentage = (totald - idled)/totald
    // from here: https://stackoverflow.com/a/23376195/228965

    vector<long> new_values = LinuxParser::CpuUtilization();
    vector<long> deltas;
    for (int i = 0; i < 10; i++) {
        deltas.push_back(new_values[i] - values[i]);
    }
    // 0.user  1.nice  2.system  3.idle  4.iowait  5.irq  6.softirq  7.steal  8.guest  9.guest_nic
    long idle = deltas[3] + deltas[4];
    long nonidle = deltas[0] + deltas[1] + deltas[2] + deltas[5] + deltas[6] + deltas[7];
    long total = idle + nonidle;
    this->utilization = (total - idle) / (double)total;
    this->values = new_values;
    return this->utilization;

}