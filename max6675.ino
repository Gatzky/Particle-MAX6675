#include "MAX6675.h"

/* PINS for MAx6675, three-pin SPI */
uint8_t tc_CLK = 6;                                                                     // PIN for CLK or SCK
uint8_t tc_SO = 4;                                                                      // PIN for SO or DO
uint8_t tc_CS = 5;                                                                      // PIN for CS

MAX6675 thermocouple(tc_CLK, tc_CS, tc_SO);                                             // Initialisation of MAX6675
/* *** */

/* Timer for periodic refresh of temperature */
int Thermocouple_timer_period = 5000;
void Thermocouple_timer_callback(void);                                                 // Definition of callback function 
Timer thermocouple_timer = Timer(5000, Thermocouple_timer_callback, true);              // Initialisation of thermocouple timer
/* *** */

/* Other global variables */
double tempC=0;                                                                         // Variable for contain temperature in Celsius
/* *** */
  
void setup() {
    Particle.variable("Celcius", tempC);                                                // Particle function to take tempC in output

    delay(500);                                                                         // Wait for MAX chip to stabilize
    thermocouple_timer.start();                                                         // Start thermocouple timer
}

void loop() {
    //no action
}

void Thermocouple_timer_callback(void){
    tempC = thermocouple.read_temp_celc();
    thermocouple_timer.reset();
}