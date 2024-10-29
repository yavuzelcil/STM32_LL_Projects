void StepperInit(void);
void SysTickInit (int load);
void StepOut(int step_pattern);

extern int StepDir;  		// Richtung des Schrittmotors
extern int AktPos;			// aktuelle Position des Schrittmotors
extern int ZielPos;			// Zielposition des Schrittmotors
extern int StepMode;		// Modus des Schrittmotors
extern int Ticks;

#ifndef StepMode_NONE
	// Positionierungsmodi
	#define StepMode_NONE 0		// Schrittmotor aus
	#define StepMode_FWD 1		// Schrittmotor dreht vorw�rts
	#define StepMode_BWD 2		// Schrittmotor dreht r�ckw�rts
	#define StepMode_POS 3		// Schrittmotor im Positionierungsmodus
	#define StepMode_REF 4		// Schrittmotor im Referenzierungsmodus
#endif
