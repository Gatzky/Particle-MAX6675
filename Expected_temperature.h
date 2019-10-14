#ifndef Expected_temperature_h
#define Expected_temperature_h

class Expected_temperature{
    public:
        Expected_temperature(uint8_t Heater_PIN);
        double expected_temperature;
        void Set_temperature(const char *event, const char *data);
        void Check_heater_flag(double actual_temperature);
        void Particle_begin(void);
        bool heater_flag;
    private:
        uint8_t heater_pin;
        //bool heater_flag;
        double min_expected_temperature;
        double max_expected_temperature;
        void Set_heater_pin(void);
};

#endif