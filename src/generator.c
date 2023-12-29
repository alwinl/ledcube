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
#include "generator.h"

#include "frames.h"

const uint8_t RED = 0x00;
const uint8_t GREEN = 0x01;
const uint8_t BLUE = 0x02;

static uint8_t get_sequence_idx( uint8_t animation_idx )
{
	static uint8_t sequence_idx = 0;

	sequence_idx = (sequence_idx + 1) % animation_lengths[animation_idx];

	return sequence_idx;
}

static uint8_t get_animation_idx( void )
{
	static uint8_t animation_ticks = 0;
	static uint8_t animation_idx = 0;

	++animation_ticks;

	if( animation_ticks < 20 )			// five seconds have not passed yet
		return animation_idx;

	animation_idx = (animation_idx + 1) % total_animations;
	animation_ticks = 0;

	return animation_idx;
}

static uint8_t get_frame_idx( void )
{
	static uint32_t last_tick_count = 0;
	static uint8_t frame_idx = 0;

	uint32_t new_tick_count = sys_tick_get_tickcount();

	if( new_tick_count < last_tick_count + 250 )
		return frame_idx;

	uint8_t animation_idx = get_animation_idx();
	frame_idx = animation_start[animation_idx] + get_sequence_idx( animation_idx );
	last_tick_count = new_tick_count;

	return frame_idx;
}

void run_cube( void )
{
	Frame* frame = &frames[ get_frame_idx() ];

	for( uint8_t layer = 0; layer < 4; ++layer ) {
		for( uint8_t position = 0; position < 16; ++position ) {

			uint64_t bit_pos = 1 << (layer * 16 + position);

			if( frame->red   & bit_pos ) turn_led_on( (RED   << 6) | ((layer & 0x03) << 4) | (position & 0x0F ) );
			if( frame->green & bit_pos ) turn_led_on( (GREEN << 6) | ((layer & 0x03) << 4) | (position & 0x0F ) );
			if( frame->blue  & bit_pos ) turn_led_on( (BLUE  << 6) | ((layer & 0x03) << 4) | (position & 0x0F ) );

		}
	}
}
