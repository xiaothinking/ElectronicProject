//------------------------------------------------------------------------------------
//�����ܼ�飺���������15HZ~~~50KHZ�ķ���������ʵ��Ƶ�ʺ�����Ķ������ƣ�����
//�ڸı�Ƶ�ʵ�ͬʱ���ı������ٸı������ͬʱ���ı�Ƶ�ʣ�ͬʱ����
//�������ڲ���------��KEY������ʱ���ֵ���û�а���ʱ��ϸ����
//����˼·�� �������õ�������ʱ��------��ʱ��0�Ͷ�ʱ��1�����ж�ʱ��0�����ڶ�ʱ��ʽ�£�
//����������Ƶ�ʣ���ʱ��1��ͬ�������ڶ�ʱ��ʽ�£������趨����
//------------------------------------------------------------------------------------ 
#include "pwm.h"

unsigned char g_timer0_h,g_timer0_l,g_timer1_h,g_timer1_l;//��ʱ��0�Ͷ�ʱ��1�ĳ�ֵ���ã�
unsigned char g_duty=50;             //��ʼռ�ձ� 50%
unsigned int  g_freq=1000;           //��ʼƵ�� 1KHz
unsigned char g_flag_f=0,g_flag_d=0; //Ƶ�ʵ��ڱ�־��������Ʊ�־��


//������Ʒ���Ƶ�ʵĶ�ʱ��0�ĳ�ʼֵ
//���freq������Ƶ��
//���㹫ʽ�� F_osc/12 Ϊ����1s��Ҫ�Ļ�������
//          ������1/freq��s ��Ҫ ��F_osc/12 ��*��1/freq����������
//          ��ʱ����ʼֵΪ 65536 - ����������
void calculate_f(unsigned int freq)
{
	  unsigned int m_count;
	  m_count=65536- F_osc/(12*freq);
    g_timer0_h = m_count/256;
    g_timer0_l = m_count%256;
}

//������Ʒ���ռ�ձȵĶ�ʱ��1�ĳ�ʼֵ
//��� freq������Ƶ��  duty������ռ�ձ�
//���㹫ʽ�� F_osc/12 Ϊ����1s��Ҫ�Ļ�������
//          ����(��1/freq��* duty) s ��Ҫ ��F_osc/12 ��*(��1/freq��* duty)��������
//          ��ʱ����ʼֵΪ 65536 - ����������
void calculate_d(unsigned int freq,unsigned int duty)
{
	  unsigned int m_count1;
	  unsigned int m_count2;
	  m_count1= freq/(duty/100);
    m_count2=65536- F_osc/(12*m_count1);
    g_timer1_h = m_count2/256;
    g_timer1_l = m_count2%256;
}
	
void timer0() interrupt 1      //����Ƶ��
{
 TH0=g_timer0_h;
 TL0=g_timer0_l;
 TR1=1;           //����ʱ��1��
 PWM_OUT=1;
}

void timer1() interrupt 3     //��������  
{
 TH1=g_timer1_h;
 TL1=g_timer1_l;
 TR1=0; //�ض�ʱ��1��һ��Ҫ��һ������Ϊ��ʱ��1�Ķ�ʱ���ڶ�ʱ��0��
 PWM_OUT=0;
}
void pwm_init(void)
{
     TMOD=0x11;      //��ʱ��0�Ͷ�ʱ��1�������ڷ�ʽ1��16λ��������
	   g_duty=50;   //��ʼռ�ձ� 50%
     g_freq=1000;     //��ʼƵ�� 1kHz
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
	
		if(g_flag_f==1)  //�ı�Ƶ��ʱҪע��Ҫ����������������ã�
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
		if(g_flag_d==1)   // ����ı䣬Ƶ�ʲ��ı䣻
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
