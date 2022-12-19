#include <iostream>
#include <fstream>
#include <cstdint>
#include <iomanip>
#include <bitset>
#include <limits>

using namespace std;

const uint8_t RED = 0x00;
const uint8_t GREEN = 0x01;
const uint8_t BLUE = 0x02;

#include "frames.h"

/*
struct Frame
{
	uint64_t red;
	uint64_t green;
	uint64_t blue;
};
*/
std::ostream& make_rainbow_frames( std::ostream& os )
{
	os << "\t// Rainbow\n";

	for( uint8_t colour_base = 0; colour_base < 3; ++colour_base ) {

		Frame frame = {0,0,0};

		for( uint8_t layer = 0; layer < 4; ++layer ) {
			for( uint8_t position = 0; position < 16; ++position ) {

				uint8_t colour_bits = 1 << ((colour_base + layer ) % 3);
				unsigned int bit_pos = layer * 16 + position;

				if( colour_bits & (1 << RED)  ) frame.red   |= 1ull << bit_pos;
				if( colour_bits & (1 << GREEN)) frame.green |= 1ull << bit_pos;
				if( colour_bits & (1 << BLUE) ) frame.blue  |= 1ull << bit_pos;
			}
		}
		os << std::setfill( '0' );
		os << "\t{ 0x" << hex << std::setw(16) << frame.red << ", 0x" << hex << std::setw(16) << frame.green << ", 0x" << hex << std::setw(16) << frame.blue << " },\n";
	}

	return os;
}

std::ostream& walk_right( std::ostream& os )
{
	os << "\t// Walk right\n";

	for( uint8_t colour_base = 0; colour_base < 3; ++colour_base ) {

		Frame frame = {0,0,0};

		for( uint8_t layer = 0; layer < 4; ++layer ) {
			for( uint8_t position = 0; position < 16; ++position ) {

				uint8_t colour = (colour_base + position / 4 ) % 3;
				uint8_t colour_bits = 1 << colour;
				unsigned int bit_pos = layer * 16 + position;

				if( colour_bits & (1 << RED)  ) frame.red   |= 1ull << bit_pos;
				if( colour_bits & (1 << GREEN)) frame.green |= 1ull << bit_pos;
				if( colour_bits & (1 << BLUE) ) frame.blue  |= 1ull << bit_pos;
			}
		}
		os << std::setfill( '0' );
		os << "\t{ 0x" << hex << std::setw(16) << frame.red << ", 0x" << hex << std::setw(16) << frame.green << ", 0x" << hex << std::setw(16) << frame.blue << " },\n";
	}

	return os;
}

std::ostream& walk_back( std::ostream& os )
{
	os << "\t// Walk back\n";

	for( uint8_t colour_base = 0; colour_base < 3; ++colour_base ) {

		Frame frame = {0,0,0};

		for( uint8_t layer = 0; layer < 4; ++layer ) {
			for( uint8_t position = 0; position < 16; ++position ) {

				uint8_t colour = (colour_base + position % 4 ) % 3;
				uint8_t colour_bits = 1 << colour;
				unsigned int bit_pos = layer * 16 + position;

				if( colour_bits & (1 << RED)  ) frame.red   |= 1ull << bit_pos;
				if( colour_bits & (1 << GREEN)) frame.green |= 1ull << bit_pos;
				if( colour_bits & (1 << BLUE) ) frame.blue  |= 1ull << bit_pos;
			}
		}
		os << std::setfill( '0' );
		os << "\t{ 0x" << hex << std::setw(16) << frame.red << ", 0x" << hex << std::setw(16) << frame.green << ", 0x" << hex << std::setw(16) << frame.blue << " },\n";
	}

	return os;
}

std::ostream& walk_right_intermediate_colours( std::ostream& os )
{
	os << "\t// Walk right intermediate colours\n";

	for( uint8_t colour_bits_base = 1; colour_bits_base < 8; ++colour_bits_base ) {

		Frame frame = {0,0,0};

		for( uint8_t layer = 0; layer < 4; ++layer ) {
			for( uint8_t position = 0; position < 16; ++position ) {

				uint8_t colour_bits = ((colour_bits_base + position / 4 ) % 0x06) + 1;
				unsigned int bit_pos = layer * 16 + position;

				if( colour_bits & (1 << RED)  ) frame.red   |= 1ull << bit_pos;
				if( colour_bits & (1 << GREEN)) frame.green |= 1ull << bit_pos;
				if( colour_bits & (1 << BLUE) ) frame.blue  |= 1ull << bit_pos;
			}
		}
		os << std::setfill( '0' );
		os << "\t{ 0x" << hex << std::setw(16) << frame.red << ", 0x" << hex << std::setw(16) << frame.green << ", 0x" << hex << std::setw(16) << frame.blue << " },\n";
	}

	return os;
}

std::ostream& walk_back_intermediate_colours( std::ostream& os )
{
	os << "\t// Walk back intermediate colours\n";

	for( uint8_t colour_bits_base = 1; colour_bits_base < 8; ++colour_bits_base ) {

		Frame frame = {0,0,0};

		for( uint8_t layer = 0; layer < 4; ++layer ) {
			for( uint8_t position = 0; position < 16; ++position ) {

				uint8_t colour_bits = ((colour_bits_base + position % 4 ) % 0x06) + 1;
				unsigned int bit_pos = layer * 16 + position;

				if( colour_bits & (1 << RED)  ) frame.red   |= 1ull << bit_pos;
				if( colour_bits & (1 << GREEN)) frame.green |= 1ull << bit_pos;
				if( colour_bits & (1 << BLUE) ) frame.blue  |= 1ull << bit_pos;
			}
		}
		os << std::setfill( '0' );
		os << "\t{ 0x" << hex << std::setw(16) << frame.red << ", 0x" << hex << std::setw(16) << frame.green << ", 0x" << hex << std::setw(16) << frame.blue << " },\n";
	}

	return os;
}

std::ostream& walk_down_intermediate_colours( std::ostream& os )
{
	os << "\t// Walk down intermediate colours\n";

	for( uint8_t colour_bits_base = 1; colour_bits_base < 8; ++colour_bits_base ) {

		Frame frame = {0,0,0};

		for( uint8_t layer = 0; layer < 4; ++layer ) {
			for( uint8_t position = 0; position < 16; ++position ) {

				uint8_t colour_bits = ((colour_bits_base + layer ) % 0x06) + 1;
				unsigned int bit_pos = layer * 16 + position;

				if( colour_bits & (1 << RED)  ) frame.red   |= 1ull << bit_pos;
				if( colour_bits & (1 << GREEN)) frame.green |= 1ull << bit_pos;
				if( colour_bits & (1 << BLUE) ) frame.blue  |= 1ull << bit_pos;
			}
		}
		os << std::setfill( '0' );
		os << "\t{ 0x" << hex << std::setw(16) << frame.red << ", 0x" << hex << std::setw(16) << frame.green << ", 0x" << hex << std::setw(16) << frame.blue << " },\n";
	}

	return os;
}

std::ostream& intermediate_colours( std::ostream& os )
{
	os << "\t// Intermediate colours\n";

	for( uint8_t colour_bits_base = 1; colour_bits_base < 8; ++colour_bits_base ) {

		Frame frame = {0,0,0};

		for( uint8_t layer = 0; layer < 4; ++layer ) {
			for( uint8_t position = 0; position < 16; ++position ) {

				uint8_t colour_bits = ((colour_bits_base + position ) % 0x06) + 1;
				unsigned int bit_pos = layer * 16 + position;

				if( colour_bits & (1 << RED)  ) frame.red   |= 1ull << bit_pos;
				if( colour_bits & (1 << GREEN)) frame.green |= 1ull << bit_pos;
				if( colour_bits & (1 << BLUE) ) frame.blue  |= 1ull << bit_pos;
			}
		}
		os << std::setfill( '0' );
		os << "\t{ 0x" << hex << std::setw(16) << frame.red << ", 0x" << hex << std::setw(16) << frame.green << ", 0x" << hex << std::setw(16) << frame.blue << " },\n";
	}

	return os;
}


int make_frame()
{
	std::ofstream source_file( "./frames.c");

	if( source_file.bad() ) {
		std::cout << "File descriptor bad!" << endl;
		return 1;
	}

	source_file <<
	"#include \"frames.h\"\n"
	"\n"
	"Frame frames[] =\n"
	"{\n";

	make_rainbow_frames( source_file );
	walk_right( source_file );
	walk_back( source_file );
	walk_right_intermediate_colours( source_file );
	walk_back_intermediate_colours( source_file );
	walk_down_intermediate_colours( source_file );
	intermediate_colours( source_file );

	source_file << "};\n\n";

	source_file << "unsigned int animation_start[] = { 0, 3, 6, 9, 16, 23, 30 };\n";

	source_file.flush();

	source_file.close();
    return 0;
}

int test_frame()
{
#ifndef MAKE_FRAME_C
	for( unsigned int idx = 0; idx < 7; ++idx ) {
		std::cout <<  "Got sequence at " << animation_start[idx] << std::endl;
	}
#endif // MAKE_FRAME_C
	return 0;
}

int main()
{
#ifdef MAKE_FRAME_C
	make_frame();
#else
	test_frame();
#endif // MAKE_FRAME_C
}
