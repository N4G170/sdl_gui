#include <string>
#include <iostream>

namespace sdl_gui
{

#ifndef SDL_GUI_LOG_HPP
#define SDL_GUI_LOG_HPP

enum LogTarget
{
    CONSOLE = 1,
    FILE = 2
};

/**
 * Variable to set the default log target used by void Log(const std::string& text);
 * eg: LogTarget::CONSOLE to log to console
 */
extern int e_default_log_target;
const std::string c_default_log_file{"log.txt"};

void Log();
void LogLine();

void Log(const std::string& text);
void LogLine(const std::string& text);
void LogToConsole(const std::string& text);
void LogLineToConsole(const std::string& text);
void LogToFile(const std::string& text);
void LogLineToFile(const std::string& text);


#endif //SDL_GUI_LOG_HPP

}//namespace
