#ifndef MAX6675_h
#define MAX6675_h

/*******************************************************************************
 ****  Class Definitions
 ******************************************************************************/
class MAX6675 {
    public:
        MAX6675(uint8_t SCLK, uint8_t CS, uint8_t SO);          /* Constructor */
        double read_temp_celc(void);                            /* Read temperature in celsius */
        bool is_disconnect;                                     /* TRUE - device is disconnect, FALSE - device is connect */
    private:
        uint8_t sclk, so, cs;                                   /* SPI pins */
        uint8_t spi_read(void);                                 /* Read 1 byte from SPI */
        uint16_t read_temp_raw(void);                           /* Read temperature in raw data */
}; // End of MAX6675 class

#endif