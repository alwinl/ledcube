/*
 * Copyright 2023 Alwin Leerling <dna.leerling@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

#include <iostream>
#include <fstream>
#include <cstdint>
#include <iomanip>
#include <vector>
#include <algorithm>

using namespace std;

const uint8_t RED = 0x00;
const uint8_t GREEN = 0x01;
const uint8_t BLUE = 0x02;

#include "frames.h"

struct pattern_t
{
	std::string name;
	unsigned int length;
	uint8_t (*color_bit_func)(uint8_t, unsigned int);
};


void make_header( std::ostream& os )
{
	os <<
		"/*\n"
		" * THIS FILE IS GENERATED. DO NOT MODIFY, CHANGES WILL BE OVERWRITTEN.\n"
		" * SEE PROGRAM make_frames\n"
		" */\n"
		"\n"
		"#include \"frames.h\"\n"
		"\n";
}

void make_controls( std::ostream& os, std::vector<pattern_t> patterns )
{
	os << dec;

	unsigned int curr_pos = 0;

	os << "unsigned int animation_start[] = { ";
	for_each( patterns.begin(), patterns.end(),
		[&os, &curr_pos]( pattern_t& pattern )
		{
			if( curr_pos )
				os << ", ";
			os << curr_pos;
			curr_pos += pattern.length;
		}
	);
	os << " };\n";

	bool first = true;

	os << "unsigned int animation_lengths[] = { ";
	for_each( patterns.begin(), patterns.end(),
		[&os, &first]( pattern_t& pattern )
		{
			if( !first )
				os <<  ", ";
			os << pattern.length;
			first = false;
		}
	);
	os << " };\n",

	os << "unsigned int total_animations = " << patterns.size() << ";\n";
	os << "\n";
}

void make_animation_header( std::ostream& os )
{
	os <<
		"Frame frames[] =\n"
		"{\n";
}

void make_animation( std::ostream& os, std::string animation_name, unsigned int total_frames, uint8_t (*pfn)(uint8_t, unsigned int) )
{
	os << "\t// " << animation_name << "\n";

	for( uint8_t frame_count = 0; frame_count < total_frames; ++frame_count ) {

		Frame frame = {0,0,0};

		for( unsigned int bit_pos = 0; bit_pos<64; ++bit_pos ) {

			uint8_t colour_bits = (*pfn)( frame_count, bit_pos );

			if( colour_bits & (1 << RED)  ) frame.red   |= 1ull << bit_pos;
			if( colour_bits & (1 << GREEN)) frame.green |= 1ull << bit_pos;
			if( colour_bits & (1 << BLUE) ) frame.blue  |= 1ull << bit_pos;
		}

		os << std::setfill( '0' );
		os << "\t{ 0x" << hex << std::setw(16) << frame.red << ", 0x" << hex << std::setw(16) << frame.green << ", 0x" << hex << std::setw(16) << frame.blue << " },\n";
	}
}

void make_animation_footer( std::ostream& os )
{
	os << "};\n\n";
}

uint8_t get_rainbow_colours_bits( uint8_t frame_count, unsigned int bit_pos )
	{ return 1 << ((frame_count + (bit_pos / 16) ) % 3); }	// 16 bits per layer, so layer changes every 16 bits. mod(3) to wrap around

uint8_t get_walk_right_colours_bits( uint8_t frame_count, unsigned int bit_pos )
	{ return 1 << ((frame_count + (bit_pos % 16) / 4 ) % 3); }

uint8_t get_walk_back_colours_bits( uint8_t frame_count, unsigned int bit_pos )
	{ return 1 << ((frame_count + (bit_pos % 16) % 4 ) % 3); }

uint8_t get_walk_right_intermediate_colours_bits( uint8_t frame_count, unsigned int bit_pos )
	{ return (((frame_count+1) + (bit_pos % 16) / 4 ) % 0x06) + 1; } // We don't want 0, range is from 001 to 111, hence mod(6) + 1

uint8_t get_walk_back_intermediate_colours_bits( uint8_t frame_count, unsigned int bit_pos )
	{ return (((frame_count + 1) + (bit_pos % 16) % 4 ) % 0x06) + 1; } // We don't want 0, range is from 001 to 111, hence mod(6) + 1

uint8_t get_walk_down_intermediate_colours_bits( uint8_t frame_count, unsigned int bit_pos )
	{ return (( (frame_count + 1) + (bit_pos / 16) ) % 0x06) + 1; } // We don't want 0, range is from 001 to 111, hence mod(6) + 1 }

uint8_t get_intermediate_colour_bits( uint8_t frame_count, unsigned int bit_pos )
	{ return (( (frame_count + 1) + (bit_pos % 16) ) % 0x06) + 1; }


void make_animation_array( std::ostream& os, std::vector<pattern_t> patterns )
{
	make_animation_header( os );

	for_each( patterns.begin(), patterns.end(),
		[&os](pattern_t& pattern)
		{
			make_animation( os, pattern.name, pattern.length, pattern.color_bit_func );
		}
	);

	make_animation_footer( os );
}

std::vector<pattern_t> patterns =
{
	{ "Rainbow", 3, get_rainbow_colours_bits },
	{ "Walk right", 3, get_walk_right_colours_bits },
	{ "Walk back", 3, get_walk_back_colours_bits },
	{ "Walk right intermediate colours", 7, get_walk_right_intermediate_colours_bits },
	{ "Walk back intermediate colours", 7, get_walk_back_intermediate_colours_bits },
	{ "Walk down intermediate colours", 7, get_walk_down_intermediate_colours_bits },
	{ "Intermediate colours", 7, get_intermediate_colour_bits }
};

int main()
{
	std::ofstream source_file( "./frames.c");

	if( source_file.bad() ) {
		std::cout << "File descriptor bad!" << endl;
		return 1;
	}

	make_header( source_file );

	make_controls( source_file, patterns );

	make_animation_array( source_file, patterns );

	source_file.flush();
	source_file.close();

	return 0;
}
