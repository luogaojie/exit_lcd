//filename :MyMisc.c
//func: Delay_ms(); Delay_us;
#include	"MyMisc.h"

BYTE	CalculateChekSum(BYTE *Data,BYTE Lenth,BYTE Method)
{
	BYTE	CheckSum = 0,Count = 0;
 
	for(Count = 0;Count < Lenth;Count ++)
	{
		switch(Method)
		{
			case ADD_SUM:	//相加求校验和
			{ 
				 CheckSum += *(Data + Count);
				 break;
			}
			case XOR_SUM:	//异或求校验和
			{
				CheckSum ^= *(Data + Count);
				break;
			}				
			default: break;	
		}	
	}
	return	CheckSum;
}

void	IO_NOT(void)
{
	//ION ^= ION;
}

void Delay_us(INT32U Count)
{
  ///*
    if(Count > 100000)
	{
		Count = 100000;
	}
  //*/
	while(Count--)
	{
		//asm(" RPT #111 || NOP");	
		//CLRWD;
	}
}


void Delay_ms(INT32U Count)
{
	while(Count--)
	{
		//asm(" RPT #111 || NOP");
        Delay_us(9100);
        if((Count % 5) == 0)
        {
           CLRWD;
        }
	}
}
