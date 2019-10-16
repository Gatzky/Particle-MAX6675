#ifndef Expected_temperature_h
#define Expected_temperature_h

class Expected_temperature{
    public:
        Expected_temperature(double Range);                                         // Constructor
        void Particle_begin(void);                                                  // Function sets 'Set_expected_temperature' as particle.subscribtion callback
        void Set_expected_temperature(const char *event, const char *data);         // Function sets expected temperature and min_exp_temp and max_exp_temp, it is call when photon get event "Expected" 
        void Check_heater_flag(double actual_temperature, bool* heater_flag);       // Function compare actual and expected temperature and acording with it sets heater flag
        double Exp_temp;                                                            // Variable to contain expected tempereture sets in 'Set_expected_temperature'
    private:
        bool isSet_exp_temp;                                                        // Flag, FALSE - if Exp_temp, Min_exp_temp and Max_exp_temp is not set, TRUE - if they are set
        double Min_exp_temp;                                                        // Exp_temp - Range, if actual temperature fall to this value heater should be set to on
        double Max_exp_temp;                                                        // Exp_temp + Range, if actual temperature rise to this value heater should be set to off
        double Range_hysteresis_exp_temp;                                           // Variable that describe range of histeresis
};

#endif