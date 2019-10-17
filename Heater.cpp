/*******************************************************************************
 ****  Imported Project Header Files
 ******************************************************************************/
#include "application.h"
#include "Heater.h"

/*******************************************************************************
 * \function      : Heater(uint8_t Heater_PIN)
 * \description   : Constructor
 *******************************************************************************
 * \param[in]     : Heater_PIN - GPIO pin for heater relay
 * \param[out]    : <none>
 * \param[in,out] : <none>
 * \return        : <none>
 ******************************************************************************/
Heater::Heater(uint8_t Heater_PIN){
    Heater_pin = Heater_PIN;
    pinMode(Heater_pin, OUTPUT);                        /* Set Heater_pin to output (forever) */
    digitalWrite(Heater_pin, LOW);                      /* Set Heater_pin to low (for start), in start heater is set to off to reduce energy cost */
    is_heater_active = FALSE;                           /* Set heater flag to false, in start heater is set to off to reduce energy cost */
} // End of Heater(uint8_t Heater_PIN)

/*******************************************************************************
 * \function      : void Set_heater_pin(void)
 * \description   : Function sets heater PIN according with heater flag
 *******************************************************************************
 * \param[in]     : <none>
 * \param[out]    : <none>
 * \param[in,out] : <none>
 * \return        : <none>
 ******************************************************************************/
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
} // End of void Set_heater_pin(void)