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

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

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

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid[[maybe_unused]]) { return string(); }

// Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) { 
  return 0; 
}