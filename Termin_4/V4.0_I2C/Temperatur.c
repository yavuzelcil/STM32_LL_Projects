/*----------------------------------------------------------------------------
  Funktionen f�r Temperatursensor	MCP 9808
	
	06/2020
 *----------------------------------------------------------------------------*/

#include "I2C.h"

#define TEMP_ADR    0x18  // Slave Adresse Temperaturf�hler
#define TEMP_RES    0x08  // Resolution Register
#define TEMP_TR     0x05  // Temperature Register

int TempRead()
// liest Temperatur aus und gibt sie in 1/16 Grad zur�ck
{

	// ...

}

void TempSetRes(int resolution)
// Aufl�sung des Temperatursensors setzen (Modi 0...3 erlaubt)
{ 

	// ...

}
