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

#ifndef LEDCUBE_H
#define LEDCUBE_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	uint32_t port;
	uint16_t pin;
} pin_t;

typedef struct
{
	pin_t reset;
	pin_t shift_clock;
	pin_t store_clock;
	pin_t serial_out;
} ledcube_pins_t;

void ledcube_init( ledcube_pins_t * pins );
void turn_led_on( uint8_t code );

void blank_cube( void );

#ifdef __cplusplus
}
#endif

#endif /* LEDCUBE_H */
