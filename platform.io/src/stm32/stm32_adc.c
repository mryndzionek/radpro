/*
 * Rad Pro
 * STM32 ADC
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#if defined(STM32)

#include <libopencm3/stm32/adc.h>
#include <libopencm3/stm32/rcc.h>

#if defined(STM32G0)

#define RCC_ADC1 RCC_ADC

#endif

#if defined(GD32)

#include "gd32f1x0_adc.h"

#endif

#include "../adc.h"
#include "../buzzer.h"
#include "../display.h"
#include "../events.h"
#include "../system.h"
#include "../tube.h"

#include "stm32.h"

// ADC constants

#define ADC_TIME_1_5 0x0
#define ADC_TIME_7_5 0x1
#define ADC_TIME_13_5 0x2
#define ADC_TIME_28_5 0x3
#define ADC_TIME_41_5 0x4
#define ADC_TIME_55_5 0x5
#define ADC_TIME_71_5 0x6
#define ADC_TIME_239_5 0x7

#define ADC_VDD 3.3F
#define ADC_VALUE_MAX ((1 << 12) - 1)

#define ADC_VALUE_TO_VOLTAGE (ADC_VDD / ADC_VALUE_MAX)

#define ADC_CALIBRATION_FACTOR_UPDATE_TIME 60

// Battery voltage

// First order filter (n: time constant in taps): k = exp(-1 / n)
// For n = 100 (seconds):

#define BATTERY_FILTER_CONSTANT 0.99F

// Device temperature

// First order filter (N: time constant in taps): k = exp(-1 / n)
// For n = 30 (seconds):

#define DEVICE_TEMPERATURE_FILTER_CONSTANT 0.967216F

static struct
{
    float filteredBatteryVoltage;
    float filteredDeviceTemperature;
} adc;

void initADC(void)
{
    rcc_periph_clock_enable(RCC_ADC1);

#if defined(STM32F0) && defined(GD32)

    // rcc_osc_on(RCC_HSI14);
    RCC_CR2 |= RCC_CR2_HSI14ON;
    // rcc_wait_for_osc_ready(RCC_HSI14);
    while (!(RCC_CR2 & RCC_CR2_HSI14RDY))
        ;

    adc_channel_length_config(ADC_REGULAR_CHANNEL, 1);
    adc_external_trigger_source_config(ADC_REGULAR_CHANNEL, ADC_EXTTRIG_REGULAR_NONE);
    adc_external_trigger_config(ADC_REGULAR_CHANNEL, ENABLE);

    adc_enable();
    sleep(2);

    adc_calibration_enable();

#elif defined(STM32F0)

    adc_calibrate(ADC1);

#elif defined(STM32F1)

    adc_enable_external_trigger_regular(ADC1, ADC_CR2_EXTSEL_SWSTART);

    adc_power_on(ADC1);

    sleep(2);

    adc_calibrate(ADC1);

#elif defined(STM32G0)

    adc_enable_regulator(ADC1);

    sleep(2);

    adc_calibrate(ADC1);

#endif

    resetADCFilters();
}

uint32_t readADC(uint8_t channel, uint8_t sampleTime)
{
    uint32_t value;

    // ADC configuration

#if defined(STM32F0) && defined(GD32)

    adc_regular_channel_config(0, channel, sampleTime);

#elif defined(STM32F0) || defined(STM32G0)

    // adc_set_regular_sequence(ADC1, 1, &channel);
    ADC_CHSELR(ADC1) = (1 << channel);

    adc_set_sample_time_on_all_channels(ADC1, sampleTime);

#elif defined(STM32F1)

    // adc_set_regular_sequence(ADC1, 1, &channel);
    // ADC_SQR1(ADC1) = (1 - 1) << ADC_SQR1_L_SHIFT;
	// ADC_SQR2(ADC1) = 0;
	ADC_SQR3(ADC1) = channel;

    // adc_set_sample_time_on_all_channels(ADC1, sampleTime);
    value = 0;
    for (uint32_t i = 0; i < 10; i++)
		value |= (value << 3) | sampleTime;
	ADC_SMPR1(ADC1) = value;
	ADC_SMPR2(ADC1) = value;

#endif

    // ADC conversion

#if defined(STM32F0) || defined(STM32G0)
    adc_power_on(ADC1);
#endif

    syncBuzzer();
    syncDisplayBacklight();
    syncTubeHV();

#if defined(STM32F0) && defined(GD32)

    adc_software_trigger_enable(ADC_REGULAR_CHANNEL);

    while (!adc_flag_get(ADC_FLAG_EOC))
        ;

    adc_flag_clear(ADC_FLAG_EOC);

    value = adc_regular_data_read();

#elif defined(STM32F0) || defined(STM32G0)

    adc_start_conversion_regular(ADC1);

    while (!adc_eoc(ADC1))
        ;

    value = adc_read_regular(ADC1);

    adc_power_off(ADC1);

#elif defined(STM32F1)

    adc_start_conversion_regular(ADC1);

    while (!adc_eoc(ADC1))
        ;

    // Clear EOC (for GD32F103)
    ADC_SR(ADC1) &= ~ADC_SR_EOC;

    value = adc_read_regular(ADC1);

#endif

    return value;
}

uint32_t readBatteryValue(void)
{
    return readADC(PWR_BAT_CHANNEL, ADC_TIME_7_5);
}

uint32_t readDeviceTemperatureValue(void)
{
#if defined(STM32F0) && defined(GD32)
    ADC_CTL1 |= ADC_CTL1_TSVREN;
#else
    adc_enable_temperature_sensor();
#endif

    uint32_t value = readADC(ADC_CHANNEL_TEMP, ADC_TIME_239_5);

#if defined(STM32F0) && defined(GD32)
    ADC_CTL1 &= ~ADC_CTL1_TSVREN;
#else
    adc_disable_temperature_sensor();
#endif

    return value;
}

static float readBatteryVoltage(void)
{
    return (ADC_VALUE_TO_VOLTAGE * PWR_BAT_SCALE_FACTOR) *
           readBatteryValue();
}

static float readDeviceTemperature(void)
{
    uint32_t value = readDeviceTemperatureValue();

#if defined(STM32F0) && defined(GD32)

    return 25.0F + (1.45F - value * ADC_VALUE_TO_VOLTAGE) / 0.0041F;

#elif defined(STM32F0)

    float valueLow = ST_TSENSE_CAL1_30C;
    float valueHigh = ST_TSENSE_CAL2_110C;
    float valueSlope = (valueHigh - valueLow) / (110.0F - 30.0F);

    return 30.0F + (value - valueLow) / valueSlope;

#elif defined(STM32F1) && defined(GD32)

    return 25.0F + (1.45F - value * ADC_VALUE_TO_VOLTAGE) / 0.0041F;

#elif defined(STM32F1) && defined(CH32)

    return 25.0F + (1.34F - value * ADC_VALUE_TO_VOLTAGE) / 0.0043F;

#elif defined(STM32F1)

    return 25.0F + (1.43F - value * ADC_VALUE_TO_VOLTAGE) / 0.0043F;

#elif defined(STM32G0)

    float valueLow = ST_TSENSE_CAL1_30C;

    return 30.0F + (value - valueLow) * ADC_VALUE_TO_VOLTAGE / 0.0025F;

#endif
}

void updateADC(void)
{
    float value;

    value = readBatteryVoltage();
    adc.filteredBatteryVoltage =
        value + BATTERY_FILTER_CONSTANT *
                    (adc.filteredBatteryVoltage - value);

    value = readDeviceTemperature();
    adc.filteredDeviceTemperature =
        value + DEVICE_TEMPERATURE_FILTER_CONSTANT *
                    (adc.filteredDeviceTemperature - value);
}

void resetADCFilters(void)
{
    adc.filteredBatteryVoltage = readBatteryVoltage();
    adc.filteredDeviceTemperature = readDeviceTemperature();
}

float getBatteryVoltage(void)
{
    return adc.filteredBatteryVoltage;
}

float getDeviceTemperature(void)
{
    return adc.filteredDeviceTemperature;
}

#endif
