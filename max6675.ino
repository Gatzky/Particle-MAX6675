#include "Heater.h"
#include "Expected_temperature.h"
#include "MAX6675.h"
#include <string>

/* PINS for MAx6675, three-pin SPI */
uint8_t tc_CLK = 6;                                                                     // PIN for spi clock (CLK or SCK)
uint8_t tc_SO = 4;                                                                      // PIN for data line (SO or DO)
uint8_t tc_CS = 5;                                                                      // PIN for select device (CS)

MAX6675 thermocouple(tc_CLK, tc_CS, tc_SO);                                             // Initialisation of MAX6675
/* *** */

/* Set and check expected temperature */
double Range_Hysteresis = 1;                                                            // Value that describe range of histeresis. 
                                                                                        //If expected temperature is 20 and range is 1 its mean that heater will set on if temp fall to 19, and set to off if temp rise to 21
Expected_temperature Exp_temp(Range_Hysteresis);                                        // Initialisation of Expected_temperature class
/* *** */

/* Class for control heater flag and heater PIN */
uint8_t Heater_PIN = 0;                                                                 // PIN for relay to control heater

Heater heater(Heater_PIN);
/* *** */

/* Variable for contain temperature in Celsius */
double Meas_temp_C = 0;                                                                 // Variable to contain measured temperature
bool isSet_meas_temp = FALSE;                                                           // Flag, FALSE if measured temperature was never set
/* *** */

/* Timer for periodic refresh of temperature */
int tc_timer_period = 5000;                                                             // Value that define period between interrupt call. In ms.
void tc_timer_callback(void);                                                           // Definition of callback function 

Timer tc_timer = Timer(5000, Thermocouple_timer_callback, true);                        // Initialisation of thermocouple timer
/* *** */

  
void setup() {
    Exp_temp.Particle_begin();                                                          // Special function for declare particle function subscribe
    Particle.variable("Celcius", Meas_temp_C);                                          // Set measured temperature as particle variable
    Particle.variable("Expected_Var", Exp_temp.Exp_temp);                               // Set expected temperature as particle variable
    Particle.variable("Heater_flag", heater.is_heater_active);                          // Set heater active flag as particle variable

    delay(500);                                                                         // wait for MAX chip to stabilize
    tc_timer.start();                                                                   // start thermocouple timer, start interrupt for measure temperature
}

void loop() {
}

void Thermocouple_timer_callback(void){
    Meas_temp_C = thermocouple.read_temp_celc();                                        // Measure temperature
    isSet_meas_temp = TRUE;                                                             // Set flag of measure temperature to true
    Exp_temp.Check_heater_flag(Meas_temp_C, &heater.is_heater_active);                  // Compare measure temperature to expected and set heater flag and pin  in accordance with comparation
    heater.Set_heater_pin();
    tc_timer.reset();                                                                   // Reset timer, interrupt will occurs after next period

}