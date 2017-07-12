#include "stm32f10x.h"
#include "stm32f10x_it.h" 
#include "usart.h"

#define prd    10000
#define Vbreak 8000

u16 cnt2,cnt3,cnt4,cnt5;
s32 CNT2,CNT3,CNT4,CNT5;
s32 V2,V3,V4,V5;

extern s32 rcnt2,rcnt3,rcnt4,rcnt5;

void get_encoder_TWO(void)//******************计算当前实际速度V
{
  s32 CNT3_temp,CNT3_last;
 
  cnt3 = TIM3 -> CNT;
  CNT3_last = CNT3;
  CNT3_temp = rcnt3 * prd + cnt3; 
  V3 = CNT3_temp - CNT3_last;		
  
  while (V3>Vbreak)				 
  {							      
   rcnt3--;					      
   CNT3_temp = rcnt3 * prd + cnt3;
   V3 = CNT3_temp - CNT3_last;		 
  }							     
  while (V3<-Vbreak)			   
  {							      
   rcnt3++;					      
   CNT3_temp = rcnt3 * prd + cnt3;
   V3 = CNT3_temp - CNT3_last;		 
  }
  CNT3 = CNT3_temp;						 
  
}
