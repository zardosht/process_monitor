#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>

class Processor {
 public:
    float Utilization();  

 private:
   float utilization {0.0};
   std::vector<long> values {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    
};

#endif //PROCESSOR_H