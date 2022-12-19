/*
 * Copyright 2022 Alwin Leerling <dna.leerling@gmail.com>
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

#include "ledcube.h"
#include "tick.h"

#include "make_frames/frames.h"

const uint8_t NO_OF_ANIMATIONS = 7;

const uint8_t RED = 0x00;
const uint8_t GREEN = 0x01;
const uint8_t BLUE = 0x02;


//unsigned int animation_start[] = { 0, 3, 6, 9, 16, 23, 30 };
unsigned int animation_lengths[] = { 3, 3, 3, 7,  7,  7,  7 };

void display_frame( Frame* frame )
{
	for( uint8_t layer = 0; layer < 4; ++layer ) {
		for( uint8_t position = 0; position < 16; ++position ) {

			uint8_t bit_idx = layer * 16 + position;

			if( (frame->red   >> bit_idx) & 0x01 ) turn_led_on( (RED   << 6) | ((layer & 0x03) << 4) | (position & 0x0F ) );
			if( (frame->green >> bit_idx) & 0x01 ) turn_led_on( (GREEN << 6) | ((layer & 0x03) << 4) | (position & 0x0F ) );
			if( (frame->blue  >> bit_idx) & 0x01 ) turn_led_on( (BLUE  << 6) | ((layer & 0x03) << 4) | (position & 0x0F ) );
		}
	}
}

uint8_t next_frame_index( uint8_t current_idx, uint8_t animation_idx )
{
	uint8_t sequence_idx = current_idx - animation_start[animation_idx];

	sequence_idx = (sequence_idx + 1) % animation_lengths[animation_idx];

	return sequence_idx + animation_start[animation_idx];
}

uint8_t next_animation_index( uint8_t current_idx )
{
	return (current_idx + 1) % NO_OF_ANIMATIONS;
}

void run_cube()
{
	static uint32_t last_tick_count = 0;
	static uint32_t animation_ticks = 20;
	static uint8_t frame_idx = 0;
	static uint8_t animation_idx = 0;
	uint32_t new_tick_count = sys_tick_get_tickcount();

	if( new_tick_count > last_tick_count + 250 ) {

		frame_idx = next_frame_index( frame_idx, animation_idx );

		if( ! --animation_ticks ) {			// five seconds
			animation_idx = next_animation_index( animation_idx );
			frame_idx = animation_start[animation_idx];

			animation_ticks = 20;
		}

		last_tick_count = new_tick_count;
	}

	display_frame( &frames[frame_idx] );
}
