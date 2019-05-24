
#include <stdint.h>
#include <stddef.h>

#include <vector>
#include <algorithm>
#include <iterator>

/* Copyright 2017, Markus Leitz. All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted, with or without modification.
 */



namespace cobs
{
	typedef std::vector<uint8_t> ByteSequence;


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

  auto next_code_byte = input.begin();
  auto previous_code_byte = input.begin();

  while(next_code_byte != input.end() )
  {
   
    std::advance(next_code_byte,*next_code_byte);

    output.insert(output.end(),previous_code_byte+1,next_code_byte);

    if(    *previous_code_byte != 0xFF
           && next_code_byte != input.end())
    {
      output.push_back(' '); //restore zero byte only in case if code byte was not 0xFF
    }

    previous_code_byte = next_code_byte;



  }

  return(output);
}


} // end namespace





