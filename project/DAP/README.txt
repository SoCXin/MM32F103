设备名称：
	MM32 CMSIS-DAP 调试器
设备功能：
	设备接电脑后枚举为USB复合设备，一个USB串行设备和一个HID设备。
	HID设备支持对ARM内核芯片使用SWD模式进行调试与下载。
	USB串行设备可用作USB转串口工具，将UART数据通过USB与电脑COM口进行通信。
引脚定义：
	PIN_SWDIO_TMS_PIN ：PA4
	PIN_SWCLK_TCK_PIN ：PA5
	LED_CONNECTED_PIN ：PB13
	LED_RUNNING_PIN	  ：PB12
波特率设置：
	波特率为：115200
	通过下值修改：
	usbd_user_cdc_acm.c 	->
	int32_t UART_Reset(void)	->
	line_coding_current.dwDTERate的值
注意事项：
	在KEIL中支持1M以内，5M、10M速度进行下载与调试。
	不支持2M速度下载调试，使用2M速度下载调试会出错，出错后需重新给设备上电。
	Win10下自带HID与CDC驱动，Win7需安装CDC驱动。