#ifndef I2C_SLAVE_H
#define	I2C_SLAVE_H

#define I2C_SLAVE_ADDR              0x16    // 7-bit address: 0x0B == 8-bit adress: 0x16
                                            // Valid 7-bit addressrange as per doc: 0x08 to 0x77
void handleI2CISR();

#endif	/* I2C_SLAVE_H */

