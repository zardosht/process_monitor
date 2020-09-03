#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
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
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
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
      string filename(file->d_name);
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
  // Total used memory = MemTotal - MemFree
  double total_mem {1.0};
  double free_mem {0.0};
  string total_mem_key = "MemTotal";
  string free_mem_key = "MemFree";  

  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  string line;
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      if (line.rfind(total_mem_key, 0) == 0) {
        std::istringstream linestream(line);
        linestream >> total_mem_key >> total_mem;
      }
      if (line.rfind(free_mem_key, 0) == 0) {
        std::istringstream linestream(line);
        linestream >> free_mem_key >> free_mem;
      }
    }
  }
  
  double used_mem = total_mem - free_mem;
  return used_mem / total_mem; 
}

// Read and return the system uptime (in seconds)
long LinuxParser::UpTime() { 
  long uptime {0};
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if(stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
  }
  return uptime; 
}

// Read and return CPU utilization
vector<long> LinuxParser::CpuUtilization() { 
  long value;
  vector<long> values {};
  std::ifstream stream(kProcDirectory + kStatFilename);
  string cpu;
  if (stream.is_open()) {
    string line; 
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> cpu;
    for (int i = 0; i < 10; i++) {
      linestream >> value;
      values.push_back(value);
    }
  }
  return values;
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  int num_processes {0};
  std::string key {"processes"};
  std::ifstream stream(kProcDirectory + kStatFilename);
  std::string line; 
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      if (line.rfind(key, 0) == 0) {
        std::istringstream linestream(line);
        linestream >> key >> num_processes;
      }
    }
  }
  return num_processes; 
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  int num_processes {0};
  std::string key {"procs_running"};
  std::ifstream stream(kProcDirectory + kStatFilename);
  std::string line; 
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      if (line.rfind(key, 0) == 0) {
        std::istringstream linestream(line);
        linestream >> key >> num_processes;
      }
    }
  }
  return num_processes;   
}


std::vector<long> LinuxParser::CpuUtilization(int pid) {
  // /proc/[PID]/stat
  // #14 utime - CPU time spent in user code, measured in clock ticks
  // #15 stime - CPU time spent in kernel code, measured in clock ticks
  // #16 cutime - Waited-for children's CPU time spent in user code (in clock ticks)
  // #17 cstime - Waited-for children's CPU time spent in kernel code (in clock ticks)
  // #22 starttime - Time when the process started, measured in clock ticks

  vector<long> values;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if(stream.is_open()) {
    string line;
    getline(stream, line);
    std::istringstream linestream(line);
    long value; 
    string dummy;
    linestream >> value >> dummy >> dummy;
    for (int i = 3; i < 23; i++) {
      linestream >> value;
      if(i == 14 || i == 15 || i == 16 || i == 17 || i == 22) {
        values.push_back(value);
      }
    }
  }
  while(values.size() < 5) {
    values.push_back(0);
  }
  return values;
}


// Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  string cmd {NO_CMD};
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if(stream.is_open()) {
    string line;
    getline(stream, line);
    if (line.size() == 0) {
      return cmd;
    }
    std::istringstream linestream(line);
    linestream >> cmd;
  }
  return cmd; 
}

// Read and return the memory used by a process
float LinuxParser::Ram(int pid) { 
  float value {0};
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    string line;
    string key {"VmSize"};
    while(getline(stream, line)) {
      if (line.rfind(key, 0) == 0) {
        std::istringstream linestream(line);
        linestream >> key >> value;
      }
    }
  }
  return value / 1024; 
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid[[maybe_unused]]) { 
  string uid {""};
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    string line;
    string key {"Uid"};
    while(getline(stream, line)) {
      if (line.rfind(key, 0) == 0) {
        std::istringstream linestream(line);
        linestream >> key >> uid;
        break;
      }
    }
  }
  return uid;
}

// Read and return the user associated with a process
string LinuxParser::User(string uid) { 
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    string line;
    while(getline(stream, line)) {
        std::istringstream linestream(line);
        vector<string> tokens;
        string token;
        while(getline(linestream, token, ':')) {
          tokens.push_back(token);
          if (tokens.size() > 2) {
            if (tokens[2].compare(uid) == 0) {
              return tokens[0];
            } else {
              break;
            }
          }
        }
    }
  }
  return "";

}

