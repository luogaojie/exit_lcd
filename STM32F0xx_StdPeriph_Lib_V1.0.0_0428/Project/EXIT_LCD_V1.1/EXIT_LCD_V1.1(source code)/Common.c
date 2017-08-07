//filename: Common.c

#include "Common.h"


volatile SYS_FLAG0 gSysFlg0;//系统标志

u16 ExitAddr = 0;//出口板实际地址
u16 ExitId = 0;//出口相对目标板地址

u8 Export[Export_Num]={0};//定义一个数组储存13个端口的状态值
u16 BoardAddr=0;//出口板的地址



BOOL Board_InOutSide_Flag=0;//板子的内外侧标志  0:内侧  1:外侧
BOOL PC_Check_Flag=0;  //PC查询标志位
BOOL InputScan_Flag = 0;//输入扫描

INT8U Input_PhotoElec[Export_Num]={0};//存储13个光电信号输入
INT8U InputPhotoElec_ONCnt[Export_Num]={0};//存储13个光电信号持续的时间
INT8U InputPhotoElec_Flag[Export_Num]={0};//光电输入信号标志位
INT8U InputScanCnt = 0;//输入扫描计数

INT8U ExportL[Export_Num],ExportH[Export_Num];//寄存输入信号