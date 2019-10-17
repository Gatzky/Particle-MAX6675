#ifndef Heater_h
#define Heater_h

/*******************************************************************************
 ****  Class Definitions
 ******************************************************************************/
class Heater{
    public:
        Heater(uint8_t Heater_PIN);                 /* Conctructor */
        void Set_heater_pin(void);                  /* Seting heater pin in according with is_heater_active flag */
        bool is_heater_active;                      /* Flag, FALSE - heater shouldnt work, TRUE - heater, should work */
    private:
        uint8_t Heater_pin;                         /* PIN where relay heater is connect */
}; // End of Heater

#endif