//----------------------------------------------------------------------------- 
// This is the crc8 function for use on the PIC16 and PIC18 etc. microprocessors 
int crc8( unsigned int8 *data, unsigned int8 len)
{
  // adapted from SHT21 sample code from
  // http://www.sensirion.com/en/products/humidity-temperature/download-center/
  // x^8 + x^5 + x^4 + 1
  unsigned int8 crc = 0xff;
  unsigned int8 byteCtr;
  for (byteCtr = 0; byteCtr < len; ++byteCtr) {
    crc ^= data[byteCtr];
    for (unsigned int8 bit = 8; bit > 0; --bit) {
      if (crc & 0x80) {
        crc = (crc << 1) ^ 0x31;
      } else {
        crc = (crc << 1);
      }
    }
  }
  return crc;
}

void cmd_continuous_measure()
{
      int status, status1, status2;
      i2c_start(); 
      status = i2c_write(0x4A); // CHECK WITH SPD800 ID = 25  // 0 is ACK    
      printf("\r\n continuous measure write(0x4A): %u",status);
      
      status1 = i2c_write(0x36);                        // 0 is ACK
      printf("\r\n continuous measure write(0x36): %u",status1);
       
      status2 = i2c_write(0x1E); 
      printf("\r\n continuous measure write(0x15): %u",status2);      // 0 is ACK
      
      i2c_stop();
      delay_ms(5);
}

void stopmeasure()
{
      int status, status1, status2;
      i2c_start(); 
      status = i2c_write(0x4A); // CHECK WITH SPD800 ID = 25  // 0 is ACK    
      printf("\r\n stop write(0x4A): %u",status);
      
      status1 = i2c_write(0x3F);                        // 0 is ACK
      printf("\r\n stop write(0x3F): %u",status1);
       
      status2 = i2c_write(0xF9); 
      printf("\r\n stop write(0xF9): %u",status2);      // 0 is ACK
      
      i2c_stop();
      delay_ms(5);
}
void softreset(void)
{
      int  status1, status2;
      i2c_start(); 
      // Note that ID_address in this case is 0x00      
      status1 = i2c_write(0x00);                        // 0 is ACK
      printf("\r\n reset write(0x00): %u",status1);
       
      status2 = i2c_write(0x06); 
      printf("\r\n reset write(0x06): %u",status2);      // 0 is ACK
      
      i2c_stop();
      delay_ms(20);

}
void identifier()
{

      BYTE data[11];
      int8 status, status1, status2, status3, status4, status5;
      
       i2c_start(); 
      status = i2c_write(0x4A); // CHECK WITH SPD800 ID = 25  // 0 is ACK    
      printf("\r\nSensirion status: %u",status);
      
      status1 = i2c_write(0x36);                        // 0 is ACK
      printf("\r\nSensirion status1: %u",status1);
      
      status2 = i2c_write(0x7C); 
      printf("\r\nSensirion status2: %u",status2);      // 0 is ACK
      i2c_stop();
      i2c_start(); 
      status = i2c_write(0x4A); // CHECK WITH SPD800 ID = 25  // 0 is ACK    
      printf("\r\nSensirion status3: %u",status3);
      
      status1 = i2c_write(0xE1);                        // 0 is ACK
      printf("\r\nSensirion status4: %u",status4);
      
      status2 = i2c_write(0x02); 
      printf("\r\nSensirion status5: %u",status5);      // 0 is ACK
  
      i2c_start(); 
      i2c_write(0x4B); 
      data[0] = i2c_read(1); // 1 is ACK
      data[1] = i2c_read(1); // 1 is ACK
      data[2] = i2c_read(1); // 1 is ACK  
      data[3] = i2c_read(1); // 1 is ACK
      data[4] = i2c_read(1); // 1 is ACK
      data[5] = i2c_read(1); // 1 is ACK        
      data[6] = i2c_read(1); // 1 is ACK
      data[7] = i2c_read(1); // 1 is ACK          
      
      i2c_stop();   
      delay_us(20);
      printf("\r\nSensirion ID DATA[0]: %02X",data[0]);
      printf("\r\nSensirion ID DATA[1]: %02X",data[1]);
      printf("\r\nSensirion ID DATA[2]: %02X",data[2]);
      printf("\r\nSensirion ID DATA[3]: %02X",data[3]);
      printf("\r\nSensirion ID DATA[4]: %02X",data[4]);
      printf("\r\nSensirion ID DATA[5]: %02X",data[5]);
      printf("\r\nSensirion ID DATA[6]: %02X",data[6]);     
}


int8  cmd_pressure(void)
{
        BYTE status, status1, status2;      
      i2c_start(); 
      status = i2c_write(0x4A); // CHECK WITH SPD800 ID = 25  // 0 is ACK    
      printf("\r\nSensirion status: %u",status);
      
      status1 = i2c_write(0x36);                        // 0 is ACK, different pressure
      printf("\r\nSensirion status1: %u",status1);
      
      status2 = i2c_write(0x15);                        // 0 is ACK, different pressure, 2F
      printf("\r\nSensirion status2: %u",status2);
      i2c_stop();
      if(status2 == 0)
      return true;

}
float measure_pressure(void)
{
 union conv { 
    signed int16 f; 
    int8 b[2];
  };
  union conv  diffpressure, temp, scale;  // p is alias pointer
   BYTE  check1, check2, check3;
   BYTE data[10];
   float pressure, temperature; 
  unsigned int16 scale_factor;
  
      delay_ms(12); // in trigger mode delay_ms(50)
      i2c_start();
      i2c_write(0x4B); 
    
      data[1] = i2c_read(1); // 1 is ACK
      data[2] = i2c_read(1); // 1 is ACK
      data[3] = i2c_read(1); // 1 is ACK  
      data[4] = i2c_read(1); // 1 is ACK
      data[5] = i2c_read(1); // 1 is ACK
      data[6] = i2c_read(1); // 1 is ACK        
      data[7] = i2c_read(1); // 1 is ACK
      data[8] = i2c_read(1); // 1 is ACK   
      data[9] = i2c_read(1); // 1 is ACK

 
      i2c_stop();
      delay_ms(1);
      
      printf("\r\nDifferent pressure DATA[0]: %02X",data[0]);
      printf("\r\nDifferent pressure DATA[1]: %02X",data[1]);
      printf("\r\nDifferent pressure DATA[2]: %02X",data[2]);
      printf("\r\nDifferent pressure DATA[3]: %02X",data[3]);
      printf("\r\nDifferent pressure DATA[4]: %02X",data[4]);
      printf("\r\nDifferent pressure DATA[5]: %02X",data[5]);
      printf("\r\nDifferent pressure DATA[6]: %02X",data[6]);   
      printf("\r\nDifferent pressure DATA[7]: %02X",data[7]);
      printf("\r\nDifferent pressure DATA[8]: %02X",data[8]);
      printf("\r\nDifferent pressure DATA[9]: %02X",data[9]); 
      
      // crc-8 0x31(x^8 + x^5 + x^4 + 1)
      // Initialization 0xFF
      check1 = crc8(&data[1],2);
      check2 = crc8(&data[4],2);
      check3 = crc8(&data[7],2);
      
      printf("\r\nCheck 1: %02X",check1);
      printf("\r\nCheck 2: %02X",check2);
      printf("\r\nCheck 3: %02X",check3); 
      
       diffpressure.b[0] = data[0];
       diffpressure.b[1] = data[1];
       temp.b[0] = data[3]; //data[3]
       temp.b[1] = data[4]; //data[4]
       scale.b[0] = data[7];
       scale.b[1] = data[8];
       scale_factor = make16(data[7], data[8]);
       temperature = (float)temp.f / 200;
       pressure = ((float)diffpressure.f)/((float)scale_factor);
      // pressure = pressure;
       printf("\r\nScale factor value: %Ld",scale_factor );
       printf("\r\nTempeature value: %3.1f",temperature );
       printf("\r\nPressure value: %3.1f",pressure );
       return pressure;

 
}
