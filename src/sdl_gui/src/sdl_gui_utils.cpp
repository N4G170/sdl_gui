#include "sdl_gui_utils.hpp"
#include "cmath"

namespace sdl_gui
{

bool PointInsideRect(const SDL_Rect& rect_1, int x, int y)
{
    //no need for another algorithm as SDL_Rect never rotate
    return x >= rect_1.x && x <= rect_1.x + rect_1.w && y >= rect_1.y && y <= rect_1.y + rect_1.h;
}

bool RectIntersect(const SDL_Rect& rect_1, const SDL_Rect& rect_2)
{
    //check if any of rect_2 vertices is inside rect_1
    //top left
    if(PointInsideRect(rect_1, rect_2.x, rect_2.y))
        return true;
    //top right
    if(PointInsideRect(rect_1, rect_2.x+ rect_2.w, rect_2.y))
        return true;
    //Bottom right
    if(PointInsideRect(rect_1, rect_2.x, rect_2.y + rect_2.h))
        return true;
    //Bottom left
    if(PointInsideRect(rect_1, rect_2.x + rect_2.w, rect_2.y + rect_2.h))
        return true;

    return false;//default result
}

//<f> Simple casts
//rect casts
SDL_Rect RectFromFloats(float x, float y, float w, float h)
{
    return {static_cast<int>(std::round(x)), static_cast<int>(std::round(y)), static_cast<int>(std::round(w)), static_cast<int>(std::round(h))};
}

SDL_Rect RectFromStructs(Position position, Dimensions size)
{
    return {static_cast<int>(std::round(position.x)), static_cast<int>(std::round(position.y)), static_cast<int>(std::round(size.w)), static_cast<int>(std::round(size.h))};
}

//position cast
Position PositionFromInts(int x, int y)
{
    return{static_cast<float>(x), static_cast<float>(y)};
}

//size cast
Dimensions SizeFromInts(int w, int h)
{
    return{static_cast<float>(w), static_cast<float>(h)};
}
//</f>

//<f> ToString()
std::string ToString(const SDL_Rect& rect)
{
    return "("+std::to_string(rect.x)+", "+std::to_string(rect.y)+") ["+std::to_string(rect.w)+", "+std::to_string(rect.h)+"]";
}

std::string ToString(const Position& position)
{
    return "("+std::to_string(position.x)+", "+std::to_string(position.y)+")";
}

std::string ToString(const Dimensions& size)
{
    return "["+std::to_string(size.w)+", "+std::to_string(size.h)+"]";
}

std::string ToString(const Position& position, const Dimensions& size)
{
    return "("+std::to_string(position.x)+", "+std::to_string(position.y)+") ["+std::to_string(size.w)+", "+std::to_string(size.h)+"]";
}
//</f>

//<f> Math

//</f>

}//namespace
