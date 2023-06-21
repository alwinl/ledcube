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

#include <libopencm3/cm3/systick.h>

static volatile uint64_t tickcount = 0;

void sys_tick_setup(void)
{
#define tick_frequency (1000)		/* 1ms period equals 1 kHz */
#define AHB_frequency (72000000)	/* system running at 72 Mhz */

	systick_set_frequency( tick_frequency, AHB_frequency );
	systick_counter_enable();
	systick_interrupt_enable();
}

void sys_tick_handler( void )
{
    ++tickcount;
}

uint32_t sys_tick_get_tickcount()
{
    return tickcount;
}

void sys_tick_delay( uint32_t time_in_ms )
{
    uint64_t end = tickcount + time_in_ms;

    while( tickcount < end )
        ;
}

