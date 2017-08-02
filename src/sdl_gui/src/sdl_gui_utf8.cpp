#include "sdl_gui_utf8.hpp"
#include <iostream>

namespace sdl_gui
{

int DecodeCharacterUTF8(const char* character)
{
    int result{0};

    //check charater length
    if(character[0] >= 0)//one byte char. if negative it means that it belong to a multibyte char
    {
        return static_cast<int>(character[0]);
    }
    else if((character[0] & utf8_byte.start_byte_4) == utf8_byte.start_byte_4)//four byte char
    {
        std::cout << "Not implemented" << "\n";
    }
    else if((character[0] & utf8_byte.start_byte_3) == utf8_byte.start_byte_3)//three byte char
    {
        std::bitset<8> b1{static_cast<typename std::make_unsigned<char>::type>(character[0])};//need 4 bits from right. goes on the leftmost side
        std::bitset<8> b2{static_cast<typename std::make_unsigned<char>::type>(character[1])};//need 6 bits from right
        std::bitset<8> b3{static_cast<typename std::make_unsigned<char>::type>(character[2])};//need 6 bits from right
        std::bitset<16> result;

        for(int i{0}; i < 6; ++i)
        {
            result[i] = b3[i];
            result[i + 6] = b2[i];
        }
        //fill missing bits
        for(int i{0}; i < 4; ++i)
            result[i + 12] = b1[i];

        return static_cast<int>(result.to_ulong());
    }
    else if((character[0] & utf8_byte.start_byte_2) == utf8_byte.start_byte_2)//two byte char
    {
        std::bitset<8> b1{static_cast<typename std::make_unsigned<char>::type>(character[0])};//need 5 bits from right. goes on the left
        std::bitset<8> b2{static_cast<typename std::make_unsigned<char>::type>(character[1])};//need 6 bits from right
        std::bitset<16> result;

        for(int i{0}; i < 5; ++i)
        {
            result[i] = b2[i];
            result[i + 6] = b1[i];
        }
        //fill missing bit
        result[5] = b2[5];

        return static_cast<int>(result.to_ulong());
    }
    else
    {
        std::cout << "Character decode error." << "\n";
    }

    return result;
}

std::vector<int> DecodeStringUTF8(const std::string& text)
{
    return DecodeStringUTF8(text.data(), text.size());
}

std::vector<int> DecodeStringUTF8(const char* text, int size)
{
    std::vector<int> decoded_string;

    for(int i{0}; i < size; )
    {
        //check charater length
        if(text[i] >= 0)//one byte char. if negative it means that it belong to a multibyte char
        {
            decoded_string.push_back(static_cast<int>(text[i]));
            ++i;//move to next char
        }
        else if((text[i] & utf8_byte.start_byte_4) == utf8_byte.start_byte_4)//four byte char
        {
            std::cout << "Not implemented" << "\n";
            i += 4;//move to next char by jumping over four bytes
        }
        else if((text[i] & utf8_byte.start_byte_3) == utf8_byte.start_byte_3)//three byte char
        {
            std::bitset<8> b1{static_cast<typename std::make_unsigned<char>::type>(text[i])};//need 4 bits from right. goes on the leftmost side
            std::bitset<8> b2{static_cast<typename std::make_unsigned<char>::type>(text[i + 1])};//need 6 bits from right
            std::bitset<8> b3{static_cast<typename std::make_unsigned<char>::type>(text[i + 2])};//need 6 bits from right
            std::bitset<16> result;

            for(int i{0}; i < 6; ++i)
            {
                result[i] = b3[i];
                result[i + 6] = b2[i];
            }
            //fill missing bits
            for(int i{0}; i < 4; ++i)
                result[i + 12] = b1[i];

            decoded_string.push_back(static_cast<int>(result.to_ulong()));

            i += 3;//move to next char by jumping over three bytes
        }
        else if((text[i] & utf8_byte.start_byte_2) == utf8_byte.start_byte_2)//two byte char
        {
            std::bitset<8> b1{static_cast<typename std::make_unsigned<char>::type>(text[i])};//need 5 bits from right. goes on the left
            std::bitset<8> b2{static_cast<typename std::make_unsigned<char>::type>(text[i + 1])};//need 6 bits from right
            std::bitset<16> result;

            for(int i{0}; i < 5; ++i)
            {
                result[i] = b2[i];
                result[i + 6] = b1[i];
            }
            //fill missing bit
            result[5] = b2[5];

            decoded_string.push_back(static_cast<int>(result.to_ulong()));

            i += 2;//move to next char by jumping over two bytes
        }
        else
        {
            std::cout << "Decode error." << "\n";
        }
    }

    return decoded_string;
}

}//namespace
