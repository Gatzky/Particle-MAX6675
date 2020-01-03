/*******************************************************************************
 ****  Imported Project Header Files
 ******************************************************************************/
#include "Heater.h"
#include "Expected_temperature.h"
#include "MAX6675.h"
#include <string>

/*******************************************************************************
 ****  Macro Definitions
 ******************************************************************************/
#define HEATER_PIN 7                                                                    /* PIN for relay to control heater */
/* PINS for MAx6675, three-pin SPI */
#define TS_SO 4                                                                         /* PIN for spi clock (CLK or SCK) */
#define TS_CS 5                                                                         /* PIN for data line (SO or DO) */
#define TS_CLK 6                                                                        /* PIN for select device (CS) */

/*******************************************************************************
 ****  Variable Declarations
 ******************************************************************************/
bool isThermoCallback = FALSE; 
bool isSet_meas_temp = FALSE;                                                           /* Flag, FALSE if 'Meas_temp_C' was never set */
int tc_timer_period = 5000;                                                             /* Value that define period between interrupt call. In ms. */
double Range_Hysteresis = 1;                                                            /* Value that describe range of histeresis. */
double Meas_temp_C = 0;                                                                 /* Variable to contain measured temperature */

/*******************************************************************************
 ****  Class Object Declarations
 ******************************************************************************/
MAX6675 thermocouple(TS_CLK, TS_CS, TS_SO);                                             /* Class for SPI communication and calculate read values */
Heater heater(HEATER_PIN);                                                              /* Class for control heater flag and heater PIN */
Expected_temperature c_Exp_temp(Range_Hysteresis);                                      /* Set and check expected temperature */
Timer tc_timer = Timer(tc_timer_period, Thermocouple_timer_callback, true);             /* Timer for periodic refresh of temperature */

/*******************************************************************************
 ****  Function Declarations
 ******************************************************************************/
void tc_timer_callback(void);                                                           /* Definition of timer callback function */

/*******************************************************************************
 * \function      : void setup ( void )
 * \description   : Setup values and functions in start of code
 *******************************************************************************
 * \param[in]     : <none>
 * \param[out]    : <none>
 * \param[in,out] : <none>
 * \return        : <none>
 ******************************************************************************/
void setup(void) {
    c_Exp_temp.Particle_begin();                                                        /* Special function for declare particle function subscribe */
    Particle.variable("Actual_Var", Meas_temp_C);                                       /* Set expected temperature as particle variable */
    Particle.variable("Expected_Var", c_Exp_temp.Exp_temp);                             /* Set expected temperature as particle variable */
    Particle.variable("Heater_flag", heater.is_heater_active);                          /* Set heater active flag as particle variable */
    Particle.variable("Disconnect", thermocouple.is_disconnect); 

    delay(500);                                                                         /* wait for MAX chip to stabilize */
    tc_timer.start();                                                                   /* start thermocouple timer, start interrupt for measure temperature */
} //End of void setup (void)

/*******************************************************************************
 * \function      : void loop ( void )
 * \description   : Infinite loop
 *******************************************************************************
 * \param[in]     : <none>
 * \param[out]    : <none>
 * \param[in,out] : <none>
 * \return        : <none>
 ******************************************************************************/
void loop() {
    if(isThermoCallback == TRUE){
        char buf[100];
        char flag = 0;
        if (heater.is_heater_active){
            flag = 1;
        }
        snprintf(buf, sizeof(buf), "{\"Meas_temp_C\":%.3f,\"flag\":%d}", Meas_temp_C, flag);
        Particle.publish("sendData", buf, PRIVATE);
        Particle.publish("readData", "", PRIVATE);
        isThermoCallback = FALSE;
        tc_timer.reset();                                                                   /* Reset timer, interrupt will occurs after next period */
    }
} //End of void loop (void)

/*******************************************************************************
 * \function      : void Thermocouple_timer_callback ( void )
 * \description   : Callback of timer interrupt. In this function measure temperature
 * and compare it with expected value occurs.
 *******************************************************************************
 * \param[in]     : <none>
 * \param[out]    : <none>
 * \param[in,out] : <none>
 * \return        : <none>
 ******************************************************************************/
void Thermocouple_timer_callback(void){
    Meas_temp_C = thermocouple.read_temp_celc();                                        /* Measure temperature */
    isSet_meas_temp = TRUE;                                                             /* Set flag of measure temperature to true */
    c_Exp_temp.Check_heater_flag(Meas_temp_C, &heater.is_heater_active);                /* Compare measure temperature to expected and set heater flag and pin  in accordance with comparation */
    heater.Set_heater_pin();
    isThermoCallback = TRUE;
    //atc_timer.reset();                                                                   /* Reset timer, interrupt will occurs after next period */
}
