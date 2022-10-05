#include "uart.h"
unsigned char ar_receive_data[20];
unsigned char g_receive_count;
unsigned char g_chk_code;
	
void uart_init(unsigned int baud)
{
	SCON =0x50;
	TL2 =RCAP2L= (65536-(F_osc/32/baud));
	TH2 =RCAP2H= (65536-(F_osc/32/baud))>>8;
	T2CON = 0x34;
	ES = 1;
	EA = 1;	
}

// 传送一个字符
void send_one_byte(unsigned char txd)
{
	SBUF = txd;
}

// 传送N个字符
void send_n_byte(unsigned char * s)
{
	while(*s)
  {
		send_one_byte(*s++);
	}
}

void uart() interrupt 4
{
   if(RI)
   {
		 ar_receive_data[g_receive_count++]=SBUF;		 
	 }
	 
	 if(TI)
   {
		 TI=0;
	 }
}

unsigned char bcc_chk(unsigned char * chk_data,unsigned char chk_len)
{
	unsigned char i,m_bcc=0;
	
	for(i=0;i<chk_len;i++)
	{
	  m_bcc^= chk_data[i];
	}
	return m_bcc;

}

unsigned char protocol_chk(unsigned char * receive_data )
{
	unsigned char m_data_len;
	m_data_len =receive_data[2]+4;
	
	if(receive_data[0]==0xCA)  //校验帧头
   {
		 if((receive_data[1]>=0x01)&&(receive_data[1]<=0x05)) //校验命令号
		  {
				 if(m_data_len==g_receive_count)
          {
						if(bcc_chk(receive_data,g_receive_count-1)==receive_data[g_receive_count-1])
             {
							  g_chk_code = 1; 
						 }
            else
             {
							 g_chk_code = 2;
						 }					  
				  }
         else
          {
						g_chk_code = 3; 
					}			 
		  }
     else
      {
				  g_chk_code = 4; 
		  }
	 }
  else
   {
		  g_chk_code = 5; 
	 }
	 
	 return g_chk_code;
	
}

void uart_handle(void)
{
	unsigned char m_cmd;
	unsigned char ar_send_buff[20];
	
	if(g_receive_count!=0)
   {
		 delay(1000);
		 if(protocol_chk(ar_receive_data)==1)
     {
			 m_cmd=ar_receive_data[1];
			 switch(m_cmd)
        {
					 case 0x01:
               {
								 ar_send_buff[0]=0xAC;
								 ar_send_buff[1]=m_cmd;
								 ar_send_buff[2]=0x01;
								 ar_send_buff[3]=0x11; //V0.1 V0.1
								 ar_send_buff[4]=bcc_chk(ar_send_buff,4);
								 
								 send_n_byte(ar_send_buff);
								 g_receive_count=0;
							 }						  
							break;
							 
						case 0x02:
               {
								 g_freq=ar_receive_data[3]*1000+ar_receive_data[4]*100+ar_receive_data[5]*10+ar_receive_data[6];
								 g_flag_f=1;
								 ar_send_buff[0]=0xAC;
								 ar_send_buff[1]=m_cmd;
								 ar_send_buff[2]=0x01;
								 if((g_freq>=100)&&(g_freq<=10000))
                 {
									  ar_send_buff[3]=0x01;
								 }
								 else
                 {
									  ar_send_buff[3]=0x00;
								 }
 
								 ar_send_buff[4]=bcc_chk(ar_send_buff,4);
								 
								 send_n_byte(ar_send_buff);
								 g_receive_count=0;
							 }						  
							break;
							 
						 case 0x03:
               {															 
								 ar_send_buff[0]=0xAC;
								 ar_send_buff[1]=m_cmd;
								 ar_send_buff[2]=0x04;
								 ar_send_buff[3]=g_freq/1000;
						     ar_send_buff[4]=(g_freq%1000)/100;
						     ar_send_buff[5]=((g_freq%1000)%100)/10;
								 ar_send_buff[6]=((g_freq%1000)%100)%10;

								 ar_send_buff[7]=bcc_chk(ar_send_buff,7);
								 
								 send_n_byte(ar_send_buff);
								 g_receive_count=0;
							 }						  
							break;
							 
					   case 0x04:
               {
								 g_duty=ar_receive_data[3]*100+ar_receive_data[4]*10+ar_receive_data[5];
								 g_flag_d=1;
								 ar_send_buff[0]=0xAC;
								 ar_send_buff[1]=m_cmd;
								 ar_send_buff[2]=0x01;
								 if((g_duty>=0)&&(g_duty<=100))
                 {
									  ar_send_buff[3]=0x01;
								 }
								 else
                 {
									  ar_send_buff[3]=0x00;
								 }
 
								 ar_send_buff[4]=bcc_chk(ar_send_buff,4);
								 
								 send_n_byte(ar_send_buff);
								 g_receive_count=0;
							 }						  
							break;
							 
						 case 0x05:
               {															 
								 ar_send_buff[0]=0xAC;
								 ar_send_buff[1]=m_cmd;
								 ar_send_buff[2]=0x04;
								 ar_send_buff[3]=g_duty/100;
						     ar_send_buff[4]=(g_duty%100)/10;
						     ar_send_buff[5]=(g_duty%100)%10;

								 ar_send_buff[6]=bcc_chk(ar_send_buff,6);
								 
								 send_n_byte(ar_send_buff);
								 g_receive_count=0;
							 }						  
							break;
					 
					 default:
						 break;
				}
		 }
		  
	 }
	
}
