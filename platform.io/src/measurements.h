/*
 * Rad Pro
 * Measurements
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#if !defined(MEASUREMENTS_H)
#define MEASUREMENTS_H

#include <stdbool.h>
#include <stdint.h>

#include "settings.h"
#include "view.h"

extern const View instantaneousRateView;
extern const View averageRateView;
extern const View doseView;
extern const View historyView;

extern const View unitsMenuView;
extern const View rateAlarmMenuView;
extern const View doseAlarmMenuView;
extern const View averageTimerMenuView;

void initMeasurements(void);

void updateTube(void);

void onMeasurementTick(uint32_t pulseCount);
void onMeasurementPeriod(void);
void onMeasurementsOneSecond(void);

void setMeasurementView(int32_t index);

float getInstantaneousRate(void);

void setDoseTime(uint32_t value);
uint32_t getDoseTime(void);
void setDosePulseCount(uint32_t value);
uint32_t getDosePulseCount(void);

void setTubeTime(uint32_t value);
uint32_t getTubeTime(void);
void setTubePulseCount(uint32_t value);
uint32_t getTubePulseCount(void);

#endif
