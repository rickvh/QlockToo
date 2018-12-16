#include "eeprom.h"
#include <xc.h>
#include <stdint.h>        /* For uint8_t definition */
#include <plib/EEP.h>

//void write_byte(unsigned char addr, unsigned char data);
//unsigned char read_byte(unsigned char addr);
//void int_EEPROM_putc(unsigned char address, unsigned char data);
//unsigned char int_EEPROM_getc(unsigned char address);


void loadSettings(struct QLOCKTOO_SETTINGS *settingsptr)
{
    settingsptr->brightness = 5;

//    settingsptr->brightness = read_byte(0x00);
//    settingsptr->brightness = int_EEPROM_getc(0x12);

//    settingsptr->brightness = Read_b_eep(0x00);
//    settingsptr->brightnessMode = Read_b_eep(0x01);
//    settingsptr->cornerMode = Read_b_eep(0x02);
//    settingsptr->energysaveMode = Read_b_eep(0x03);
}

/**
 * WARNING: User must take care to disable interrupts before calling!!
 * @param settingsptr settings
 */
void saveSettings(struct QLOCKTOO_SETTINGS *settingsptr)
{
    int_EEPROM_putc(0x12, settingsptr->brightness);




//    write_byte(0x00, settingsptr->brightness);
//    Write_b_eep(0x00, settingsptr->brightness);
//    Write_b_eep(0x01, settingsptr->brightnessMode);
//    Write_b_eep(0x02, settingsptr->cornerMode);
//    Write_b_eep(0x03, settingsptr->energysaveMode);
}

/*
void write_byte(unsigned char addr, unsigned char data)
{
    EEADR = addr;
    EEDATA = data;

    EEPGD = 0;  // Point to DATA memory
    CFGS = 0;   // Access EEPROM
    WREN = 1;   // Enable writes

    // write!
    EECON2 = 0x55;
    EECON2 = 0xAA;
    WR = 1;

    WREN = 0;   // Disable writes
}

unsigned char read_byte(unsigned char addr)
{
    int_EEPROM_putc
    EEADR = addr;
    EEPGD = 0;  // Point to DATA memory
    CFGS = 0;   // Access EEPROM
    RD = 1;     // EEPROM Read
    return EEDATA;
}*/

//This function Writes data to given address in internal EEPROM of PIC MCU

void int_EEPROM_putc(unsigned char address, unsigned char data)
{
    unsigned char INTCON_SAVE;

    EEADR  = address;
    EEDATA = data;

    EECON1bits.EEPGD= 0; // 0 = Access data EEPROM memory
    EECON1bits.CFGS = 0; // 0 = Access Flash program or DATA EEPROM memory
    EECON1bits.WREN = 1; // enable writes to internal EEPROM

    INTCON_SAVE=INTCON; // Save INTCON register contants
    INTCON=0;             // Disable interrupts, Next two lines SHOULD run without interrupts

    EECON2=0x55;        // Required sequence for write to internal EEPROM
    EECON2=0xaa;        // Required sequence for write to internal EEPROM

    EECON1bits.WR=1;    // begin write to internal EEPROM
    INTCON=INTCON_SAVE; //Now we can safely enable interrupts if previously used

    Nop();

    while (PIR2bits.EEIF==0)//Wait till write operation complete
    {
        Nop();
    }

    EECON1bits.WREN=0; // Disable writes to EEPROM on write complete (EEIF flag on set PIR2 )
    PIR2bits.EEIF=0; //Clear EEPROM write complete flag. (must be cleared in software. So we do it here)

}

// This function reads data from address given in internal EEPROM of PIC
unsigned char int_EEPROM_getc(unsigned char address)
{
    EEADR=address;
    EECON1bits.EEPGD= 0; // 0 = Access data EEPROM memory
    EECON1bits.CFGS = 0; // 0 = Access Flash program or DATA EEPROM memory
    EECON1bits.RD   = 1; // EEPROM Read
    return EEDATA;       // return data
}