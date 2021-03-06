/**
*@file project3.h
*@brief project3 functions
*@Author Aakash Kumar/ArunSundar
*@date Nov 11 2017

*/

#include<stdint.h>
#include<stdlib.h>
#include<stdio.h>

#include "logger.h"
#include "memory.h"
#include "logger_queue.h"
#include "profiler.h"


#define KL25Z

#ifdef KL25Z
#include "dma.h"
#include "led.h"
#include "spi.h"
#include "nordic.h"
#include "gpio.h"

#endif
/**
@brief Implementation function for project3
@param none
@return  none
*/
	
void project3(void);

/**
@brief Implementation function for spi-nrf interface
@param none
@return  none
*/

void spi_nrf(void);

void project3_demo(void);
