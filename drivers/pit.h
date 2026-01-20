#include <stdint.h>
#include <stdbool.h>

typedef struct {
    bool out_pin;
    bool null_count;
    uint8_t mode;
} pit_status_t;

typedef enum{
    PIT_MODE_M0 = 0x00,      // interrupt on terminal count
    PIT_MODE_M1 = 0x02,      // hardware re-triggerable one-shot
    PIT_MODE_M2 = 0x04,      // rate generator
    PIT_MODE_M3 = 0x06,      // square wave generator
    PIT_MODE_M4 = 0x08,      // software triggered strobe
    PIT_MODE_M5 = 0x0A       // hardware triggered strobe
} pit_mode;

bool pit_set_count(unsigned int channel, uint16_t count, pit_mode mode);
bool pit_set_frequency(uint8_t channel, uint32_t hz, pit_mode mode);
pit_status_t pit_readback_status(uint8_t channel);