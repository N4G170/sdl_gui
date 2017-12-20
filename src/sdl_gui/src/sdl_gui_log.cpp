#include "sdl_gui_log.hpp"
#include <iostream>
#include <fstream>
#include <mutex>

namespace sdl_gui
{

int e_default_log_target = LogTarget::CONSOLE | LogTarget::FILE;

void Log(const std::string& text)
{
    if((e_default_log_target & LogTarget::CONSOLE) == LogTarget::CONSOLE)
        LogToConsole(text);
    if((e_default_log_target  & LogTarget::FILE) == LogTarget::FILE)
        LogToFile(text);
}

void LogLine(const std::string& text)
{
    if((e_default_log_target & LogTarget::CONSOLE) == LogTarget::CONSOLE)
        LogLineToConsole(text);
    if((e_default_log_target  & LogTarget::FILE) == LogTarget::FILE)
        LogLineToFile(text);
}

void LogToConsole(const std::string& text)
{
    static std::mutex console_mutex;
    std::lock_guard<std::mutex> lock(console_mutex);

    std::cout<<text<<" ";
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
    static bool first_run{true};
    static std::mutex file_mutex;
    std::lock_guard<std::mutex> lock(file_mutex);

    if(first_run)
    {
        first_run = false;
        std::ofstream logfile (c_default_log_file, std::ofstream::out | std::ofstream::trunc);//clean file
        logfile << text;
        logfile.close();
    }
    else
    {
        std::ofstream logfile (c_default_log_file, std::ofstream::out | std::ofstream::app);//append
        logfile << text;
        logfile.close();
    }


    //the mutex will be released when the lock_guard obj goes out of scope
}

void LogLineToFile(const std::string& text)
{
    LogToFile(text + '\n');
    //the mutex will be released when the lock_guard obj goes out of scope
}

}//namespace
