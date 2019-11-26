/*
	Copyright 2016 - 2017 Benjamin Vedder	benjamin@vedder.se

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

#include "adconv.h"
#include "utils.h"
#include "conf_general.h"
#include "terminal.h"
#include "commands.h"

// Settings
#define VREFINT					1.21

#define ADC_GRP_NUM_CHANNELS	8
#define ADC_GRP_BUF_DEPTH		1

static adcsample_t samples[ADC_GRP_NUM_CHANNELS * ADC_GRP_BUF_DEPTH];
static float vin_filter = 0.0;

static void terminal_cmd_get_vin(int argc, const char **argv);

static void adccallback(ADCDriver *adcp, adcsample_t *buffer, size_t n) {
	(void)adcp;
	(void)buffer;
	(void)n;

	const float v_reg = (VREFINT * 4095.0) / (float)samples[6];
	float sample = (samples[0] / 4095.0 * v_reg) * ((VIN_R1 + VIN_R2) / VIN_R2);
	UTILS_LP_FAST(vin_filter, sample, 0.02);
}

//static void adcerrorcallback(ADCDriver *adcp, adcerror_t err) {
//	(void)adcp;
//	(void)err;
//}

/*
 * ADC conversion group.
 * Mode:        Continuous, 16 samples of 8 channels, SW triggered.
 */
static const ADCConversionGroup adcgrpcfg = {
		TRUE,
		ADC_GRP_NUM_CHANNELS,
		adccallback,
		0,//adcerrorcallback,
		0,                        /* CR1 */
		ADC_CR2_SWSTART,          /* CR2 */
		ADC_SMPR1_SMP_AN10(ADC_SAMPLE_56) |
		ADC_SMPR1_SMP_AN11(ADC_SAMPLE_56) |
		ADC_SMPR1_SMP_AN12(ADC_SAMPLE_56) |
		ADC_SMPR1_SMP_AN13(ADC_SAMPLE_56) |
		ADC_SMPR1_SMP_SENSOR(ADC_SAMPLE_144) |
		ADC_SMPR1_SMP_VREF(ADC_SAMPLE_144),
		ADC_SMPR2_SMP_AN4(ADC_SAMPLE_56), /* SMPR2 */
		ADC_SQR1_NUM_CH(ADC_GRP_NUM_CHANNELS),
		ADC_SQR2_SQ8_N(ADC_CHANNEL_SENSOR) | ADC_SQR2_SQ7_N(ADC_CHANNEL_VREFINT),
		ADC_SQR3_SQ6_N(ADC_CHANNEL_IN11)  | ADC_SQR3_SQ5_N(ADC_CHANNEL_IN10) |
		ADC_SQR3_SQ4_N(ADC_CHANNEL_IN13) | ADC_SQR3_SQ3_N(ADC_CHANNEL_IN12) |
		ADC_SQR3_SQ2_N(ADC_CHANNEL_IN11) | ADC_SQR3_SQ1_N(ADC_CHANNEL_IN10)
};

void adconv_init(void) {
	palSetPadMode(GPIOC, 0, PAL_MODE_INPUT_ANALOG);
	palSetPadMode(GPIOC, 1, PAL_MODE_INPUT_ANALOG);
	palSetPadMode(GPIOC, 2, PAL_MODE_INPUT_ANALOG);
	palSetPadMode(GPIOC, 3, PAL_MODE_INPUT_ANALOG);

	adcStart(&ADCD1, NULL);
	adcSTM32EnableTSVREFE();

	adcStartConversion(&ADCD1, &adcgrpcfg, samples, ADC_GRP_BUF_DEPTH);

	terminal_register_command_callback(
			"adconv_get_vin",
			"Read the input voltage.",
			0,
			terminal_cmd_get_vin);
}

/**
 * Get the raw ADC value from one of the pins on the ADC_GPIO header.
 */
uint16_t adconv_get_pin(int pin) {
	if (pin < 0 || pin >= 4) {
		return 0;
	}

	return samples[pin];
}

/**
 * Read the voltage on ADC input pin.
 *
 * @param pin
 * The pin to read from.
 *
 * @return
 * The voltage in volts.
 */
float adconv_get_volts(int pin) {
	const float v_reg = (VREFINT * 4095.0) / (float)samples[6];
	return (float)adconv_get_pin(pin) / 4095.0 * v_reg;
}

/**
 * Get the filtered input voltage.
 *
 * @return
 * The input voltage to the PCB in volts.
 */
float adconv_get_vin(void) {
	return vin_filter;
}

static void terminal_cmd_get_vin(int argc, const char **argv) {
	(void)argc;
	(void)argv;
	commands_printf("Input Voltage: %.2f V\n", (double)adconv_get_vin());
}
