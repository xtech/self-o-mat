#ifndef COBS_H
#define COBS_H

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


ByteSequence cobs_encode(const ByteSequence &input);


ByteSequence cobs_decode(const ByteSequence &input );


} // end namespace





#endif