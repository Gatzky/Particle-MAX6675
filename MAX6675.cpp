#include "application.h"
#include "avr/pgmspace.h"
#include "stdlib.h"
#include "MAX6675.h"

#define BYTE_LENGTH 8
#define CELSIUS_DIVIDER 32
#define DISCONNECTED_CHECK 0xFF00
#define DICONNECTED_TEMP 0xFF00

MAX6675::MAX6675(uint8_t SCLK, uint8_t CS, uint8_t SO) {
    sclk = SCLK;                                                //  \
    cs = CS;                                                    //   Set PINS variables
    so = SO;                                                    //  /

    pinMode(cs, OUTPUT);                                        //  \
    pinMode(sclk, OUTPUT);                                      //   Set PINS to directions
    pinMode(so, INPUT);                                         //  /

    digitalWrite(cs, HIGH);                                     // Set device select to HIGH (default dont select device)
}

uint16_t MAX6675::read_temp_raw(void) {
    uint16_t retVal = 0;

    digitalWrite(cs, LOW);                                      // Set deivce select to LOW - start communicaton
    delayMicroseconds(1);                                       // Wait for resposne from device

    retVal = spi_read();                                        // Read 1 byte from device
    retVal <<= BYTE_LENGTH;                                     // Shift 1 byte to 9-16 bytes
    retVal |= spi_read();                                       // Read 2 byte from device

    digitalWrite(cs, HIGH);                                     // Set deivce select to HIGH - stop communicaton
    
    if((retVal > DISCONNECTED_CHECK) || (retVal == 0)){         // Check that retVal has reasonable values
        is_disconnet = TRUE;                                    // If not sets disconnect flag to TRUE
    }

    return retVal;                                              // Return
}

double MAX6675::read_temp_celc(void) {
    double retVal = (double)read_temp_raw();                    // Read temperature in raw format
    if (is_disconnet == TRUE){                                  // If device is not connect
        return DICONNECTED_TEMP;                                // Stop function
    }
    
    retVal /= CELSIUS_DIVIDER;                                  // Divide value to get temperature in Celsius
    
    return retVal;                                              // Return
}
    

uint8_t MAX6675::spi_read(void) { 
    int i;
    uint8_t retVal = 0;

    for (i=(BYTE_LENGTH-1); i>=0; i--)                          // From 7 byte to 0 byte
    {
        digitalWrite(sclk, LOW);                                // Set CLK to Low
        delayMicroseconds(1);                                   // Wait for device response
    
        if (digitalRead(so)) {                                  // Read bit on data line
            retVal |= (1 << i);                                 // Write bit to retVal
        }

        digitalWrite(sclk, HIGH);                               // Set CLK to HIGH
        delayMicroseconds(1);                                   // Wait for device response
    }

  return retVal;                                                // Return
}