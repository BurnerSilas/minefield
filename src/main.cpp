#include <Arduino.h>
#include "driver/i2s.h"
#include "Distortion.h"
#include "SerialControl.h"

//----PINS----
#define I2S_MCK_IO (4) // master clock [MCLK]
#define I2S_BCK_IO (5) // bit clock [BCK]
#define I2S_WS_IO (6)  // word select [LRCK]
#define I2S_DO_IO (7)  // data out [DATA]
#define I2S_DI_IO (8)  // data in [DATA]
#define I2S_NUM (I2S_NUM_0)

void configureSetup();

Distortion distortion(Distortion::Mode::FUZZ, /*drive*/ 8.0f, /*level*/ 0.6f);
SerialControl serialControl(distortion);

void setup()
{
    Serial.begin(115200);

    delay(3000);

    Serial.println("Serial ready!");

    configureSetup();

    Serial.println("Setup done!");

    Serial.println("Starting Audio Pass-Through...");
}

void loop()
{

    serialControl.update();

    int32_t rx_buffer[64];
    size_t bytes_read, bytes_written;

    // read audio from ADC
    i2s_read(I2S_NUM, rx_buffer, sizeof(rx_buffer), &bytes_read, portMAX_DELAY);

    if (bytes_read > 0)
    {
        size_t sampleCount = bytes_read / sizeof(int32_t);

        // distortion.processBuffer(rx_buffer, sampleCount);
        //  write audio to DAC
        i2s_write(I2S_NUM, rx_buffer, bytes_read, &bytes_written, portMAX_DELAY);
    }
}

void configureSetup()
{
    //-------------I2S-----------------
    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_RX), // Senden & Empfangen
        .sample_rate = 48000,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
        .communication_format = I2S_COMM_FORMAT_STAND_I2S,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 8,
        .dma_buf_len = 64, // Kleine Buffer für geringe Latenz
        .use_apll = true};
    i2s_driver_install(I2S_NUM, &i2s_config, 0, NULL);

    //-------------PINS------------------
    i2s_pin_config_t pin_config = {
        .mck_io_num = I2S_MCK_IO, // I2S_MCK_IO,I2S_PIN_NO_CHANGE
        .bck_io_num = I2S_BCK_IO,
        .ws_io_num = I2S_WS_IO,
        .data_out_num = I2S_DO_IO,
        .data_in_num = I2S_DI_IO};

    i2s_set_pin(I2S_NUM, &pin_config);

    Serial.println("Setup I2S done!");
}