#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h" // Jetzt fehlerfrei auflösbar
#include "driver/i2s_std.h"  // Neuer nativer I2S-Standard-Treiber
#include "esp_log.h"         // Für professionelles Logging statt Serial.print
#include "Distortion.h"
#include "Limiter.h"
#include "LowPassFilter.h"
#include "SerialControl.h"
#include "AutoWah.h"
#include "BitCrusher.h"

i2s_chan_handle_t tx_handle = NULL;
i2s_chan_handle_t rx_handle = NULL;

//----PINS----
#define I2S_MCK_IO GPIO_NUM_4 // master clock [MCLK]
#define I2S_BCK_IO GPIO_NUM_5 // bit clock [BCK]
#define I2S_WS_IO GPIO_NUM_6  // word select [LRCK]
#define I2S_DO_IO GPIO_NUM_7  // data out [DATA]
#define I2S_DI_IO GPIO_NUM_8  // data in [DATA]
#define I2S_NUM I2S_NUM_0

Distortion *distortion = nullptr;
Limiter *limiter = nullptr;
LowPassFilter *lowPass = nullptr;
SerialControl *serialControl = nullptr;
AutoWah *autoWah = nullptr;
BitCrusher *bitCrusher = nullptr;

void configureI2S();
void configureMultitasking();
void processAudio(void *pvParameters);

extern "C" void app_main()
{

    ESP_LOGI("MAIN", "Serial ready!");

    vTaskDelay(pdMS_TO_TICKS(3000));

    distortion = new Distortion();
    limiter = new Limiter(0.5f, SOFT);
    lowPass = new LowPassFilter(8000.0f, 48000.0f);
    autoWah = new AutoWah();
    serialControl = new SerialControl(*distortion, *limiter, *lowPass, *autoWah);

    bitCrusher = new BitCrusher();
    serialControl->init();

    configureI2S();

    configureMultitasking();

    ESP_LOGI("MAIN", "Setup done!");

    ESP_LOGI("MAIN", "Starting Audio Pass-Through...");

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(20));
    }
}

void processAudio(void *pvParameters)
{
    while (rx_handle == NULL || tx_handle == NULL)
    {
        vTaskDelay(pdMS_TO_TICKS(10));
    }

    int32_t audio_buffer[128];
    size_t bytes_read, bytes_written;

    ESP_LOGI("AUDIO", "Start Audio Processing Loop.");

    while (1)
    {
        // read audio from ADC
        i2s_channel_read(rx_handle, audio_buffer, sizeof(audio_buffer), &bytes_read, portMAX_DELAY);

        // ESP_LOGI("AUDIO", "bytes_read: %d", bytes_read);

        if (bytes_read > 0)
        {
            size_t sampleCount = bytes_read / sizeof(int32_t);

            for (size_t i = 0; i < sampleCount; i += 2)
            {
                float audioSample = (float)audio_buffer[i] / 2147483648.0f;

                audioSample = lowPass->process(audioSample);
                // audioSample = distortion->process(audioSample);
                audioSample = bitCrusher->process(audioSample);
                audioSample = limiter->process(audioSample);
                // audioSample = autoWah->process(audioSample);

                int32_t rawOutput = (int32_t)(audioSample * 2147483647.0f);

                int32_t cleanOutput = rawOutput & 0xFFFFFF00;

                audio_buffer[i] = cleanOutput;     // Links im Kopfhörer
                audio_buffer[i + 1] = cleanOutput; // Rechts im Kopfhörer
            }

            // write audio to DAC
            i2s_channel_write(tx_handle, audio_buffer, bytes_read, &bytes_written, portMAX_DELAY);
        }
    }
}

void processSerial(void *pvParameters)
{
    while (1)
    {
        serialControl->update();
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

/*multitasking Setup*/
void configureMultitasking()
{
    ESP_LOGI("MAIN", "Setting up Core 0 for audio processing.");

    xTaskCreatePinnedToCore(
        processAudio,   // Function
        "processAudio", // Name
        8048,           // Stack
        NULL,           // Parameters
        5,              // Priority
        NULL,           // Task handle
        0               // Core
    );

    ESP_LOGI("MAIN", "Setting up Core 1 for Serial processing.");

    xTaskCreatePinnedToCore(
        processSerial,
        "processSerial",
        4096,
        NULL,
        1,
        NULL,
        1);
}

/*I2S Setup*/
void configureI2S()
{
    i2s_chan_config_t chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM, I2S_ROLE_MASTER);
    chan_cfg.dma_desc_num = 8;
    chan_cfg.dma_frame_num = 64;

    ESP_ERROR_CHECK(i2s_new_channel(&chan_cfg, &tx_handle, &rx_handle));

    i2s_std_config_t std_cfg = {
        .clk_cfg = I2S_STD_CLK_DEFAULT_CONFIG(48000),
        .slot_cfg = I2S_STD_PHILIPS_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_32BIT, I2S_SLOT_MODE_STEREO),
        .gpio_cfg = {
            .mclk = I2S_MCK_IO,
            .bclk = I2S_BCK_IO,
            .ws = I2S_WS_IO,
            .dout = I2S_DO_IO,
            .din = I2S_DI_IO,
            .invert_flags = {
                .mclk_inv = false,
                .bclk_inv = false,
                .ws_inv = false,
            }}};

    std_cfg.slot_cfg.slot_bit_width = I2S_SLOT_BIT_WIDTH_32BIT;

    std_cfg.clk_cfg.clk_src = I2S_CLK_SRC_PLL_240M;

    ESP_ERROR_CHECK(i2s_channel_init_std_mode(tx_handle, &std_cfg));
    ESP_ERROR_CHECK(i2s_channel_init_std_mode(rx_handle, &std_cfg));

    ESP_ERROR_CHECK(i2s_channel_enable(tx_handle));
    ESP_ERROR_CHECK(i2s_channel_enable(rx_handle));

    ESP_LOGI("MAIN", "I2S Full-Duplex Setup done!");
}