#include <type_traits>
#include "SDL.h"
#include "sdl_gui_structs.hpp"
#include <string>

namespace sdl_gui
{

#ifndef SDL_GUI_UTILS
#define SDL_GUI_UTILS

//<f> Enum Casts
/**
 * Cast a enum entry to its inner type (default int)
 */
template<typename T>
// constexpr auto ToInnerType(T enum_entry) noexcept // needs c++14
constexpr typename std::underlying_type<T>::type ToInnerType(T enum_entry) noexcept
{
    return static_cast<typename std::underlying_type<T>::type>(enum_entry);
}

template<typename T>
constexpr bool IsFlagActive(int flag, T enum_entry) noexcept
{
    return flag & ToInnerType(enum_entry);
}
//</f>

//<f> Rect Intersections
//intersect
bool PointInsideRect(const SDL_Rect& rect_1, int x, int y);
bool RectIntersect(const SDL_Rect& rect_1, const SDL_Rect& rect_2);
//</f>

//<f> Simple casts
//rect casts
SDL_Rect RectFromFloats(float x, float y, float w, float h);
SDL_Rect RectFromStructs(Position position, Dimensions size);

//position cast
Position PositionFromInts(int x, int y);
//size cast
Dimensions SizeFromInts(int w, int h);
//</f>

//<f> ToString()
std::string ToString(const SDL_Rect& rect);
std::string ToString(const Position& position);
std::string ToString(const Dimensions& size);
std::string ToString(const Position& position, const Dimensions& size);
//</f>

//<f> Maths
/**
 * \brief Limits the value to the interval between min and max
 * @param value value to be checked
 * @param min   min value of the interval
 * @param max   max value of the interval
 * \return  the final value
 */
template<typename T>
T KeepInInterval(T value, T min, T max)
{
    if(value < min)
        value = min;
    else if(value > max)
        value = max;
    return value;
}

template<typename T>
T MapValueToRange(T value, T original_min, T original_max, T new_min, T new_max)
{
    if(original_max - original_min <= 0 || new_max - new_min <= 0)
        return new_min;

    auto ratio = (value - original_min) / (original_max - original_min);//move range to origin (EX. (0,0)) and calculate ratio

    //apply ratio to new_max(moved to origin) and then reposition the value inside new range (with + new_min, to move the value to the correct range)
    return (new_max - new_min) * ratio + new_min;
}

/**
 * \brief The ratio = (value - original_min) / (original_max - original_min)
 */
template<typename T>
T MapValueToRange(T ratio, T new_min, T new_max)
{
    if(new_max - new_min <= 0)
        return new_min;
    //apply ratio to new_max(moved to origin) and then reposition the value inside new range (with + new_min, to move the value to the correct range)
    return (new_max - new_min) * ratio + new_min;
}

//</f>

#endif//SDL_GUI_UTILS

}
