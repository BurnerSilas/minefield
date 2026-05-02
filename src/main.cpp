#include <Arduino.h>
#include "driver/i2s.h"

//----PINS----
#define I2S_BCK_IO (27) // bit clock [BCK]
#define I2S_WS_IO (26)  // word select [LRCK]
#define I2S_DO_IO (25)  // data out [DATA]
#define I2S_DI_IO (34)  // data in [DATA]
#define I2S_MCK_IO (1)  // master clock [MCLK]
#define I2S_NUM (I2S_NUM_0)

void configureSetup();

void setup()
{
    Serial.begin(115200);

    configureSetup();

    Serial.println("I2S Pass-Through started...");
}

void loop()
{
    int16_t rx_buffer[128];
    size_t bytes_read, bytes_written;

    // read audio from ADC
    i2s_read(I2S_NUM, rx_buffer, sizeof(rx_buffer), &bytes_read, portMAX_DELAY);

    if (bytes_read > 0)
    {
        // write audio to DAC
        i2s_write(I2S_NUM, rx_buffer, bytes_read, &bytes_written, portMAX_DELAY);
    }
}

void configureSetup()
{
    //-------------I2S-----------------
    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_RX), // Senden & Empfangen
        .sample_rate = 48000,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,      // Stereo
        .communication_format = I2S_COMM_FORMAT_STAND_I2S, 
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 8,
        .dma_buf_len = 64, // Kleine Buffer für geringe Latenz
        .use_apll = false};
    i2s_driver_install(I2S_NUM, &i2s_config, 0, NULL);

    //-------------PINS------------------
    i2s_pin_config_t pin_config = {
        .mck_io_num = GPIO_NUM_1,
        .bck_io_num = I2S_BCK_IO,
        .ws_io_num = I2S_WS_IO,
        .data_out_num = I2S_DO_IO,
        .data_in_num = I2S_DI_IO};

    i2s_set_pin(I2S_NUM, &pin_config);
}