#include <bitset>
#include <string>
#include <vector>

namespace sdl_gui
{

#ifndef UTF8_HPP
#define UTF8_HPP


struct UTF8_BYTE
{
    const unsigned char start_byte_1{ 0b00000000 };
    const unsigned char start_byte_2{ 0b11000000 };
    const unsigned char start_byte_3{ 0b11100000 };
    const unsigned char start_byte_4{ 0b11110000 };
    const unsigned char extra_byte  { 0b10000000 };
} const utf8_byte;

int DecodeCharacterUTF8( const char* character);
std::vector<int> DecodeStringUTF8(const std::string& text);
std::vector<int> DecodeStringUTF8(const char* text, int size);

#endif//UTF8_HPP
}//namespace
