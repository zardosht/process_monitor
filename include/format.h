#ifndef FORMAT_H
#define FORMAT_H

#include <string>

namespace Format {

    const int SECONDS_PER_HOUR = 3600;
    const int SECONDS_PER_MINUTE = 60;

    std::string ElapsedTime(long times);  
};                                  

#endif //FORMAT_H