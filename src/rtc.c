/**
*@file rtc.c
*@brief RTC implementation is done from this file
*@Author Aakash Kumar/ArunSundar
*@date Nov 20 2017

*/




#include "rtc.h"

/**
@brief Configures the RTC for performing for sending time
@param none
@return  none
*/

uint8_t ledpayload[10],lplength;

void RTC_Init()
{

	// Clock Setup
	MCG_C1 |= MCG_C1_IRCLKEN_MASK; //Enable internal reference clock
	MCG_C2 &= ~(MCG_C2_IRCS_MASK);  //Internal Reference Clock -->Slow ??

	// Enable PORTC Clock
	SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;

	//**RTC_CLKIN**//
	PORTC_PCR1 |= (PORT_PCR_MUX(0x1));       //PTC1 as RTC_CLKIN
	SIM_SOPT1 |= SIM_SOPT1_OSC32KSEL(0b10);  //32 Khz clock source for RTC

	//**32KHz Frequency**//
	SIM_SOPT2 |= SIM_SOPT2_CLKOUTSEL(0b100); //Clockout pin --> 32 KHz

	PORTC_PCR3 |= (PORT_PCR_MUX(0x5)); //PTC3 as CLKOUT

	// RTC Setup
	SIM_SCGC6|=SIM_SCGC6_RTC_MASK; // Enable RTC in SIM
	SIM_SOPT1_OSC32KSEL(2); // Use RTC_CLKIN

	/*Clear Registers*/
	RTC_CR  = RTC_CR_SWR_MASK;
	RTC_CR  &= ~RTC_CR_SWR_MASK;

	if (RTC_SR & RTC_SR_TIF_MASK){
		#ifdef buildepochsec
		RTC_TSR = buildepochsec; // passing the build time obtained from makefile
		#else
		RTC_TSR = 0;
		#endif
	}

	/*Enable RTC seconds irq*/
	NVIC_ClearPendingIRQ(RTC_Seconds_IRQn);
	NVIC_EnableIRQ(RTC_Seconds_IRQn);

	/*Enable Seconds Interrupt*/
	RTC_IER |= RTC_IER_TSIE_MASK; //Seconds interrupt enable

	/*Timer enable*/
	RTC_SR |= RTC_SR_TCE_MASK;

}

/*
 *
 *
 */
/**
@brief gives the time for the logs
@param none
@return returns seconds from rtc 
*/
uint32_t getlogtime(){
	return RTC_TSR;
}

/** 
@brief IRQ handler for RTC interrupt
@param none
@return none
*/
void RTC_Seconds_IRQHandler()
{
	LOG_ITEM(createLog(HEART_BEAT,0,NULL),Logger_q);
	lplength=my_itoa(ledstate,ledpayload,10);
	LOG_ITEM(createLog(LED_STATUS,lplength,ledpayload),Logger_q);

}
