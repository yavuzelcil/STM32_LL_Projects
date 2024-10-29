/*----------------------------------------------------------------------------
  Funktionen für Temperatursensor	MCP 9808
	
	06/2020
 *----------------------------------------------------------------------------*/

#include "I2C.h"
#include "Temperatur.h"



int TempRead() {
	// liest Temperatur aus und gibt sie in 1/16 Grad zurück
		uint8_t registerAddress = TEMP_TR;
    uint8_t temp_data[2];
    I2CWrite(&registerAddress, 1, TEMP_ADR);  // Set register pointer to temperature register
    I2CRead(temp_data, 2, TEMP_ADR);  // Read two bytes from temperature register
    
    uint8_t UpperByte = temp_data[0];
    uint8_t LowerByte = temp_data[1];
    
    UpperByte = UpperByte & 0x1F;  // Clear flag bits
    
    int Temperature;
    if ((UpperByte & 0x10) == 0x10){  // TA < 0°C
        UpperByte = UpperByte & 0x0F;  // Clear SIGN
        Temperature = 256 - (UpperByte * 16 + LowerByte / 16);
    } else {  // TA = 0°C
        Temperature = (UpperByte * 16 + LowerByte / 16);
    }
    
    return Temperature;
}



void TempSetRes(int resolution) {
    uint8_t res = resolution & 0x03;  // Ensure resolution is within valid range (0 to 3)
    I2CWrite(&res, 1, TEMP_ADR);  // Set register pointer to resolution register
}



