�豸���ƣ�
	MM32 CMSIS-DAP ������
�豸���ܣ�
	�豸�ӵ��Ժ�ö��ΪUSB�����豸��һ��USB�����豸��һ��HID�豸��
	HID�豸֧�ֶ�ARM�ں�оƬʹ��SWDģʽ���е��������ء�
	USB�����豸������USBת���ڹ��ߣ���UART����ͨ��USB�����COM�ڽ���ͨ�š�
���Ŷ��壺
	PIN_SWDIO_TMS_PIN ��PA4
	PIN_SWCLK_TCK_PIN ��PA5
	LED_CONNECTED_PIN ��PB13
	LED_RUNNING_PIN	  ��PB12
���������ã�
	������Ϊ��115200
	ͨ����ֵ�޸ģ�
	usbd_user_cdc_acm.c 	->
	int32_t UART_Reset(void)	->
	line_coding_current.dwDTERate��ֵ
ע�����
	��KEIL��֧��1M���ڣ�5M��10M�ٶȽ�����������ԡ�
	��֧��2M�ٶ����ص��ԣ�ʹ��2M�ٶ����ص��Ի��������������¸��豸�ϵ硣
	Win10���Դ�HID��CDC������Win7�谲װCDC������