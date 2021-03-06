/**
*@file logger_queue.c
*@brief Binary Logger queue functions
*@Author Aakash Kumar/Arun Sundar
*@date Nov 23 2017

*/

#include "logger_queue.h"

//#define VERBOSE

logdata_t logData;

/**
@brief  Initializes the data memebers of the CB_t structure
        and allocates memory for the circularbuffer 

@param  CB_t* source_ptr pointer to the CB_t structure

@param  length gives the length of the circular buffer

@return returns the status of the operation on the buffer
*/

CB_status CB_log_init(CB_log_t* source_ptr, size_t length)
{	
	/*checks for null pointer */
	if(source_ptr==NULL || length<=0)
	{
		LOG_ITEM(createLog(ERROR,0,NULL),Logger_q);
		return null_error;
		
	}
	else 
	{       /* initializes the pointers to the circular buffer*/

		source_ptr->base=(logdata_t *)calloc(length,sizeof(logdata_t));

		source_ptr->head=source_ptr->base;
		
		source_ptr->tail=source_ptr->head;
		
		source_ptr->length=length;
		
		source_ptr->limit=source_ptr->base+(length-1);
		
		source_ptr->count=0;
		
		return success;	
	}
	
}

/**
@brief  Adds data to the circular buffer 

@param  logdata_t *logVal pointer to the CB_t structure

@param  CB_log_t *source_ptr pointer to the buffer

@return returns the status of the operation on the buffer
*/

CB_status log_add (logdata_t *logVal, CB_log_t *source_ptr)
{
	if(log_enable)
	{
		/*checks for null pointer */
	if(log_is_full(source_ptr)==ok)
		{	/* For adding when the buffer is not full */
			START_CRITICAL(); // Start of critical section
			
			if(source_ptr->head>source_ptr->limit)
			{	
				source_ptr->head=source_ptr->base;
				*(source_ptr->head)=*logVal;
				source_ptr->head+=1;
	   		}
			else
			{
				*(source_ptr->head)=*logVal;
				source_ptr->head+=1;
			}

			source_ptr->count++;
			
			END_CRITICAL(); // End of critical section
			
			if(log_is_full(source_ptr)==buffer_full) log_flush(Logger_q);

			return success;
		}
	else 
		return log_is_full(source_ptr);

	}
	else
		return no_logging;
}

/**
@brief  Removes data to the circular buffer 

@param  CB_t* source_ptr pointer to the CB_t structure

@param  logvalue gets the value to be removed from the circular buffer

@return returns the status of the operation on the buffer
*/


CB_status log_remove(logdata_t * logValue, CB_log_t* source_ptr)
{
	if(log_is_empty(source_ptr)==ok)
	{	
		START_CRITICAL(); // Start of critical section
		
		/* gets the value that is removed */
		* logValue = *(source_ptr->tail);
		
		/* checks corner case and assigns tails to the base address */

		if(source_ptr->tail==source_ptr->limit)
		{
			source_ptr->tail=source_ptr->base;
		}
		else 
		{ 
			/* for regular cases other than the corner cases */	
			source_ptr->tail+=1;
		}
		source_ptr->count--;
		
		END_CRITICAL(); // end of critical section
		
		return success;
	}
	else 
		return log_is_empty(source_ptr);
}

/**
@brief prints the log data to the uart/stdio
@param logdata_t *logData pointer to the buffer containing data to be logged
@return none
*/
void print_log(logdata_t *logData)
{
	#ifdef VERBOSE
	LOG_RAW_STRING(logId_texts[logData->logID]);
	LOG_RAW_STRING(" at ");
	LOG_RAW_INT(logData->timestamp);
		if(logData->payload ==NULL || logData->logLength <1)
		{
			LOG_RAW_STRING(" with No payload ");
			LOG_RAW_STRING("\n\r");
		}
		else 
		{
			LOG_RAW_STRING(" with payload ");
			LOG_RAW_DATA(logData->payload,(logData->logLength));
			LOG_RAW_STRING("\n\r");
		}
		
	#else
	LOG_RAW_INT(logData->timestamp);LOG_RAW_STRING(",");
	LOG_RAW_INT(logData->logID);LOG_RAW_STRING(",");	
	if(logData->payload!=NULL) LOG_RAW_DATA((uint8_t *)logData->payload,(logData->logLength));
		if(calc_checksum(logData->logID,logData->timestamp,logData->logLength,logData->payload)==logData->checksum) LOG_RAW_STRING(",1");
		else LOG_RAW_STRING(",0");
	//LOG_RAW_DATA((uint8_t)&(logData->checksum),4);
	LOG_RAW_STRING("\n\r");
	#endif
	
}


/**
@brief Blocks till all the data is removed from the log buffer
@param CB_log_t* source_ptr pointer to the buffer containing data to be logged
@return none
*/

void log_flush(CB_log_t *  source_ptr){
	
	
	while(log_is_empty(source_ptr)!=buffer_empty)
	{
		log_remove(&logData,source_ptr);
		print_log(&logData);
	}
	
}

#ifndef KL25Z

CB_status log_is_empty(CB_log_t* source_ptr) 
	{
		/*checks for null pointer */
		if(source_ptr==NULL)
		{
			LOG_ITEM(createLog(ERROR,0,NULL),Logger_q);
			return null_error;
			
		}
		else
		{

			/* check buffer full condition */

			if(source_ptr->count==0)
			{
				return buffer_empty;
			}
			else return ok;
		}
	}

CB_status log_is_full(CB_log_t* source_ptr)
	{

		// checks for null pointer


		if(source_ptr==NULL)
		{
			LOG_ITEM(createLog(ERROR,0,NULL),Logger_q);
			return null_error;
		}

		else
		{

			// check buffer full condition

			if(source_ptr->count==source_ptr->length)
			{
				return buffer_full;
			}
			else return ok; // if buffer is not full and valid pointer is passed , return ok
		}
	}


#endif

