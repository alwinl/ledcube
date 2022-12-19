/*
 * Copyright 2021 Alwin Leerling <dna.leerling@gmail.com>
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
 *
 *
 */

#include "ledcube.h"
#include "tick.h"

const uint8_t RED = 0x00;
const uint8_t GREEN = 0x01;
const uint8_t BLUE = 0x02;

static void rainbow()
{
	for( uint8_t colour_base = 0; colour_base < 3; ++colour_base ) {

		for( uint32_t start = sys_tick_get_tickcount(); sys_tick_get_tickcount() - start < 250;  ) {

			for( uint8_t layer = 0; layer < 4; ++layer ) {
				for( uint8_t position = 0; position < 16; ++position ) {

					uint8_t colour = (colour_base + layer ) % 3;
					uint8_t colour_bits = 1 << colour;

					if( colour_bits & (1 << RED)  ) turn_led_on( (RED   << 6) | ((layer & 0x03) << 4) | (position & 0x0F ) );
					if( colour_bits & (1 << GREEN)) turn_led_on( (GREEN << 6) | ((layer & 0x03) << 4) | (position & 0x0F ) );
					if( colour_bits & (1 << BLUE) ) turn_led_on( (BLUE  << 6) | ((layer & 0x03) << 4) | (position & 0x0F ) );
				}
			}
		}
	}
}

static void walk_right()
{
	for( uint8_t colour_base = 0; colour_base < 3; ++colour_base ) {

		for( uint32_t start = sys_tick_get_tickcount(); sys_tick_get_tickcount() - start < 250;  ) {

			for( uint8_t layer = 0; layer < 4; ++layer ) {
				for( uint8_t position = 0; position < 16; ++position ) {

					uint8_t colour = (colour_base + position / 4 ) % 3;
					uint8_t colour_bits = 1 << colour;

					if( colour_bits & (1 << RED)  ) turn_led_on( (RED   << 6) | ((layer & 0x03) << 4) | (position & 0x0F ) );
					if( colour_bits & (1 << GREEN)) turn_led_on( (GREEN << 6) | ((layer & 0x03) << 4) | (position & 0x0F ) );
					if( colour_bits & (1 << BLUE) ) turn_led_on( (BLUE  << 6) | ((layer & 0x03) << 4) | (position & 0x0F ) );
				}
			}
		}
	}
}

static void walk_back()
{
	for( uint8_t colour_base = 0; colour_base < 3; ++colour_base ) {

		for( uint32_t start = sys_tick_get_tickcount(); sys_tick_get_tickcount() - start < 250;  ) {

			for( uint8_t layer = 0; layer < 4; ++layer ) {
				for( uint8_t position = 0; position < 16; ++position ) {

					uint8_t colour = (colour_base + position % 4 ) % 3;
					uint8_t colour_bits = 1 << colour;

					if( colour_bits & (1 << RED)  ) turn_led_on( (RED   << 6) | ((layer & 0x03) << 4) | (position & 0x0F ) );
					if( colour_bits & (1 << GREEN)) turn_led_on( (GREEN << 6) | ((layer & 0x03) << 4) | (position & 0x0F ) );
					if( colour_bits & (1 << BLUE) ) turn_led_on( (BLUE  << 6) | ((layer & 0x03) << 4) | (position & 0x0F ) );
				}
			}
		}
	}
}

static void walk_right_intermediate_colours()
{
	for( uint8_t colour_bits_base = 1; colour_bits_base < 8; ++colour_bits_base ) {

		for( uint32_t start = sys_tick_get_tickcount(); sys_tick_get_tickcount() - start < 250;  ) {

			for( uint8_t layer = 0; layer < 4; ++layer ) {
				for( uint8_t position = 0; position < 16; ++position ) {

					uint8_t colour_bits = ((colour_bits_base + position / 4 ) % 0x06) + 1;

					if( colour_bits & (1 << RED)  ) turn_led_on( (RED   << 6) | ((layer & 0x03) << 4) | (position & 0x0F ) );
					if( colour_bits & (1 << GREEN)) turn_led_on( (GREEN << 6) | ((layer & 0x03) << 4) | (position & 0x0F ) );
					if( colour_bits & (1 << BLUE) ) turn_led_on( (BLUE  << 6) | ((layer & 0x03) << 4) | (position & 0x0F ) );
				}
			}
		}
	}
}

static void walk_back_intermediate_colours()
{
	for( uint8_t colour_bits_base = 1; colour_bits_base < 8; ++colour_bits_base ) {

		for( uint32_t start = sys_tick_get_tickcount(); sys_tick_get_tickcount() - start < 250;  ) {

			for( uint8_t layer = 0; layer < 4; ++layer ) {
				for( uint8_t position = 0; position < 16; ++position ) {

					uint8_t colour_bits = ((colour_bits_base + position % 4 ) % 0x06) + 1;

					if( colour_bits & (1 << RED)  ) turn_led_on( (RED   << 6) | ((layer & 0x03) << 4) | (position & 0x0F ) );
					if( colour_bits & (1 << GREEN)) turn_led_on( (GREEN << 6) | ((layer & 0x03) << 4) | (position & 0x0F ) );
					if( colour_bits & (1 << BLUE) ) turn_led_on( (BLUE  << 6) | ((layer & 0x03) << 4) | (position & 0x0F ) );
				}
			}
		}
	}
}

static void walk_down_intermediate_colours()
{
	for( uint8_t colour_bits_base = 1; colour_bits_base < 8; ++colour_bits_base ) {

		for( uint32_t start = sys_tick_get_tickcount(); sys_tick_get_tickcount() - start < 250;  ) {

			for( uint8_t layer = 0; layer < 4; ++layer ) {
				for( uint8_t position = 0; position < 16; ++position ) {

					uint8_t colour_bits = ((colour_bits_base + layer ) % 0x06) + 1;

					if( colour_bits & (1 << RED)  ) turn_led_on( (RED   << 6) | ((layer & 0x03) << 4) | (position & 0x0F ) );
					if( colour_bits & (1 << GREEN)) turn_led_on( (GREEN << 6) | ((layer & 0x03) << 4) | (position & 0x0F ) );
					if( colour_bits & (1 << BLUE) ) turn_led_on( (BLUE  << 6) | ((layer & 0x03) << 4) | (position & 0x0F ) );
				}
			}
		}
	}
}

static void intermediate_colours()
{
	for( uint8_t colour_bits_base = 1; colour_bits_base < 8; ++colour_bits_base ) {

		for( uint32_t start = sys_tick_get_tickcount(); sys_tick_get_tickcount() - start < 250;  ) {

			for( uint8_t layer = 0; layer < 4; ++layer ) {
				for( uint8_t position = 0; position < 16; ++position ) {

					uint8_t colour_bits = ((colour_bits_base + position ) % 0x06) + 1;

					if( colour_bits & (1 << RED)  ) turn_led_on( (RED   << 6) | ((layer & 0x03) << 4) | (position & 0x0F ) );
					if( colour_bits & (1 << GREEN)) turn_led_on( (GREEN << 6) | ((layer & 0x03) << 4) | (position & 0x0F ) );
					if( colour_bits & (1 << BLUE) ) turn_led_on( (BLUE  << 6) | ((layer & 0x03) << 4) | (position & 0x0F ) );
				}
			}
		}
	}
}

void run_cube()
{
    for( uint32_t start = sys_tick_get_tickcount(); sys_tick_get_tickcount() - start < 5000;  )
        walk_back_intermediate_colours();

    for( uint32_t start = sys_tick_get_tickcount(); sys_tick_get_tickcount() - start < 5000;  )
        rainbow();

    for( uint32_t start = sys_tick_get_tickcount(); sys_tick_get_tickcount() - start < 5000;  )
        walk_right();

    for( uint32_t start = sys_tick_get_tickcount(); sys_tick_get_tickcount() - start < 5000;  )
        walk_back();

    for( uint32_t start = sys_tick_get_tickcount(); sys_tick_get_tickcount() - start < 5000;  )
        walk_down_intermediate_colours();

    for( uint32_t start = sys_tick_get_tickcount(); sys_tick_get_tickcount() - start < 5000;  )
        walk_right_intermediate_colours();

    for( uint32_t start = sys_tick_get_tickcount(); sys_tick_get_tickcount() - start < 5000;  )
        intermediate_colours();
}
