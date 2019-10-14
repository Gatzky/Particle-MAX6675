#include "application.h"
#include "avr/pgmspace.h"
#include "stdlib.h"
#include "Expected_temperature.h"
#define NOT_CHECKED 0
#define DICONNECTED_TEMP 0xFF00

Expected_temperature::Expected_temperature(uint8_t Heater_PIN){
    heater_pin = Heater_PIN;
    pinMode(heater_pin, OUTPUT);
    digitalWrite(heater_pin, LOW);
    heater_flag = false;
    expected_temperature = NOT_CHECKED;
    min_expected_temperature = NOT_CHECKED;
    max_expected_temperature = NOT_CHECKED;
}

void Expected_temperature::Particle_begin(void){
    Particle.subscribe("Expected", &Expected_temperature::Set_temperature, this, ALL_DEVICES);    
}

void Expected_temperature::Set_temperature(const char *event, const char *data){
    expected_temperature = atof(data);
    min_expected_temperature = expected_temperature - 1;
    max_expected_temperature = expected_temperature + 1;
}

void Expected_temperature::Check_heater_flag(double actual_temperature){
    /*if ((expected_temperature == NOT_CHECKED) || (min_expected_temperature == NOT_CHECKED) || (max_expected_temperature == NOT_CHECKED)){
        //report error
        return;
    }
    else if (actual_temperature == DICONNECTED_TEMP){
        //report error
        return;        
    }*/
    
    if((heater_flag == true) && (actual_temperature > max_expected_temperature)){
        heater_flag = false;
    }
    else if((heater_flag == false) && (actual_temperature < min_expected_temperature)){
        heater_flag = true;
    }
    else{
        //no action
    }
    
    Set_heater_pin();
}

void Expected_temperature::Set_heater_pin(void){
    if(heater_flag == true){
        digitalWrite(heater_pin, HIGH);
    }
    else if(heater_flag == false){
        digitalWrite(heater_pin, LOW);
    }
    else{
        //no action
    }
}