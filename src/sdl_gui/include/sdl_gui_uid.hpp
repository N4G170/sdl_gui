#include <bitset>

namespace sdl_gui
{

#ifndef SDL_GUI_UID_HPP
#define SDL_GUI_UID_HPP

//Incremental ID
using IID = std::bitset<16>;
//set of bit that will be incremented. the bit missing will be flags to mark the type of id
using IID_COUNT = std::bitset<12>;
using IID_CONTEXT = std::bitset<4>;

using UID = size_t;

enum class IDType
{
    GLOBAL = 0,
    GUI = 1
};

//allows the use of static_assert to check bitset size
template< typename >
struct BitSetSize;
//specialization for bitset
template< std::size_t S >
struct BitSetSize< std::bitset< S > > : std::integral_constant< std::size_t, S > {};

/**
 * \brief Generates a new IID (simple counter). Thread safe
 */
IID GenerateLocalID(IDType type = IDType::GLOBAL);

/**
 * \brief Generates a new UID (custom algorithm). Thread safe
 */
UID GenerateUID();

#endif//SDL_GUI_UID_HPP

}//namespace
