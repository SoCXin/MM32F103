#include "stdio.h"
//�����巶������21
//������ʵ�� 
#include "application.h"

//FreeRTOS
#include "FreeRTOS.h"
#include "task.h"


///////////////////////////////////////////////////////


 int main(void)
 { 
	taskENTER_CRITICAL();	
	xTaskCreate( vSystemInitTask, (const  portCHAR * ) "Init", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+3, NULL );
	taskEXIT_CRITICAL();
  vTaskStartScheduler();
	return 0;
}

