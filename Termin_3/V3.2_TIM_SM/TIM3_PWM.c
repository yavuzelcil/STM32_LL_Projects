/*----------------------------------------------------------------------------
  Funktionen für PWM (Servo und RGB-LED)
	
	06/2020
 *----------------------------------------------------------------------------*/

#include <stm32f4xx_ll_bus.h>
#include <stm32f4xx_ll_gpio.h>
#include <stm32f4xx_ll_tim.h>

void TIM3InitPWM(void)
// Initialisierung des Timer 3 für PWM (Servo + RGB-LED)
{
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);			// GPIOA Takt einschalten
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);			// GPIOC Takt einschalten

	// PA6 (Servo) initialisieren
	LL_GPIO_SetPinSpeed(GPIOA,LL_GPIO_PIN_6,LL_GPIO_SPEED_FREQ_LOW);
	LL_GPIO_SetPinMode(GPIOA,LL_GPIO_PIN_6,LL_GPIO_MODE_ALTERNATE);
	LL_GPIO_SetAFPin_0_7(GPIOA,LL_GPIO_PIN_6,LL_GPIO_AF_2);

	// PA7 (LED rot) initialisieren
	LL_GPIO_SetPinSpeed(GPIOA,LL_GPIO_PIN_7,LL_GPIO_SPEED_FREQ_LOW);
	LL_GPIO_SetPinMode(GPIOA,LL_GPIO_PIN_7,LL_GPIO_MODE_ALTERNATE);
	LL_GPIO_SetAFPin_0_7(GPIOA,LL_GPIO_PIN_7,LL_GPIO_AF_2);

	// PC8 (LED grün) initialisieren
	LL_GPIO_SetPinSpeed(GPIOC,LL_GPIO_PIN_8,LL_GPIO_SPEED_FREQ_LOW);
	LL_GPIO_SetPinMode(GPIOC,LL_GPIO_PIN_8,LL_GPIO_MODE_ALTERNATE);
	LL_GPIO_SetAFPin_8_15(GPIOC,LL_GPIO_PIN_8,LL_GPIO_AF_2);

	// PC9 (LED blau) initialisieren
	LL_GPIO_SetPinSpeed(GPIOC,LL_GPIO_PIN_9,LL_GPIO_SPEED_FREQ_LOW);
	LL_GPIO_SetPinMode(GPIOC,LL_GPIO_PIN_9,LL_GPIO_MODE_ALTERNATE);
	LL_GPIO_SetAFPin_8_15(GPIOC,LL_GPIO_PIN_9,LL_GPIO_AF_2);

	// Timer initialisieren
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);
	LL_TIM_SetPrescaler(TIM3,160-1);
	LL_TIM_SetAutoReload(TIM3,2000-1);
	LL_TIM_SetCounterMode(TIM3,LL_TIM_COUNTERMODE_UP);
	
	// Kanal 1 (Servo)
	
	LL_TIM_OC_SetPolarity(TIM3,LL_TIM_CHANNEL_CH1,LL_TIM_OCPOLARITY_HIGH);
	LL_TIM_OC_SetMode(TIM3,LL_TIM_CHANNEL_CH1,LL_TIM_OCMODE_PWM1);
	LL_TIM_OC_SetCompareCH1(TIM3,150-1);
	
	LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH1);
	LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH2);
	LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH3);
	LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH4);
	
	
	
	// Kanal 2...4 (RGB-LED)
	LL_TIM_OC_SetPolarity(TIM3,LL_TIM_CHANNEL_CH2,LL_TIM_OCPOLARITY_LOW);
	LL_TIM_OC_SetMode(TIM3,LL_TIM_CHANNEL_CH2,LL_TIM_OCMODE_PWM1);
	LL_TIM_OC_SetCompareCH2(TIM3,1000-1);
	
	LL_TIM_OC_SetPolarity(TIM3,LL_TIM_CHANNEL_CH3,LL_TIM_OCPOLARITY_LOW);
	LL_TIM_OC_SetMode(TIM3,LL_TIM_CHANNEL_CH3,LL_TIM_OCMODE_PWM1);
	LL_TIM_OC_SetCompareCH3(TIM3,1000-1);
	
	LL_TIM_OC_SetPolarity(TIM3,LL_TIM_CHANNEL_CH4,LL_TIM_OCPOLARITY_LOW);
	LL_TIM_OC_SetMode(TIM3,LL_TIM_CHANNEL_CH4,LL_TIM_OCMODE_PWM1);
	LL_TIM_OC_SetCompareCH4(TIM3,1000-1);

	// Timer starten
	// ...
	LL_TIM_EnableCounter(TIM3);
}

void TIM3Servo (int pos)
// Servo auf Position fahren (0...100)
{
  if (pos <0 || pos >100)														// Abbruch, weil ausserhalb des Wertebereichs
		return;
	else{
		LL_TIM_OC_SetCompareCH1(TIM3,(pos+99));
	}
}

void TIM3RGB(int red, int green, int blue)
// RGB-LED-Helligkeit setzen (0...255)
{
  float ms20 = 2000;
	int red_ms = 0;
	int green_ms = 0;
	int blue_ms = 0;

	if ((red >=0) && (red <=255))
	{
    red_ms = (float)(red) / 255 * ms20;
		 LL_TIM_OC_SetCompareCH2(TIM3, red_ms);
	}
	if ((green >=0) && (green <=255))
	{
    green_ms = (float)(green) / 255 * ms20;
		 LL_TIM_OC_SetCompareCH3(TIM3, green_ms);
	}
	if ((blue >=0) && (blue <=255))
	{
    blue_ms = (float)(blue) / 255 * ms20;
		 LL_TIM_OC_SetCompareCH4(TIM3, blue_ms);
	}
}
