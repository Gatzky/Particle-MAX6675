#ifndef MAX6675_h
#define MAX6675_h

class MAX6675 {
    public:
        MAX6675(uint8_t SCLK, uint8_t CS, uint8_t SO);
        double read_temp_celc(void);
        
    private:
        uint8_t sclk, so, cs;
        uint8_t spi_read(void);
        uint16_t read_temp_raw(void);
};

#endif