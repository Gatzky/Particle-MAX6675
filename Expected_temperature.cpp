#include "application.h"
#include "Expected_temperature.h"
#define NOT_SET 0

Expected_temperature::Expected_temperature(double Range){
    isSet_exp_temp = FALSE;
    Exp_temp = NOT_SET;
    Min_exp_temp = NOT_SET;
    Max_exp_temp = NOT_SET;
    Range_hysteresis_exp_temp = Range;
}

void Expected_temperature::Particle_begin(void){
    Particle.subscribe("Expected", &Expected_temperature::Set_expected_temperature, this, ALL_DEVICES);    // Set 'Set_expected_temperature' as callback of Particle.subscribe()
}

void Expected_temperature::Set_expected_temperature(const char *event, const char *data){
    Exp_temp = atof(data);                                                                                  // Overwrite 'Exp_temp' by data from event. 'atof' change string to double
    Min_exp_temp = Exp_temp - Range_hysteresis_exp_temp;                                                    // Calculate Min_exp_temp
    Max_exp_temp = Exp_temp + Range_hysteresis_exp_temp;                                                    // Calculate Max_exp_temp
    isSet_exp_temp = TRUE; 
}

void Expected_temperature::Check_heater_flag(double actual_temperature, bool* is_heater_active){
    if(isSet_exp_temp == FALSE){                                                                            // If expected values are not sets
        //report error
        return;                                                                                             // Stop checking
    }
    
    if((*is_heater_active == true) && (actual_temperature > Max_exp_temp)){                                 // If heater is on and tempereture rise above max 
        *is_heater_active = false;                                                                          // Sets heater to off
    }
    else if((*is_heater_active == false) && (actual_temperature < Min_exp_temp)){                           // If heater is off and temperature rise below min
        *is_heater_active = true;                                                                           // Sets heater to on
        
    }
    else{
        //no action                                                                                         // If something else - do nothing
    }
}