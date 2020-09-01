#include <string>
#include <sstream>
#include <iomanip>

#include "format.h"

using std::string;


// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) { 
    int hours = seconds / SECONDS_PER_HOUR; 
    int minutes = (seconds % SECONDS_PER_HOUR) / SECONDS_PER_MINUTE;
    int secs = seconds - (hours * SECONDS_PER_HOUR) - (minutes * SECONDS_PER_MINUTE);

    std::ostringstream ss;
    ss << std::setw(2) << std::setfill('0') << hours 
       << ":" 
       << std::setw(2) << std::setfill('0') << minutes 
       << ":" 
       << std::setw(2) << std::setfill('0')<< secs;

    return ss.str();
    
}
