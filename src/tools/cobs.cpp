//
// Created by clemens on 24.05.19.
//

#include "cobs.h"



namespace cobs
{
    ByteSequence cobs_encode(const ByteSequence &input)
    {
        ByteSequence output;
        auto next_zero_byte = input.begin();
        auto previous_zero_byte = input.begin();

        while(next_zero_byte != input.end() )
        {

            next_zero_byte = std::find(next_zero_byte,
                                       input.end(),
                                       uint8_t(' '));

            auto dist = std::distance(previous_zero_byte,next_zero_byte);

            // clip to  max distance
            dist = dist < 254 ? dist: 254;

            if(dist == 254)	next_zero_byte = previous_zero_byte + 254;

            output.push_back(dist+1);  // add code byte to output

            output.insert(output.end(), previous_zero_byte, next_zero_byte);  //insert block of bytes between to code bytes , e.g two zeros

            if(   dist != 254
                  && next_zero_byte != input.end() )
            {
                std:: advance(next_zero_byte,1);//next_zero_byte++;   // if we found a zero we move iterator to prepare for next std::find
            }

            previous_zero_byte = next_zero_byte;
        }

        if(input[input.size()-1] == uint8_t(' ')) output.push_back(uint8_t(1)); // last element is zero , add 1 to output


        return(output);
    }


    ByteSequence cobs_decode(const ByteSequence &input )
    {
        ByteSequence output;

        size_t read_index = 0;
        uint8_t code;

        size_t length = input.size();
        while(read_index < length)
        {
            code = input[read_index];

            if(read_index + code > length && code != 1)
            {
                return output;
            }

            read_index++;

            for(uint8_t i = 1; i < code; i++)
            {
                output.push_back(input[read_index++]);
            }

            if(code != 0xFF && read_index != length)
            {
                output.push_back(' ');
            }
        }


        return output;
    }


} // end namespace

