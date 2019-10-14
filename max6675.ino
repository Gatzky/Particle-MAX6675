#include "Expected_temperature.h"
#include "MAX6675.h"
#include <string>

/* PINS for MAx6675, three-pin SPI */
uint8_t tc_CLK = 6;                                                                     // PIN for CLK or SCK
uint8_t tc_SO = 4;                                                                      // PIN for SO or DO
uint8_t tc_CS = 5;                                                                      // PIN for CS

MAX6675 thermocouple(tc_CLK, tc_CS, tc_SO);                                             // Initialisation of MAX6675
/* *** */

/* Subscribed event for set expected temperature */
uint8_t Heater_PIN = 0;
Expected_temperature Expected(Heater_PIN);
/* *** */

/* Timer for periodic refresh of temperature */
int Thermocouple_timer_period = 5000;
void Thermocouple_timer_callback(void);                                                                 // Definition of callback function 
Timer thermocouple_timer = Timer(5000, Thermocouple_timer_callback, true);         // Initialisation of thermocouple timer
/* *** */

/* Variable for contain temperature in Celsius */
double tempC=0;
/* *** */

  
void setup() {
    Expected.Particle_begin();
    Particle.variable("Celcius", tempC);
    Particle.variable("Expected_Var", Expected.expected_temperature);
    Particle.variable("Heater_flag", Expected.heater_flag);

    // wait for MAX chip to stabilize
    delay(500);
    thermocouple_timer.start();
}

void loop() {
    //no action
}

void Thermocouple_timer_callback(void){
    tempC = thermocouple.read_temp_celc();
    thermocouple_timer.reset();
    Expected.Check_heater_flag(tempC);
}