#include "SerialControl.h"

static const char *TAG = "SERIAL";

void SerialControl::init()
{
    usb_serial_jtag_driver_config_t config = {
        .tx_buffer_size = SERIAL_BUF_SIZE,
        .rx_buffer_size = SERIAL_BUF_SIZE,
    };
    usb_serial_jtag_driver_install(&config);
    ESP_LOGI(TAG, "USB Serial JTAG ready");
    respond("Effect Pedal ready. Commands: DIST, LIM, LPF\n");
}

void SerialControl::update()
{
    uint8_t byte;
    while (usb_serial_jtag_read_bytes(&byte, 1, 0) > 0)
    {
        // Echo zurückschicken
        usb_serial_jtag_write_bytes(&byte, 1, 0);

        if (byte == '\n' || byte == '\r')
        {
            if (m_bufLen > 0)
            {
                m_buf[m_bufLen] = '\0';
                parseCommand(m_buf);
                m_bufLen = 0;
            }
        }
        else if (m_bufLen < SERIAL_BUF_SIZE - 1)
        {
            m_buf[m_bufLen++] = (char)byte;
        }
    }
}

void SerialControl::parseCommand(const char *cmd)
{
    if (strncmp(cmd, "DIST:", 5) == 0)
        handleDistortion(cmd + 5);
    else if (strncmp(cmd, "LIM:", 4) == 0)
        handleLimiter(cmd + 4);
    else if (strncmp(cmd, "LPF:", 4) == 0)
        handleLowPass(cmd + 4);
    else
        respond("ERR: unknown prefix. Use DIST:, LIM:, LPF:\n");
    if (strncmp(cmd, "DIST:BYPASS:", 12) == 0)
        handleBypass(m_distortion, cmd + 12);
    if (strncmp(cmd, "LIM:BYPASS:", 11) == 0)
        handleBypass(m_limiter, cmd + 11);
    if (strncmp(cmd, "LPF:BYPASS:", 11) == 0)
        handleBypass(m_lowPass, cmd + 11);
}

// -------------------------------------------------------
// Neuen Effekt hinzufügen:
// 1. Handler Methode hier implementieren
// 2. In parseCommand eine neue Zeile mit dem Prefix eintragen
// 3. Referenz im Konstruktor ergänzen
// -------------------------------------------------------

void SerialControl::handleDistortion(const char *sub)
{
    if (strncmp(sub, "GAIN:", 5) == 0)
    {
        float val = atof(sub + 5);
        m_distortion.setGain(val);
        ESP_LOGI(TAG, "OK: DIST:GAIN=%.2f", val);
    }
    else if (strncmp(sub, "MIX:", 4) == 0)
    {
        float val = atof(sub + 4);
        m_distortion.setMix(val);
        ESP_LOGI(TAG, "OK: DIST:MIX=%.2f", val);
    }
    else if (strncmp(sub, "BASSBLEND:", 10) == 0)
    {
        float val = atof(sub + 10);
        m_distortion.setBassBlend(val);
        ESP_LOGI(TAG, "OK: DIST:BASSBLEND=%.2f", val);
    }
    else
    {
        respond("ERR: unknown DIST command. Use GAIN:, MIX:, BASSBLEND:, BYPASS:\n");
    }
}

void SerialControl::handleLimiter(const char *sub)
{
    if (strncmp(sub, "THRESH:", 7) == 0)
    {
        float val = atof(sub + 7);
        m_limiter.setLimit(val);
        ESP_LOGI(TAG, "OK: LIM:THRESH=%.2f", val);
    }
    else if (strncmp(sub, "TYPE:", 5) == 0)
    {
        const char *type = sub + 5;
        if (strcmp(type, "HARD") == 0)
            m_limiter.setType(HARD);
        else if (strcmp(type, "SOFT") == 0)
            m_limiter.setType(SOFT);
        else
            respond("ERR: unknown type. Use HARD or SOFT\n");
        ESP_LOGI(TAG, "OK: LIM:TYPE=%s", type);
    }
    else
    {
        respond("ERR: unknown LIM command. Use THRESH:, TYPE:, BYPASS:\n");
    }
}

void SerialControl::handleLowPass(const char *sub)
{
    if (strncmp(sub, "CUTOFF:", 7) == 0)
    {
        float val = atof(sub + 7);
        m_lowPass.setCutoff(val);
        ESP_LOGI(TAG, "OK: LPF:CUTOFF=%.2f", val);
    }
    else
    {
        respond("ERR: unknown LPF command. Use CUTOFF:, BYPASS:\n");
    }
}

void SerialControl::respond(const char *msg)
{
    usb_serial_jtag_write_bytes(msg, strlen(msg), 10);
}