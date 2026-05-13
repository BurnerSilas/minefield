
- take advantage of "true parallelism" 

write parallel tasks for ESP32 using FreeRTOS and the Arduino Development Framework (ADF) 

> I wanna separate the audio processing form all the other tasks since it has the highest priority for a device that is meant to be used as a real Effect Pedal 

stopt working: 
- init order of I2S and multitasking
- reducing stupid Stack size (for now)
- adding a while(true)-loop for audio processing
- disabled interface and effect for now
