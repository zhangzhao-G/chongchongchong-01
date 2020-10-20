#include "TL740.h"
#include "usart.h"	  
uint8_t pointer = 0;
uint8_t T_i = 0 ;
uint32_t Total;
double Temp_angle ,Temp_aw;
double Calculate_Angle(uint8_t array[],uint8_t sizeofbuffer)
{
	pointer = 0 ;
	while(pointer<sizeofbuffer)
	{
		if(array[pointer]==0x68)
		{
			Total = array[pointer+1]+array[pointer+2]+array[pointer+3]+\
							array[pointer+4]+array[pointer+5]+array[pointer+6]+\
							array[pointer+7]+array[pointer+8]+array[pointer+9]+\
							array[pointer+10]+array[pointer+11]+array[pointer+12] ;
			if(Total%256==array[pointer+13])
			{
				sizeofbuffer = 0 ;
				if((array[pointer+10]/16)==1)
					Temp_angle = -(array[pointer+10]%16*100+array[pointer+11]/16*10+array[pointer+11]%16+array[pointer+12]/16*0.1+array[pointer+12]%16*0.01);
				else
					Temp_angle = (array[pointer+10]%16*100+array[pointer+11]/16*10+array[pointer+11]%16+array[pointer+12]/16*0.1+array[pointer+12]%16*0.01) ;	
				sizeofbuffer=0 ;
				break ;
			}
			else
				pointer++ ;
		}
		else
			pointer++ ;
		if(pointer== sizeofbuffer)
		{
			for( T_i=0;T_i<sizeofbuffer-1;T_i++)
			{
				array[T_i]=array[T_i+1];
			}
			return 360 ;
		}
	}
	return Temp_angle ;
}

double Calculate_aw(uint8_t array[],uint8_t sizeofbuffer)
{
	pointer = 0 ;
	while(pointer<sizeofbuffer)
	{
		if(array[pointer]==0x68)
		{
			Total = array[pointer+1]+array[pointer+2]+array[pointer+3]+\
							array[pointer+4]+array[pointer+5]+array[pointer+6]+\
							array[pointer+7]+array[pointer+8]+array[pointer+9]+\
							array[pointer+10]+array[pointer+11]+array[pointer+12] ;
			if(Total%256==array[pointer+13])
			{
				sizeofbuffer = 0 ;
				if((array[pointer+7]/16)==1)
					Temp_aw = -(array[pointer+7]%16*10+array[pointer+8]/16+array[pointer+8]%16*0.1+array[pointer+9]/16*0.01+array[pointer+9]%16*0.001);
				else
					Temp_aw = (array[pointer+7]%16*10+array[pointer+8]/16+array[pointer+8]%16*0.1+array[pointer+9]/16*0.01+array[pointer+9]%16*0.001);
				printf("Temp_aw=%f\n",Temp_aw);
				sizeofbuffer=0 ;
				break ;
			}
			else
				pointer++ ;
		}
		else
			pointer++ ;
		if(pointer== sizeofbuffer)
		{
			for( T_i=0;T_i<sizeofbuffer-1;T_i++)
			{
				array[T_i]=array[T_i+1];
			}
			return 0 ;
		}
	}
	return  Temp_aw ;
}


