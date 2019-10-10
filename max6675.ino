#include "MAX6675.h"

int thermoSO = 4;
int thermoCS = 5;
int thermoCLK = 6;

MAX6675 thermocouple(thermoCLK, thermoCS, thermoSO);
double tempC;
  
void setup() {
  Particle.variable("Celcius", tempC);

  // wait for MAX chip to stabilize
  delay(500);
}

void loop() {
  // basic readout test, just print the current temp
  
  tempC = thermocouple.read_temp_celc();
 
   delay(1000);
}
