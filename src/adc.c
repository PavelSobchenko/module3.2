#include "adc.h"

static adc_oneshot_unit_handle_t s_unit_handles[SOC_ADC_PERIPH_NUM] = { 0 };

static void adcCalibrationInit(AdcDriver *adc) {
    esp_err_t ret = ESP_FAIL;

    adc_cali_curve_fitting_config_t cali_config = {
        .unit_id = adc->unit,
        .chan = adc->channel,
        .atten = adc->atten,
        .bitwidth = adc->bitwidth
    };
    ret = adc_cali_create_scheme_curve_fitting(&cali_config, &adc->handleCali);
    if (ret == ESP_OK) {
        adc->calibrated = true;
    }
}

void adcInit(AdcDriver *adc) {
    if (s_unit_handles[adc->unit] == NULL) {
        adc_oneshot_unit_init_cfg_t init_config = {
            .unit_id = adc->unit,
        };
        ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config, &s_unit_handles[adc->unit]));
    }
    adc->handle = s_unit_handles[adc->unit];

    adc_oneshot_chan_cfg_t config = {
        .atten = adc->atten,
        .bitwidth = adc->bitwidth,
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc->handle, adc->channel, &config));

    adcCalibrationInit(adc);
}

void adcRead(AdcDriver *adc) {
    int raw = 0;
    ESP_ERROR_CHECK(adc_oneshot_read(adc->handle, adc->channel, &raw));

    if (adc->count == SMA_SAMPLE_COUNT) {
        adc->sum -= adc->samples[adc->writeIndex];
    } else {
        adc->count++;
    }

    adc->samples[adc->writeIndex] = raw;
    adc->sum += raw;
    adc->writeIndex = (adc->writeIndex + 1) % SMA_SAMPLE_COUNT;
    adc->average = (uint16_t)(adc->sum/adc->count);

    adc->mv = raw * 3100 / 4095;

    if (adc->calibrated) {
        ESP_ERROR_CHECK(adc_cali_raw_to_voltage(adc->handleCali, raw, &adc->mvCali));

        adc->diffMv = adc->mvCali - adc->mv;
        adc->diffPct = adc->diffMv * 100.0f / adc->mvCali;
    }
}