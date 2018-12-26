#ifndef VERSION_H
#define	VERSION_H

/******************************************************************************/
/* For use with USB-MSD bootloader                                            */
/******************************************************************************/
#define FIRMWARE_VERSION            0x002428// Firmware version is located here
                                            // 3 bytes: Major Ver, Minor Ver, FW Valid
#define MAJOR_FW_VAL                '0'     // Firmware Version 0.5  (ASCII)
#define	MINOR_FW_VAL                '5'
#define	FW_VALID                    0xAA    // Flag to indicate valid firmware
/******************************************************************************/

#endif	/* VERSION_H */

