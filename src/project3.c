
/**
*@file project3.c
*@brief project3 implementation is done from this file
*@Author Aakash Kumar/ArunSundar
*@date Nov 20 2017

*/

#include "project3.h"

#define KL25Z



void project3(void)
{
	if()
	Logger_q=malloc(sizeof(CB_log_t));
	logdata_t * logData = malloc(sizeof(logdata_t));

	#ifdef KL25Z
	RTC_Init();
	#endif

	if(CB_log_init(Logger_q,10) == success) LOG_ITEM(createLog(LOGGER_INITIALZED,0,NULL),Logger_q);


	#ifdef KL25Z
	// Initializations required only in KL25Z
	UART_configure();
	DMA_Init();
	SysTick_Init(1000000);

	NVIC_SetPriority(RTC_Seconds_IRQn,0);
	NVIC_SetPriority(DMA0_IRQn,2); //Set priority of 1 for DMA interrupt
	NVIC_SetPriority(UART0_IRQn,4); //Set priority of 2 for UART0 interrupt
	__enable_irq(); // Enable global interrupts
	
	#endif
	LOG_RAW_INT(14);
	LOG_ITEM(createLog(SYSTEM_INITIALIZED,0,NULL),Logger_q);

	//malloc 2 buffers for setting up profiling transfers
	uint8_t * src_addr=malloc(5000),* dst_addr=malloc(5000);
	
	//

	
	//
	profiling_memory_functions(src_addr,dst_addr);
	// free the allocated pointers after profiling is done
	free(src_addr);
	free(dst_addr);
	
	/*
	while(1)
	{
		if(log_is_full(Logger_q)==buffer_full)
		{
			log_flush(Logger_q);
		}
	}*/

}



/*

	//LOG_RAW_STRING("\n\rInitialized \r\n");
	//LOG_RAW_STRING("\n\r");

	
	printf("\nBuild time %d : %d : %d :%d \n",buildday,buildhour,buildmin,buildsec);
	

	//Jan 1 1970 00:00 linux epoch

*/




/*	LOG_RAW_STRING("\n\rDest Addr: ");
	LOG_RAW_INT(dst_addr);
	LOG_RAW_STRING("\n\rDest Data\n\r");
	LOG_RAW_DATA(dst_addr,test_transfer_length);

	my_memcpy(src_addr,dst_addr,test_transfer_length);
	my_memmove_dma(src_addr,dst_addr,test_transfer_length);


	LOG_RAW_STRING("\n\rSource Addr: ");
	LOG_RAW_INT(src_addr);
	LOG_RAW_STRING("\n\rSource Data\n\r");
	LOG_RAW_DATA(src_addr,test_transfer_length);
	LOG_RAW_STRING("\n\rDest Addr: ");
	LOG_RAW_INT(dst_addr);
	LOG_RAW_STRING("\n\rDest Data\n\r");
	LOG_RAW_DATA(dst_addr,test_transfer_length);*/

