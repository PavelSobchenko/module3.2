#include "adc.h"
#include "logger.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define LDR_ADC_CHAN ADC_CHANNEL_3
#define POT_ADC_CHAN ADC_CHANNEL_4
#define ADC_ATTEN ADC_ATTEN_DB_12

typedef enum { ADC_LDR, ADC_POT, ADC_COUNT } AdcId;


AdcDriver drivers[ADC_COUNT] = {
    [ADC_LDR] = {
        .atten = ADC_ATTEN,
        .unit = ADC_UNIT_1,
        .bitwidth = ADC_BITWIDTH_DEFAULT,
        .channel = LDR_ADC_CHAN,
        .name = "LDR"
    },
    [ADC_POT] = {
        .atten = ADC_ATTEN,
        .unit = ADC_UNIT_1,
        .bitwidth = ADC_BITWIDTH_DEFAULT,
        .channel = POT_ADC_CHAN,
        .name = "POT"
    }
};
Logger logger = { .tag = "MODULE 3.2" };

float convertMvToV(int mv) {
    return mv / 1000.0f;
}

void logDriver(AdcDriver *driver) {
    logInfo(&logger,
        "%s: average: %d, volts: %.2f, volts cali: %.2f, diff: %d mV (%.1f%%)",
        driver->name,
        driver->average,
        convertMvToV(driver->mv),
        convertMvToV(driver->mvCali),
        driver->diffMv,
        driver->diffPct
    );
}

void app_main() {
    adcInit(&drivers[ADC_LDR]);
    adcInit(&drivers[ADC_POT]);

    while(1) {
        adcRead(&drivers[ADC_LDR]);
        adcRead(&drivers[ADC_POT]);

        for(size_t i = 0; i < ADC_COUNT; i++) {
            logDriver(&drivers[i]);
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}