#include "sdl_gui_log.hpp"
#include <iostream>
#include <mutex>

namespace sdl_gui
{

LogTarget e_default_log_target = LogTarget::CONSOLE;

void Log(const std::string& text)
{
    if(e_default_log_target == LogTarget::CONSOLE)
        LogToConsole(text);
    else if(e_default_log_target == LogTarget::FILE)
        LogToFile(text);
}

void LogLine(const std::string& text)
{
    if(e_default_log_target == LogTarget::CONSOLE)
        LogLineToConsole(text);
    else if(e_default_log_target == LogTarget::FILE)
        LogLineToFile(text);
}

void LogToConsole(const std::string& text)
{
    static std::mutex console_mutex;
    std::lock_guard<std::mutex> lock(console_mutex);

    std::cout<<text;
    std::cout.flush();//force printing
    //the mutex will be released when the lock_guard obj goes out of scope
}

void LogLineToConsole(const std::string& text)
{
    static std::mutex console_mutex;
    std::lock_guard<std::mutex> lock(console_mutex);

    std::cout<<text<<std::endl;
    //the mutex will be released when the lock_guard obj goes out of scope
}

void LogToFile(const std::string& text)
{
    static std::mutex file_mutex;
    std::lock_guard<std::mutex> lock(file_mutex);

    //TODO:Log to file
    LogToConsole(text);

    //the mutex will be released when the lock_guard obj goes out of scope
}

void LogLineToFile(const std::string& text)
{
    static std::mutex file_mutex;
    std::lock_guard<std::mutex> lock(file_mutex);

    //TODO:Log to file
    LogLineToConsole(text);

    //the mutex will be released when the lock_guard obj goes out of scope
}

}//namespace
