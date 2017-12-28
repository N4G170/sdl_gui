#include "sdl_gui_uid.hpp"
#include "sdl_gui_log.hpp"

#include <functional>
#include <mutex>
#include <chrono>
#include <sstream>
#include <ctime>
#include <iomanip>//put_time

namespace sdl_gui
{

IID GenerateLocalID(IDType type)
{
    static std::mutex mutex;
    std::lock_guard<std::mutex> lock(mutex);//lock will release at the end of the function (out of scope)

    static_assert(BitSetSize<IID>::value == (BitSetSize<IID_COUNT>::value + BitSetSize<IID_CONTEXT>::value), "ERROR! Invalid UUID sizes.");
    static unsigned int s_uuid_count{0};
    // x bits reserved for context
    IID_CONTEXT context_id = static_cast<int>(type);
    // y bits for counting
    IID_COUNT count_id = s_uuid_count;
    //final uuid
    IID final_id;

    for(unsigned int i{0}; i < context_id.size(); ++i)
        final_id[final_id.size() - 1 - i] = context_id[ context_id.size() - 1 - i];

    for(unsigned int i{0}; i < count_id.size(); ++i)
        final_id[i] = count_id[i];

    ++s_uuid_count;

    return final_id;
}

UID GenerateUID()
{
    static std::mutex mutex;
    std::lock_guard<std::mutex> lock(mutex);//lock will release at the end of the function (out of scope)

    static size_t counter{0};//used to "avoid" equal string for use in the hash
    static std::string last_string{};

    //read current date time
    auto now = std::chrono::system_clock::now();
    auto now_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    //format datetime and convert it to a string
    struct tm buffer;
    #ifdef __linux__
    // ss << std::put_time(std::localtime(&now_time_t), "%X %d-%m-%Y");
    localtime_r(&now_time_t, &buffer);
    // ss << std::put_time(std::localtime(&now_time_t), "%Y-%m-%d %X");
    #elif _WIN32
	localtime_s(&buffer, &now_time_t);
    // ss << std::put_time(&buffer, "%X %d-%m-%Y");
    #endif
    ss << std::put_time(&buffer, "%X %d-%m-%Y");

    std::string new_string{ss.str()};

    if(last_string != new_string)//we moved to a new second
    {
        counter = 0;//reset counter
        last_string = new_string;
    }
    ++counter;

    //add counter to string
    new_string = std::to_string(counter) + new_string + std::to_string(counter);

    std::hash<std::string> std_hash;
    UID uid{std_hash(new_string)};

    return uid;
}
}//namespace
