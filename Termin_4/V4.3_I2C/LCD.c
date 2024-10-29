/*----------------------------------------------------------------------------
  Funktionen für LC-Display
	
	06/2020
 *----------------------------------------------------------------------------*/
#include "I2C.h"
#include "main.h"
#include "LCD.h" //#define LCD_ADR	0x27		// I2C-Adresse des LCDisplays

#define RS 0x01					// Bit für Daten (gesetzt für Daten, gelöcht für Kommando)
#define RW 0x02					// Read/Write (gesetzt für Read)
#define E 0x04					// Enabe (fallende Flanke übernimmt Daten/Kommandos)
#define BL 0x08					// Hintergrundbeleuchtung an/aus

void LCDSendCmd (char cmd)
// sendet Kommando an Display
{
  unsigned char data_u, data_l;
	unsigned char data_t[4];
	
	data_u = (cmd & 0xf0);			// obere 4 Bit des Kommandos
	data_l = ((cmd << 4) & 0xf0);	// untere 4 Bit des Kommandos
	
	// Kommando senden
	data_t[0] = data_u|BL|E;  //en=1, rs=0
	data_t[1] = data_u|BL;  	//en=0, rs=0
	data_t[2] = data_l|BL|E;  //en=1, rs=0
	data_t[3] = data_l|BL;  	//en=0, rs=0
	
	I2CWrite(data_t,4,LCD_ADR);
}

void LCDSendData (char data)
// sendet Daten an Display
{
	unsigned char data_u, data_l;
	unsigned char data_t[4];

	// ... Datensenderoutine einfügen
	data_u = (data & 0xf0);          // upper 4 bits of data
  data_l = ((data << 4) & 0xf0);     // lower 4 bits of data
    
  // send data
  data_t[0] = data_u|BL|E|RS;    // en=1, rs=1
  data_t[1] = data_u|BL|RS;      // en=0, rs=1
  data_t[2] = data_l|BL|E|RS;    // en=1, rs=1
  data_t[3] = data_l|BL|RS;      // en=0, rs=1
  
	I2CWrite(data_t,4,LCD_ADR);
}



void LCDClear(void)
// Display löschen und kurz warten
{
	// ... Display löschen einfügen
	 LCDSendCmd(0x01);
	 DelayMs(20);
	
}

void LCDSendString (char *str)
// String an Display ausgeben
{
	while (*str) LCDSendData (*str++);
}

void LCDDefineDegree(void)
// definiert Gradzeichen als Zeichen 0 für LCD
{

	// ... Gradzeichen definieren
	char degree_char[8] = {0x1C, 0x14, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00};
    LCDSendCmd(0x40);  // Set CGRAM address
    for(int i=0; i<8; ++i) {
        LCDSendData(degree_char[i]);
    }
    LCDSendCmd(0x80);  // Set DDRAM address

}
