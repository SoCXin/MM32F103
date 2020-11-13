/*
*********************************************************************************************************
*	                                  
*	ģ������ : GUI����������
*	�ļ����� : MainTask.c
*	��    �� : V1.0
*	˵    �� : GUI����������
*	�޸ļ�¼ :
*		�汾��    ����          ����                 ˵��
*		v1.0    2012-12-07    Eric2013      ST�̼���汾 V1.0.2�汾��
*
*	Copyright (C), 2013-2014
*   QQ����Ⱥ��216681322
*   BLOG: http://blog.sina.com.cn/u/2565749395
*********************************************************************************************************
*/

#ifndef __MainTask_H
#define __MainTask_H

#include "stm32f4xx.h"

#include "GUI.h"
#include "DIALOG.h"
#include "WM.h"
#include "BUTTON.h"
#include "CHECKBOX.h"
#include "DROPDOWN.h"
#include "EDIT.h"
#include "FRAMEWIN.h"
#include "LISTBOX.h"
#include "MULTIEDIT.h"
#include "RADIO.h"
#include "SLIDER.h"
#include "TEXT.h"
#include "PROGBAR.h"
#include "SCROLLBAR.h"
#include "LISTVIEW.h"
#include "GRAPH.h"
#include "MENU.h"
#include "MULTIPAGE.h"
#include "ICONVIEW.h"
#include "TREEVIEW.h"

#include "ff.h"

/*************************BMP��Ϣͷ 40�ֽ�***************************/
typedef  __packed struct
{
    DWORD biSize ;		   //˵��BITMAPINFOHEADER�ṹ����Ҫ��������
    LONG  biWidth ;		   //˵��ͼ��Ŀ�ȣ�������Ϊ��λ 
    LONG  biHeight ;	   //˵��ͼ��ĸ߶ȣ�������Ϊ��λ 
    WORD  biPlanes ;	   //ΪĿ���豸˵��λ��������ֵ�����Ǳ���Ϊ1 
    WORD  biBitCount ;	   //˵��������/���أ���ֵΪ1��4��8��16��24����32
    DWORD biCompression ;  //˵��ͼ������ѹ�������͡���ֵ����������ֵ֮һ��
						   //BI_RGB��û��ѹ����
						   //BI_RLE8��ÿ������8���ص�RLEѹ�����룬ѹ����ʽ��2�ֽ����(�ظ����ؼ�������ɫ����)��  
    					   //BI_RLE4��ÿ������4���ص�RLEѹ�����룬ѹ����ʽ��2�ֽ����
  						   //BI_BITFIELDS��ÿ�����صı�����ָ�������������
    DWORD biSizeImage ;	   //˵��ͼ��Ĵ�С�����ֽ�Ϊ��λ������BI_RGB��ʽʱ��������Ϊ0  
    LONG  biXPelsPerMeter ;//˵��ˮƽ�ֱ��ʣ�������/�ױ�ʾ
    LONG  biYPelsPerMeter ;//˵����ֱ�ֱ��ʣ�������/�ױ�ʾ
    DWORD biClrUsed ;	   //˵��λͼʵ��ʹ�õĲ�ɫ���е���ɫ������
    DWORD biClrImportant ; //˵����ͼ����ʾ����ҪӰ�����ɫ��������Ŀ�������0����ʾ����Ҫ�� 
}BITMAPINFOHEADER ;

/*************************BMPͷ�ļ� 14�ֽ�***************************/
typedef  __packed struct
{
    WORD  bfType ;        //�ļ���־.ֻ��'BM',����ʶ��BMPλͼ����
    DWORD bfSize ;	      //�ļ���С,ռ�ĸ��ֽ�
    WORD  bfReserved1 ;   //����
    WORD  bfReserved2 ;   //����
    DWORD bfOffBits ;     //���ļ���ʼ��λͼ����(bitmap data)��ʼ֮��ĵ�ƫ����
}BITMAPFILEHEADER ;

/*************************λͼ��Ϣͷ 54�ֽ�*************************/
typedef  __packed struct
{ 
	BITMAPFILEHEADER bmfHeader;
	BITMAPINFOHEADER bmiHeader;    
}BITMAPINFO;

/*
************************************************************************
*						  FatFs
************************************************************************
*/
extern FRESULT result;
extern FIL file;
extern FILINFO finfo;
extern DIR DirInf;
extern UINT bw;
extern FATFS fs;

extern void GUI_DrawBMP(uint8_t S_xpos,uint16_t S_ypos,TCHAR *filename);
extern void GUI_SaveBMP(uint16_t startx,uint16_t starty,uint16_t sizex,uint16_t sizey,void *Save_Path);

/*
************************************************************************
*						emWin����
************************************************************************
*/
extern GUI_CONST_STORAGE  GUI_FONT GUI_FontHZ_Song_12;
extern GUI_CONST_STORAGE  GUI_FONT GUI_FontHZ_FangSong_16;
extern GUI_CONST_STORAGE  GUI_FONT GUI_FontHZ_Song_16;
extern GUI_CONST_STORAGE  GUI_FONT GUI_FontHZ_Hei_24;
extern GUI_CONST_STORAGE  GUI_FONT GUI_FontHZ_Kai_24;
extern GUI_CONST_STORAGE  GUI_FONT GUI_FontHZ_Song_24;
extern GUI_CONST_STORAGE  GUI_FONT GUI_FontHZ_SimSun_1616;
extern GUI_CONST_STORAGE  GUI_FONT GUI_FontHZ_SimSun_2424;

#endif

/*****************************(END OF FILE) *********************************/
