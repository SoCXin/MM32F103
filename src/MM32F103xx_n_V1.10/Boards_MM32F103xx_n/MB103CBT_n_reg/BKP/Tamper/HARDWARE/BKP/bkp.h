#ifndef __BKP_H
#define __BKP_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////
//������
//LED��������
//////////////////////////////////////////////////////////////////////////////////
u8 CheckBackupReg(void);
void WriteToBackupReg(void);
void BKP_DATA(void);

#endif
