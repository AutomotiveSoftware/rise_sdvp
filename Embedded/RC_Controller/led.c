/*
	Copyright 2013-2016 Benjamin Vedder	benjamin@vedder.se

	This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "led.h"
#include "ch.h"
#include "hal.h"
#include "conf_general.h"

void led_init(void) {
	palSetPadMode(LED_RED_GPIO, LED_RED_PIN,
			PAL_MODE_OUTPUT_PUSHPULL |
			PAL_STM32_OSPEED_HIGHEST);
	palSetPadMode(LED_GREEN_GPIO, LED_GREEN_PIN,
			PAL_MODE_OUTPUT_PUSHPULL |
			PAL_STM32_OSPEED_HIGHEST);

	led_write(LED_RED, 0);
	led_write(LED_GREEN, 0);
}

void led_write(int num, int state) {
	switch (num) {
	case LED_RED:
		palWritePad(LED_RED_GPIO, LED_RED_PIN, state);
		break;

	case LED_GREEN:
		palWritePad(LED_GREEN_GPIO, LED_GREEN_PIN, state);
		break;

	default:
		break;
	}
}

void led_toggle(int num) {
	switch (num) {
	case LED_RED:
		palTogglePad(LED_RED_GPIO, LED_RED_PIN);
		break;

	case LED_GREEN:
		palTogglePad(LED_GREEN_GPIO, LED_GREEN_PIN);
		break;

	default:
		break;
	}
}
