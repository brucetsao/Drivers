/**************************************************************************
   File Name:       main.c
   Description:     WT588D test programme
   compiler:        Keil C51 9.02a
   Author:          LJW
   Created:         2011/09/02
   Modified:        NO
   Revision:        1.0
   MCU:             STC10F04
   Oscillator Crystal Frequency:    11.0592MHz
***************************************************************************/

/****************************************************/
/*                                                  */
/* Included files                                   */
/*                                                  */
/****************************************************/
#include "STC8051.H"            /* STC8051 definitions       */
#include <Delay.h>


/****************************************************/
/*                                                  */
/* 管脚定义                     */
/*                                                  */
/****************************************************/
/////////////////////////////////
sbit 	key1 = P3^7;
sbit 	key2 = P3^6;
sbit 	key3 = P2^4;
sbit 	key4 = P2^3;

sbit    LED_OUT   = P2^0;
sbit    FLAG_OUT   = P2^1;

//1Line_A
sbit 	P_DATA    = P0^0;			//定义数据传输端口对应语音芯片P03

//3Line_A
sbit 	P_DATA_3A = P0^0;			//定义数据传输端口
sbit 	CS_3A     = P0^1;	 		//定义片选信号端口
sbit 	CLK_3A    = P0^2;	 		//定义时钟端口



//一线，二线，三线有关
#define 	H  		1	//定义1为高电平
#define 	L 		0	//定义0为低电平

//////////////变量/////////////////
//bit 	flag 	  = 1;
bit 	B_DATA;						//传输数据一位
unsigned char  	SB_DATA   = 0;				//一字节公用数据缓存器
unsigned char  	S_DATA    = 0x00;

/******************************************
   键盘扫描函数
*******************************************/


unsigned char 	key,key_zhi;

unsigned int ADJ_NUM = 6;
unsigned int S_time = 5;

/*-------------------------------------- 
;模块名称:Line_1A(void)
;功    能:实现一线单字节低位在前串口通信函数
;入    参:无
;出    参:0	   返回主函数
;-------------------------------------*/
void Line_1A(unsigned char dat)
{
	unsigned char i;
	
	
//********************************************************

	P_DATA = 0;

	Delay_1ms(5);   				//拉低至少5ms

	B_DATA = dat&0X01;
	for(i=0;i<8;i++)
	{
		P_DATA = 1;				//拉高数据传输线，准备传输数据
		if(B_DATA==0)
		{	/*表示逻辑电平0*/
			Delay_10us(20);     // 延时200us
			P_DATA = 0;
			Delay_10us(20);	    // 延时600us
			Delay_10us(20);
			Delay_10us(20);	    
		}
		else
		{  /*表示逻辑电平1*/
			Delay_10us(20);
			Delay_10us(20);
			Delay_10us(20);
			P_DATA = 0;
			Delay_10us(20);
		}
		dat = dat>>1;
		B_DATA = dat&0X01;
	}
	P_DATA = 1;					//拉高电平
}

/*-------------------------------------- 
;模块名称:Line_3A(void)
;功    能:实现三线单字节低位在前串口通信函数适用于WT588D
;入    参:无
;出    参:0	   返回主函数
;-------------------------------------*/
void Line_3A(unsigned char dat)
{
	unsigned char i,key_copy = 0X00;
	P_DATA = 1;
	CLK_3A    = H;		 		//拉高
	
	CS_3A= L;							  //拉低片选信号
	Delay_1ms(5);  						//5ms
	B_DATA = dat&0X01;
		
	for(i=0;i<8;i++)
	{
		CLK_3A    = L;					// 拉低
		P_DATA_3A = B_DATA;			//传输数据一位
		
		Delay_10us(15);	 				//延时150us
		CLK_3A    = H;		 		//拉高
		Delay_10us(15);  		 		//延时150us
							
		dat = dat>>1;	 			
		B_DATA = dat&0X01;	  
	}
	P_DATA_3A  = 1;						//拉高电平
	CS_3A  = H;		   
	CLK_3A = H;				 
}

/*************************************************
  Function:       Main
  Description:    Main Function
  Input:          void
  Output:         NULL
  Return:         void
*************************************************/
void main()
{
	//P0口设为开漏极输出方式
	P0M0 = 0xff;
	P0M1 = 0xff;
	P0 = 0xFF;

	//P0口设为推挽输出方式
	P2M0 = 0x01;

	P4SW = (1<<6);	//P4.6为IO口
	P_DATA  = 1;			//拉高数据端口
	P_DATA_3A  = 1;						//拉高电平
	CS_3A  = H;		   
	CLK_3A = H;

	while(1)
	{		
		if(key1 == 0)
		{
			Delay_1ms(30);
			if(key1 == 0)
			{
				LED_OUT = !LED_OUT;
				ADJ_NUM ++;
				//Line_1A(ADJ_NUM);
				Line_3A(ADJ_NUM);
			}
			while(key1 == 0);
		}
		if(key2 == 0)
		{
			Delay_1ms(30);
			if(key2 == 0)
			{
				LED_OUT = !LED_OUT;
				ADJ_NUM --;
				//Line_1A(ADJ_NUM);
				Line_3A(ADJ_NUM);
			}
			while(key2 == 0);
		}
		Delay_10ms(150);
	}

}

/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
