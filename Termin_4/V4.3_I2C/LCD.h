#define LCD_ADR	0x27		// I2C-Adresse des LCDisplays

void LCDSendCmd (char cmd);
void LCDSendData (char data);
void LCDClear(void);
void LCDSendString (char *str);
void LCDDefineDegree(void);
