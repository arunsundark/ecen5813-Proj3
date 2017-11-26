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


#ifdef KL25Z
#include "dma.h"

#define PROJECT3_RUN project3_kl25z

void project3_kl25z(void);

#else

#define PROJECT3_RUN project3
	
void project3(void);




#endif