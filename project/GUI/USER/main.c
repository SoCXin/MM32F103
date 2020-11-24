#include "stdio.h"
//开发板范例代码21
//触摸屏实验
#include "application.h"

//FreeRTOS
#include "FreeRTOS.h"
#include "task.h"

/******************************************************************************
**函数信息 ：
**功能描述 ：
**输入参数 ：无
**输出参数 ：无
*******************************************************************************/
int main(void)
{
	taskENTER_CRITICAL();
	xTaskCreate( vSystemInitTask, (const  portCHAR * ) "Init", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+3, NULL );
	taskEXIT_CRITICAL();
	vTaskStartScheduler();
	return 0;
}

/*----------------------- (C) COPYRIGHT 2020 www.OS-Q.comm --------------------*/
