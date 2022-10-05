//------------------------------------------------------------------------------------
//程序功能简介：本程序产生15HZ~~~50KHZ的方波，并且实现频率和脉宽的独立调制，即可
//在改变频率的同时不改变脉宽，再改变脉宽的同时不改变频率；同时设置
//两个调节步长------在KEY键按下时，粗调，没有按下时，细调；
//程序思路： 本程序用到两个定时器------定时器0和定时器1，其中定时器0工作在定时方式下，
//决定方波的频率；定时器1，同样工作在定时方式下，用于设定脉宽；
//------------------------------------------------------------------------------------ 
#include "pwm.h"

unsigned char g_timer0_h,g_timer0_l,g_timer1_h,g_timer1_l;//定时器0和定时器1的初值设置；
unsigned char g_duty=50;             //初始占空比 50%
unsigned int  g_freq=1000;           //初始频率 1KHz
unsigned char g_flag_f=0,g_flag_d=0; //频率调节标志和脉宽调制标志；


//计算控制方波频率的定时器0的初始值
//入参freq：方波频率
//计算公式： F_osc/12 为产生1s需要的机器周期
//          产生（1/freq）s 需要 （F_osc/12 ）*（1/freq）机器周期
//          定时器初始值为 65536 - 机器周期数
void calculate_f(unsigned int freq)
{
	  unsigned int m_count;
	  m_count=65536- F_osc/(12*freq);
    g_timer0_h = m_count/256;
    g_timer0_l = m_count%256;
}

//计算控制方波占空比的定时器1的初始值
//入参 freq：方波频率  duty：方波占空比
//计算公式： F_osc/12 为产生1s需要的机器周期
//          产生(（1/freq）* duty) s 需要 （F_osc/12 ）*(（1/freq）* duty)机器周期
//          定时器初始值为 65536 - 机器周期数
void calculate_d(unsigned int freq,unsigned int duty)
{
	  unsigned int m_count1;
	  unsigned int m_count2;
	  m_count1= freq/(duty/100);
    m_count2=65536- F_osc/(12*m_count1);
    g_timer1_h = m_count2/256;
    g_timer1_l = m_count2%256;
}
	
void timer0() interrupt 1      //决定频率
{
 TH0=g_timer0_h;
 TL0=g_timer0_l;
 TR1=1;           //开定时器1；
 PWM_OUT=1;
}

void timer1() interrupt 3     //决定脉宽  
{
 TH1=g_timer1_h;
 TL1=g_timer1_l;
 TR1=0; //关定时器1，一定要这一步，因为定时器1的定时短于定时器0；
 PWM_OUT=0;
}
void pwm_init(void)
{
     TMOD=0x11;      //定时器0和定时器1都工作在方式1，16位计数器；
	   g_duty=50;   //初始占空比 50%
     g_freq=1000;     //初始频率 1kHz
     calculate_f(g_freq);  
     calculate_d(g_freq,g_duty);
	   TH0=g_timer0_h;
     TL0=g_timer0_l;
		 TH1=g_timer1_h;
		 TL1=g_timer1_l;
     ET0=1;
     ET1=1;
     EA=1;
     TR0=1;
     TR1=1;    
}


void key_scan(void)
{
	
	if(KEY_F_UP==0)
  {
		delay(10);
		if(KEY_F_UP==0)
     {
			 if(g_freq>=1000)
       {
				  g_freq=g_freq+10;
			 }
       else
       {
				 	g_freq++;
			 }
		
       if(g_freq>10000)
       {
				  g_freq=10000;
			 }
			 			 
			 g_flag_f=1;
		 }		
	}
	else if(KEY_F_DOWN==0)
  {
		delay(10);
		if(KEY_F_DOWN==0)
     {
			 
			 if(g_freq>=1000)
       {
				  g_freq=g_freq-10;
			 }
       else
       {
				 	g_freq--;
			 }
			 
       if(g_freq<100)
       {
				  g_freq=100;
			 }
			 g_flag_f=1;
		 }			
	}
	else if(KEY_D_UP==0)
  {
		delay(10);
		if(KEY_D_UP==0)
     {
			 g_duty++;
       if(g_duty>100)
       {
				  g_duty=100;
			 }
			 g_flag_d=1;
		 }			
	}
	else if(KEY_D_DOWN==0)
  {
		delay(10);
		if(KEY_D_DOWN==0)
     {
			 g_duty--;
       if(g_duty<0)
       {
				  g_duty=0;
			 }
			 g_flag_d=1;
		 }			
	}
	
}

void key_handle(void)
{
	 unsigned char  m_fqian,m_fbai,m_fshi,m_fge;
	 unsigned char  m_dbai,m_dshi,m_dge;
   unsigned char  ar_num[]={"0123456789"};
	
		if(g_flag_f==1)  //改变频率时要注意要进行脉宽的重新设置；
    {
			  calculate_f(g_freq);  
        calculate_d(g_freq,g_duty);
			
				 if(g_freq>=1000)
				 {
						 m_fqian=g_freq/1000;
						 m_fbai=(g_freq%1000)/100;
						 m_fshi=((g_freq%1000)%100)/10;
					 
						 lcd_pos(0x05);
					  if(m_fqian>=10)
             {
							 lcd_wdat('1');
							 lcd_wdat('0');
							 lcd_wdat('.');
							 lcd_wdat('0');
						 }
           else
             {
							 lcd_wdat(ar_num[m_fqian]);
							 lcd_wdat('.');
							 lcd_wdat(ar_num[m_fbai]);
							 lcd_wdat(ar_num[m_fshi]);
						 }

						 lcd_wdat('k');
						 lcd_wdat('H');
						 lcd_wdat('z');
				 }       
				 else if(g_freq>=100)
				 {
						 m_fbai=g_freq/100;
						 m_fshi=(g_freq%100)/10;
						 m_fge=(g_freq%100)%10;
					 
						 lcd_pos(0x05);
						 lcd_wdat(' ');
						 lcd_wdat(ar_num[m_fbai]);
						 lcd_wdat(ar_num[m_fshi]);
						 lcd_wdat(ar_num[m_fge]);
						 lcd_wdat(' ');
						 lcd_wdat('H');
						 lcd_wdat('z');
				 }
				 
			  g_flag_f=0;
		}
		if(g_flag_d==1)   // 脉宽改变，频率不改变；
    {
				 m_dbai=g_duty/100;
				 m_dshi=(g_duty%100)/10;
				 m_dge=(g_duty%100)%10;
			 
				 lcd_pos(0x47);
				 if(m_dbai==1)
					{
				     lcd_wdat(ar_num[m_dbai]);
					}
				 else
          {
						 lcd_wdat(' ');
				  }
				 lcd_wdat(ar_num[m_dshi]);
				 lcd_wdat(ar_num[m_dge]);
				 lcd_wdat('%');
			
        calculate_d(g_freq,g_duty);
			
			  g_flag_d=0;
		}
}
