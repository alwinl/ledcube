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

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#include "tick.h"
#include "ledcube.h"
#include "generator.h"

ledcube_pins_t ledcube_pins =
{
	.reset = { GPIOB, GPIO12 },
	.shift_clock = { GPIOB, GPIO13 },
	.store_clock = { GPIOB, GPIO14 },
	.serial_out = { GPIOB, GPIO15 }
};

/* Set STM32 to 72 MHz. */
static void clock_setup( void )
{
	/* Use a High Speed External 8Mhz crystal and run at 72Mhz */
	rcc_clock_setup_pll( &rcc_hse_configs[RCC_CLOCK_HSE8_72MHZ] );

	rcc_periph_clock_enable( RCC_AFIO );	/* Enable AFIO clock. */
	rcc_periph_clock_enable( RCC_GPIOB );	/* Enable GPIOB clock. */
	rcc_periph_clock_enable( RCC_GPIOC );	/* Enable GPIOC clock. */
}

static void gpio_setup( void )
{
	/* Ensure we can use SWD after we have flashed this binary */
	gpio_primary_remap( AFIO_MAPR_SWJ_CFG_JTAG_OFF_SW_ON, 0 );

	/* Set GPIO13 (in GPIO port C) to 'output push-pull'. */
	gpio_set_mode( GPIOC, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO13 );
	gpio_set_mode( GPIOB, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO12 | GPIO13 | GPIO14 | GPIO15 );

	/* Preconfigure the LED. */
	gpio_clear( GPIOC, GPIO13 );	/* Switch on LED. */
	gpio_clear( GPIOB, GPIO12 | GPIO13 | GPIO14 | GPIO15 );
}

int main( void )
{
	clock_setup();
	gpio_setup();
	sys_tick_setup();
    ledcube_init( &ledcube_pins );

	/* Blink the LED (PC13) on the board. */
	while( 1 ) {

        run_cube( );
		gpio_toggle( GPIOC, GPIO13 );	/* LED on/off */

	}

	return 0;
}
