#include "application.h"
#include "avr/pgmspace.h"
#include "stdlib.h"
#include "MAX6675.h"

#define BYTE_LENGTH 8
#define CELSIUS_DIVIDER 32

MAX6675::MAX6675(int8_t SCLK, int8_t CS, int8_t SO) {
    sclk = SCLK;
    cs = CS;
    so = SO;

    //define pin modes
    pinMode(cs, OUTPUT);
    pinMode(sclk, OUTPUT); 
    pinMode(so, INPUT);

    digitalWrite(cs, HIGH);
}

uint16_t MAX6675::read_temp_raw(void) {
    uint16_t retVal = 0;

    digitalWrite(cs, LOW);
    delayMicroseconds(1);

    retVal = spi_read();
    retVal <<= BYTE_LENGTH;
    retVal |= spi_read();

    digitalWrite(cs, HIGH);

    return retVal;
}

double MAX6675::read_temp_celc(void) {
    double retVal = (double)read_temp_raw();
    retVal /= CELSIUS_DIVIDER;
    
    return retVal;
}
    

uint8_t MAX6675::spi_read(void) { 
    int i;
    uint8_t retVal = 0;

    for (i=(BYTE_LENGTH-1); i>=0; i--)
    {
        digitalWrite(sclk, LOW);
        delayMicroseconds(1);
    
        if (digitalRead(so)) {
            retVal |= (1 << i);
        }

        digitalWrite(sclk, HIGH);
        delayMicroseconds(1);
    }

  return retVal;
}
