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

#include <libopencm3/stm32/gpio.h>

/*
 * Unfortunately the amount of wires to solder got to me.
 * So the position of each led in the layer jumps a bit around
 * I have written a utility to produce the logical to physical mapping.
 * This utility creates this map_table array
 * (See /home/alwin/Documents/Programming/led_cube/Mapper)
 */
#include "table.h"
#include "ledcube.h"
#include "tick.h"

static pin_t reset_pin;
static pin_t shift_clock_pin;
static pin_t store_clock_pin;
static pin_t serial_out_pin;


/*
 * Assert and de-assert the Master Reset line to clear all the 595's
 */
void ledcube_init( ledcube_pins_t * pins )
{
    reset_pin = pins->reset;
    shift_clock_pin = pins->shift_clock;
    store_clock_pin = pins->store_clock;
    serial_out_pin = pins->serial_out;

    gpio_clear( reset_pin.port, reset_pin.pin );
    gpio_clear( reset_pin.port, reset_pin.pin );    // make it a decent pulse
    gpio_clear( reset_pin.port, reset_pin.pin );
    gpio_set( reset_pin.port, reset_pin.pin );
}

/*
 * @brief Turns on one colour of one led in the cube
 *
 * This function will shift out 56 bits of data to the 595's
 * 16 bits for green leds plus 8 for the level (only 4 used) plus
 * 16 bits for red leds plus 16 bits for blue leds: 16 * 3 + 8 = 56
 * So we can calculate the offsets for each part:
 * red offset   = 16 + 8      = 24
 * green offset = 0           = 0
 * blue offset  = 16 + 8 + 16 = 40
 * level offset = 16          = 16
 *
 * The new board is wired differently. New offsets: (We need to stream blue, green, red layer)
 * blue  = 0
 * green = 16
 * red   = 32
 * layer = 48
 *
 * The code identifies one colour in one unique led:
 * bit 1 to 4 of the code select a led on a layer
 * bit 5 & 6 selects the layer
 * bit 7 & 8 select the colour
 *
 * code : physical location of the led, layer and colour
 */
void turn_led_on( uint8_t code )
{
	/* we run a counter from 0 to 113. Each counter tick represents half of a clock pulse
	 * We have two clocks, the shift clock and the storage clock.
	 * We pulse the shift clock by setting the pin every even tick and resetting every odd tick.
	 * The shift clock gets pulsed 56 times, the last counter period (112 and 113) is used for the storage clock
	 * The data is clocked in on the positive clock edge.
	 * We set the data on an even count and generate a positive clock edge on the odd count. We then set the new
	 * data bit on the following count (which is even again)
	 * Thus the counter value is twice the bit position
	 *
	 * There are three lines we have to set:
	 *  the data line (serial out),
	 *  the shift clock line, and
	 *  the storage clock line
	 */

	/* check if the code is valid */
	if ((code & 0xC0) == 0xC0) /* colour "4" is requested */
		return;

	// translate from logical position to counter index
	uint8_t phy_code = map_table[code];

	/* we need to calculate which layer to turn on, and the colour and position of the led */
	uint8_t layer = (phy_code >> 4) & 0x03;
	uint8_t colour = (phy_code >> 6) & 0x03;
	uint8_t position = (phy_code) & 0x0F;

	static uint8_t offset[4] = { 32, 16, 0, 48 };

	/* from the colour and position we need to calculate a counter value */
	uint8_t position_match = (offset[colour] + position) << 1;
	uint8_t layer_match = (offset[3] + layer) << 1;

	for (uint8_t counter = 0; counter < 114; ++counter) {

		/* set the data line (serial out) on the even counts */
		if( !(counter % 2) ) {
			if ((counter == layer_match) || (counter == position_match))
				gpio_set( serial_out_pin.port, serial_out_pin.pin );
			else
				gpio_clear( serial_out_pin.port, serial_out_pin.pin );
		}

		/* set the clock lines */
		if( counter == 112 ) {
			gpio_set( store_clock_pin.port, store_clock_pin.pin );
		} else if( counter == 113 ) {
			gpio_clear( store_clock_pin.port, store_clock_pin.pin );
		} else { /* only set the shift clock when the storage clock is not pulsing */
			if( counter % 2 ) // generate a rising edge on odd counts
                gpio_set( shift_clock_pin.port, shift_clock_pin.pin );
			else
				gpio_clear( shift_clock_pin.port, shift_clock_pin.pin );
		}

	}
}

/*
 * We want to be able to blank the cube, specifically when we load new
 * data.
 */
void blank_cube( void )
{
    gpio_clear( serial_out_pin.port, serial_out_pin.pin );

	for (uint8_t counter = 0; counter < 114; ++counter) {

		/* set the clock lines */
		if (counter == 112) {
			gpio_set( store_clock_pin.port, store_clock_pin.pin );
		} else if (counter == 113) {
			gpio_clear( store_clock_pin.port, store_clock_pin.pin );
		} else { /* only set the shift clock when the storage clock is not pulsing */
			if (counter % 2) // generate a rising edge on odd counts
				gpio_set( shift_clock_pin.port, shift_clock_pin.pin );
			else
				gpio_clear( shift_clock_pin.port, shift_clock_pin.pin );
		}

	}
}

