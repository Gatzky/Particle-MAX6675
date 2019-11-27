/*******************************************************************************
 ****  Imported Project Header Files
 ******************************************************************************/
#include "application.h"
#include "Expected_temperature.h"

/*******************************************************************************
 ****  Macro Definitions
 ******************************************************************************/
#define NOT_SET 0

/*******************************************************************************
 * \function      : Expected_temperature (double Range)
 * \description   : Constructor
 *******************************************************************************
 * \param[in]     : Range - default hysteresis range of expected temperature
 * \param[out]    : <none>
 * \param[in,out] : <none>
 * \return        : <none>
 ******************************************************************************/
Expected_temperature::Expected_temperature(double Range){
    isSet_exp_temp = FALSE;
    Exp_temp = NOT_SET;
    Min_exp_temp = NOT_SET;
    Max_exp_temp = NOT_SET;
    Range_hysteresis_exp_temp = Range;
} // End of Expected_temperature (double Range)

/*******************************************************************************
 * \function      : void Particle_begin (void)
 * \description   : Function call Particle function because it is impossible in constructor
 *******************************************************************************
 * \param[in]     : <none>
 * \param[out]    : <none>
 * \param[in,out] : <none>
 * \return        : <none>
 ******************************************************************************/
void Expected_temperature::Particle_begin(void){
    Particle.subscribe("hook-response/readData/0", &Expected_temperature::Set_expected_temperature, this, MY_DEVICES);     /* Set 'Set_expected_temperature' as callback of Particle.subscribe() */
} // End of void Particle_begin (void)

/*******************************************************************************
 * \function      : void Set_expected_temperature (const char *event, const char *data)
 * \description   : Callback of interrupt occurs when event "Expected" occurs
 *******************************************************************************
 * \param[in]     : event - pointer to event name
 *                : data - pointer to data of event
 * \param[out]    : <none>
 * \param[in,out] : <none>
 * \return        : <none>
 ******************************************************************************/
void Expected_temperature::Set_expected_temperature(const char *topic, const char *data){
    Exp_temp = 40;
	//StaticJsonBuffer<100> jsonBuffer;
	//char *mutableCopy = strdup(data);
	//JsonObject& root = jsonBuffer.parseObject(mutableCopy);
	//free(mutableCopy);
	//Exp_temp = atof(root["Expected temperature:"]);*/	                                            /* Overwrite 'Exp_temp' by data from event. 'atof' change string to double */

    Min_exp_temp = Exp_temp - Range_hysteresis_exp_temp;                                            /* Calculate Min_exp_temp */
    Max_exp_temp = Exp_temp + Range_hysteresis_exp_temp;                                            /* Calculate Max_exp_temp */
    isSet_exp_temp = TRUE; 
} // End of void Set_expected_temperature (const char *event, const char *data)

/*******************************************************************************
 * \function      : void Check_heater_flag(double actual_temperature, bool* is_heater_active)
 * \description   : Function compare measuered temperature and expected temperature 
 *                  and check heater flag and acroding with it sets heater flag
 *******************************************************************************
 * \param[in]     : actual_temperature - measured temperature
 * \param[out]    : <none>
 * \param[in,out] : is_heater_active - pointer to heater flag
 * \return        : <none>
 ******************************************************************************/
void Expected_temperature::Check_heater_flag(double actual_temperature, bool* is_heater_active){
    if(isSet_exp_temp == FALSE){                                                                            /* If expected values are not sets */
        //report error
        return;                                                                                             /* Stop checking */
    }
    
    if((*is_heater_active == true) && (actual_temperature > Max_exp_temp)){                                 /* If heater is on and tempereture rise above max  */
        *is_heater_active = false;                                                                          /* Sets heater to off */
    }
    else if((*is_heater_active == false) && (actual_temperature < Min_exp_temp)){                           /* If heater is off and temperature rise below min */
        *is_heater_active = true;                                                                           /* Sets heater to on */
        
    }
    else{
        //no action                                                                                         /* If something else - do nothing */
    }
} // End of void Check_heater_flag(double actual_temperature, bool* is_heater_active