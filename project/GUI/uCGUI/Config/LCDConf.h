#ifndef LCDCONF_H
#define LCDCONF_H


#define LCD_XSIZE          (240)	
#define LCD_YSIZE          (320)		
#define LCD_CONTROLLER     (9341)	 //LCD���������ͺ�
#define LCD_BITSPERPIXEL   (16)
#define LCD_FIXEDPALETTE   (565) //��ɫ���ʽ
#define LCD_SWAP_RB        (1)   //������ɫ����
#define LCD_INIT_CONTROLLER()    LCD_Driver_Init()         //TFT��ʼ������
#endif /* LCDCONF_H */

