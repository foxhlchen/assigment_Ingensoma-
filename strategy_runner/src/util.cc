
namespace util {

// return current time (hhmmss) add span (seconds)
long TimeCalculation(long curr, long span) {
    long hh = curr / 10000;
    long mm = (curr % 10000) / 100;
    long ss = curr % 100;

    long seconds = hh * 3600 + mm * 60 + ss;
    seconds += span;
    
    hh = seconds / 3600;
    mm = (seconds % 3600) / 60;
    ss = seconds % 60;

    return hh * 10000 + mm * 100 + ss;
}

} // namespace util