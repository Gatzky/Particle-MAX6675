#include "application.h"
#include "avr/pgmspace.h"
#include "stdlib.h"
#include "MAX6675.h"

#define BYTE_LENGTH 8
#define CELSIUS_DIVIDER 32
#define DISCONNECTED_CHECK 0xFF00
#define DICONNECTED_TEMP 0xFF00

MAX6675::MAX6675(uint8_t SCLK, uint8_t CS, uint8_t SO) {
    sclk = SCLK;
    cs = CS;
    so = SO;

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
    
    if((retVal > DISCONNECTED_CHECK) || (retVal == 0)){
        retVal = DICONNECTED_TEMP;
        //Particle.publish("Error", "MAX6675 not connected", PUBLIC);
    }

    return retVal;
}

double MAX6675::read_temp_celc(void) {
    double retVal = (double)read_temp_raw();
    if (retVal == DICONNECTED_TEMP){
        //Particle.publish("Error", "MAX6675 not connected", PUBLIC);
        return retVal;
    }
    
    retVal /= CELSIUS_DIVIDER;
    //Particle.publish("Temperature read", String(retVal));
    
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