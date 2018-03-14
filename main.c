
#include <18F2480.h>
//#include <16F887.h>
#fuses HS,NOPROTECT,NOLVP,NOWDT
#use delay(clock=20000000)
#use i2c(Master, sda=PIN_C4, scl=PIN_C3,fast= 500000, FORCE_HW)  // FORCE_SW
#use rs232(baud=38400, xmit=PIN_C6, rcv=PIN_C7)
#include <can-18xxx8.c>

#include <spd800.c>


int16 ms;

#int_timer2
void isr_timer2(void) {
   ms++; //keep a running timer that increments every milli-second
}


//----------------------------------------------------------------------------- 
// This is the crc8 function for use on the PIC16 and PIC18 etc. microprocessors 

void main() {
  
   struct rx_stat rxstat;
   int32 rx_id;
   int in_data[8];
   int rx_len;

//send a request (tx_rtr=1) for 8 bytes of data (tx_len=8) from id 24 (tx_id=24)
   int out_data[8];
   int32 tx_id=21; // id = 0x29 for MAX31855 temperature
   
   int1 tx_rtr=0;// CU LA 1
   int1 tx_ext=1;// CU LA 0
   int tx_len=8;
   int tx_pri=3;

   int i;
  
   
   for (i=0;i<8;i++) {
      out_data[i]=0x33;
      in_data[i]=0;
   
     
   }

 ////////////////////
 
  union conv { 
    float f; 
    int8 b[4]; 
  };
  union conv RSCPressure;  // p is alias pointer
  
  float pressure;
 
 

   printf("\r\n\r\nCCS CAN EXAMPLE\r\n");
   setup_timer_2(T2_DIV_BY_4,79,16);   //setup up timer2 to interrupt every 1ms if using 20Mhz clock
   delay_ms(200);
   can_init();
  
   enable_interrupts(INT_TIMER2);   //enable timer2 interrupt
   enable_interrupts(GLOBAL);       //enable all interrupts (else timer2 wont happen)
   printf("\r\nRunning...");
 
   delay_ms(10);
      softreset();
      delay_ms(20);  
     // cmd_continuous_measure();
   while(TRUE)
   {   
        
      //stopmeasure();
      //delay_ms(5);
    
      softreset();
      delay_ms(50);
    //  identifier();
      delay_ms(50);    
     // cmd_continuous_measure();
     while(cmd_pressure())
     {
        
     
        pressure = measure_pressure();
        RSCPressure.f = pressure;
        out_data[0] = RSCPressure.b[0];
        out_data[1] = RSCPressure.b[1];
        out_data[2] = RSCPressure.b[2];
        out_data[3] = RSCPressure.b[3];
      printf("\r\nPressure value: %3.1f",pressure );     
      
       if ( can_tbe() && (ms > 2000))
        {
         ms=0;
         i=can_putd(tx_id, out_data, tx_len,tx_pri,tx_ext,tx_rtr); //put data on transmit buffer
         if (i != 0xFF) { //success, a transmit buffer was open
            printf("\r\nPUT %U: ID=%LU LEN=%U ", i, tx_id, tx_len);  //i return 1 if transmit success
            printf("PRI=%U EXT=%U RTR=%U\r\n   DATA = ", tx_pri, tx_ext, tx_rtr);
            //  printf("\r\nID=%X", out_data[0]);
            for (i=0;i<tx_len;i++) {
               printf("\r\n%X ",out_data[i]);
            }
            printf("\r\n");
         }
         else { //fail, no transmit buffer was open
            printf("\r\nFAIL on PUTD\r\n");
        
         }
        
      }
      delay_ms(500);
      // end 
     }
      delay_ms(500);
   }
}
