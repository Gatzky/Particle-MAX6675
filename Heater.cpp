#include "application.h"
#include "Heater.h"

Heater::Heater(uint8_t Heater_PIN){
    Heater_pin = Heater_PIN;
    pinMode(Heater_pin, OUTPUT);                        // Set Heater_pin to output (for always)
    digitalWrite(Heater_pin, LOW);                      // Set Heater_pin to low (for start), in start heater is set to off to reduce energy cost
    is_heater_active = FALSE;                           // Set heater flag to false, in start heater is set to off to reduce energy cost
}

void Heater::Set_heater_pin(void){ 
    if(is_heater_active == TRUE){
        digitalWrite(Heater_pin, HIGH);
    }
    else if(is_heater_active == FALSE){
        digitalWrite(Heater_pin, LOW);
    }
    else{
        //no action
    }
}