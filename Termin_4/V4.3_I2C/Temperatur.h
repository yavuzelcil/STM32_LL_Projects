#define TEMP_ADR    0x18  // Slave Adresse Temperaturfühler
#define TEMP_RES    0x08  // Resolution Register
#define TEMP_TR     0x05  // Temperature Register


int TempRead(void);
void TempSetRes(int resolution);
