#pragma once

#include <stdint.h>
#include <stddef.h>
#include "esp_err.h"
#include "soc/soc_caps.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"

#define SMA_SAMPLE_COUNT 10

typedef struct AdcDriver AdcDriver;

struct AdcDriver {
    adc_oneshot_unit_handle_t handle;
    adc_unit_t unit;
    adc_atten_t atten;
    adc_bitwidth_t bitwidth;
    adc_channel_t channel;

    bool calibrated;
    adc_cali_handle_t handleCali;

    const char *name;

    size_t writeIndex;
    size_t count;
    uint16_t samples[SMA_SAMPLE_COUNT];
    uint16_t average;
    uint32_t sum;

    int mv;
    int mvCali;
    int diffMv;
    float diffPct;
};

void adcInit(AdcDriver *adc);
void adcRead(AdcDriver *adc);
