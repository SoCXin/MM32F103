#ifndef __ALARM_H__
#define __ALARM_H__

#include "sys.h"

//////////////////////////���������������////////////////////////////////
#define ALARM_COUNT 6
#define ONEDAY (86400)
#define ALARM_SAVE_ADDR (64) //EEPROM save address

#define MASK_SLOT1 (0x1)
#define MASK_SLOT2 (0x2)
typedef enum _action_t{
	kOff=0,
  kOn=1,
}action_t;

typedef enum _dest_t{
	kSlotNone=0,
	kSlot1=MASK_SLOT1,
	kSlot2=MASK_SLOT2,
	kSlot3=4,
	kSlotAll=64,
}dest_t;

// ring buffer
typedef struct _alarm_control_t
{
	u32 seccount[ALARM_COUNT];  //���õ�����ʱ��
  u8 repeat[ALARM_COUNT];   //�Ƿ��ظ�
	action_t action[ALARM_COUNT];	//��������/��
	u8 hasSet[ALARM_COUNT];//�Ƿ��Ѿ�������Ч���������repeat������Զ����1
	u8 dest[ALARM_COUNT]; //����Ŀ��
//ring buffer ָ��		
	u8 current;//The current setting alarm
	u8 next; //The next alarm index
}alarm_control_t;

extern alarm_control_t g_alarmObj;

void Alarm_SetOneAlarm(u8 flag);
void Alarm_DelOneAlarm(u8 index);
void Alarm_LoadAlarmSetting(void);
void Alarm_StoreAlarmSetting(void);
//u32 Alarm_CheckValidAlarm(u32 count);
void Alarm_UpdateAlarmObj(u32 count, u8 dest, u8 isPeriod, action_t isOn);
#endif
