//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//NGUYEN DINH PHU SU PHAM KY THUAT TP HCM
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//dinh nghia dia chi ghi va dia chi doc ds1307
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx                    
#define ds13_addr_wr 0xd0
#define ds13_addr_rd 0xd1

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx  
//khai bao 2 mang: 
//mang ghi: giay-phut-gio-thu-ngay-thang-nam-madk_msds  
//mang doc: giay-phut-gio-thu-ngay-thang-nam-madk_msds  
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx      
usi8 time_w[9]={0x30,0x33,0x15,0x03,0x16,0x02,0x21,0x90,0x92};  
usi8 time_r[9]={0x30,0x30,0x10,0x08,0x20,0x02,0x20,0x90,0x96};
#define giay_ds  time_r[0]              
#define phut_ds  time_r[1]                  
#define gio_ds   time_r[2]
#define thu_ds   time_r[3]
#define ngay_ds  time_r[4]
#define thang_ds time_r[5]                  
#define nam_ds   time_r[6]

#define ma_qd   time_w[8] 
#define ma_ds   time_r[8]                                       
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx  
//cap nhat time: giay-phut-gio-thu-ngay-thang-nam-madk_msds  
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx 
void ds1307_cap_nhat_time(int1 sel)                 
{         
   usi8 i;   
   i2c_start();
   i2c_write(ds13_addr_wr);        
   i2c_write(0x00);    
   for(i=0;i<9;i++)                          
   {  
      if(sel)  i2c_write(time_w[i]);
      else     i2c_write(time_r[i]);
   }                                 
   i2c_stop();        
}                      
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx  
//doc thoi gian   
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx                             
void ds1307_doc_time(usi8 j)     
{   
   usi8 i; 
   i2c_start();
   i2c_write(ds13_addr_wr);
   i2c_write(0x00);                     
   i2c_start();                
                
   i2c_write(ds13_addr_rd);                              
   for(i=0;i<j;i++) time_r[i]=i2c_read();  
                                 
   i = i2c_read(0); //not ack, don't care i  
   i2c_stop();
}                                                                       
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx  
//kiem tra ma da luu: sai thi cap nhat, dung thi thoat
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx           
void ds1307_kiem_tra_ma()                               
{                                              
   ds1307_doc_time(9);          
   if(ma_qd!=ma_ds)   
   {
      ds1307_cap_nhat_time(1);
   }
} 
void ds1307_luu_vao_ds(usi8 dcdt,dt)                 
{           
   i2c_start();
   i2c_write(ds13_addr_wr);        
   i2c_write(dcdt);    
   i2c_write(dt);
   i2c_stop();        
}

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx   
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//thu vien cua pcf8591: 4adc:2lm35, joystick, 1dac: led
//step size bang 5
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx   
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx   
#define  pcf_addr_wr  0x90                    
#define  pcf_addr_rd  0x91
                                                   
#define  pcf_chan0 0x40
#define  pcf_chan1 0x41
#define  pcf_chan2 0x42
#define  pcf_chan3 0x43
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx  
//ham thiet lap kenh
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx                                    
void pcf_channel(usi8 chan)
{                                  
   i2c_start();
   i2c_write(pcf_addr_wr);
   i2c_write(chan);      
   i2c_stop();
}
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx  
//ham vua chon kenh, vua xuat du lieu ra DAC
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx 
void pcf_xuat_dac(usi8 dac_out)
{
   i2c_start();                     
   i2c_write(pcf_addr_wr);
   i2c_write(pcf_chan0);
   i2c_write(dac_out);
   i2c_stop();             
}                    
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx  
//ham thiet lap kenh va doc ket qua cua lan truoc 
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx 
usi8 pcf_doc_adc(usi8 ch)
{    
   usi8 kq; 
   kq=0;  
   pcf_channel(ch);
   i2c_start();                           
   i2c_write(pcf_addr_rd);        
   kq= i2c_read(0);
   i2c_stop();       
   return kq;
}
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx  
//ham doc kenh, so lan 
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx 
usi8 pcf_doc_chan(usi8 chan, usi8 solan)                                     
{ 
   unsigned int16 pcf;
   usi8 i;           
   pcf = 0;     
   for(i=0;i<solan;i++)            
   {
      delay_us(30);                              
      pcf=pcf + pcf_doc_adc(chan);              
   }                                              
   pcf = pcf/solan;   
   return (pcf%256);
}

                                                                
