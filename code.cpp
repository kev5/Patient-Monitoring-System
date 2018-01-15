 # Copyright 2017 Keval Khara kevalkhara95@gmail.com
 
 /* Patient Monitoring System */

#include <avr/io.h>
#include <string.h>
#include <stdlib.h>

unsigned char u8_data;

//LCD connections 

//#define D0   PB6
//#define D1   PD4
//#define D2   PD3
//#define D3   PD2

#define E   PB7
#define RS   PB0

//Key interface
#define Key1 PC0  //bpm
#define Key2 PC1  //up
#define Key3 PC2  //down
#define Key4 PC3  //ok
#define Key5 PB3  //cancel 
#define Key6 PB5  //temp

//Memory Interface
#define SDA  PD6
#define SLC     PD7

#define BpKey PB2

//Declaration
int Write_EPROM(char addH,char addL,char data);
int Read_EPROM(char addH,char addL);

void display(char string[16]);
void displayrp(void);
void displaybyte(char D);
void dispinit(void);
void cleardisplay(void);
void line1(void);
void line2(void);
void line3(void);
void line4(void);
void epulse(void);
void delay_ms(unsigned int de);
void USART_Transmit(char data );
void senddata(char string[16]);
void USART_Init();
void USART_Receive();
void sendSMS();

char mystr[6];
int heartrate,q;
int main(void)
{ 
 unsigned char pbpm,sbpm,flg,mon,msgflg,i;
 
 int bpm,mheartrate,ptemp,stemp;
 bpm=0;
 pbpm=0;
 ptemp=0;
 stemp=0;
 sbpm=40;
 flg=0;
 mon=0; //Monitoring off
 msgflg=0; //No message sent
 mheartrate=0;

 DDRB = 0b11000011; //LCD port direction
 DDRD = 0b00011100; //LCD port direction
 PORTC= 0b00001111;
 PORTB= 0b00101000;
// PORTC=0;
  delay_ms(17);  //heart rate calc 100msec delay
// PORTC=0;

  delay_ms(500);  //Initiaize LCD
  dispinit();
  delay_ms(200);
 USART_Init();
// senddata("Hi There");
USART_Transmit('S');
delay_ms(2000);
delay_ms(2000);
USART_Transmit('H');

    line1();
    display("Heart Rate:000bpm");
    line2(); 
    display("Temperature:00.0C");
char flgg;
flgg=0x00;

 while(1)
 { 
//=====================Heart Rate Measurement========================================
    while(((PINC & 0x10) == 0x10) && (flg==0))
    {
     bpm=0;
    }  
    while(((PINC & 0x10) == 0x00) && (flg==0))  //ADC4
    {
     delay_ms(18); //166
     bpm++;
    }  
    
    if(flg==0) 
    {
      heartrate=6000/bpm;
      if((heartrate < 400) && (heartrate > 55))
      {
       sprintf(mystr, "%03d", heartrate);
       
       if(mon==0)   
       {
        USART_Transmit(40);
        USART_Transmit(72);
        senddata(mystr);
        USART_Transmit(41);
       }
       line3();
       display("Heart Rate:");
       display(mystr);
       display("bpm");
       bpm=0;
       mheartrate=heartrate;
      }
    }
//========================Temperature Measurement===================================
  if(flg==0)
  {
    ADMUX=0xE5;
    ADCSRA=0xC7;
    while (!(ADCSRA & (1<<ADIF)));
    q=ADCH;
    ADCSRA |= 1<<4; //ADCSRA = ADCSRA OR (0b00010000)
    sprintf(mystr, "%03d", (q*10));
       if(mon==0)   
       {
        USART_Transmit(40);
        USART_Transmit(84);
        senddata(mystr);
        USART_Transmit(41);
       }
    line4();
    display("Temperature:");
    displaybyte(mystr[0]);
    displaybyte(mystr[1]);
    displaybyte(0xDF);
    displaybyte(0x43);
    displaybyte(0x20);
  }  
//============================SMS decision=========================================
   if((mon==1) && (flg==0) && (msgflg==0))
   { 
    if(mheartrate > pbpm)
    {
     cleardisplay();
     line1();
     display("Heart Rate More ");
     line2();
     display("Sending SMS....."); //Send SMS after this line giving details of 
                                  //Heart Rate, Temperature
     sendSMS();

     delay_ms(30000);
     line1();
     display("    Patient     ");
     line2(); 
     display("Observing System");
     line3();
     display("Heart Rate:000bpm");
     msgflg=1;
    }
    if(q > ptemp)
    {
     cleardisplay();
     line1();
     display("Temperature More ");
     line2();
     display("Sending SMS.....");
     sendSMS();
     delay_ms(30000);
     line1();
     display("    Patient     ");
     line2(); 
     display("Observing System");
     line3();
     display("Heart Rate:000bpm");
     msgflg=1;
    }
   }
//==============================Heart Rate Setting==================================
    if(((PINC & 0x01)==0x00) && (flg==0)) //BPM
    {
     cleardisplay();
     line1();
     display(" Set Heart Rate ");
     line3();
     display("Heart Rate:");
     sprintf(mystr, "%03d", pbpm);
     display(mystr);
     display("bpm");
     flg=1;
     sbpm=pbpm;
    }
    if(((PINC & 0x02)==0x00) && (flg==1))//Up
    {
     if(sbpm < 250)
     {
      sbpm++;
     }
     line3();
     display("Heart Rate:");
     sprintf(mystr, "%03d", sbpm);
     display(mystr);
     display("bpm");
    }

    if(((PINC & 0x04)==0x00) && (flg==1)) //Down
    {
     if(sbpm > 0)
     {
      sbpm--;
     }
     line3();
     display("Heart Rate:");
     sprintf(mystr, "%03d", sbpm);
     display(mystr);
     display("bpm");
    }


    if(((PINC & 0x08)==0x00) && (flg==1)) //Ok
    {
     line1();
     display("    Patient     ");
     line2(); 
     display("Observing System");
     line3();
     display("Heart Rate:000bpm");
     line4(); 
     display("Temperature:00.0C");
     pbpm=sbpm;
     flg=0;
    }

    if((PINB & 0x08)==0x00)//Cancel
    {
     line1();
     display("    Patient     ");
     line2(); 
     display("Observing System");
     line3();
     display("Heart Rate:000bpm");
     line4(); 
     display("Temperature:00.0C");
     flg=0;
    }
//==========================Temperature setting menu=================================
    if(((PINB & 0x20)==0x00) && (flg==0)) //Temprature
    {
     cleardisplay();
     line1();
     display(" Set Temperature ");
     line3();
     display("Temperature:");
     sprintf(mystr,"%02d",ptemp);
     displaybyte(mystr[0]);
     displaybyte(mystr[1]);
     displaybyte(0xDF);
     displaybyte(0x43);
     displaybyte(0x20);
     flg=2;
     stemp=ptemp;
    }

    if(((PINC & 0x02)==0x00) && (flg==2))//Up
    {
     if(stemp < 80)
     {
      stemp++;
     }
     line3();
     display("Temperature:");
     sprintf(mystr, "%02d", stemp);
     displaybyte(mystr[0]);
     displaybyte(mystr[1]);
     displaybyte(0xDF);
     displaybyte(0x43);
     displaybyte(0x20);
    }

    if(((PINC & 0x04)==0x00) && (flg==2))//Down
    {
     if(stemp > 0)
     {
      stemp--;
     }
     line3();
     display("Temperature:");
     sprintf(mystr, "%02d", stemp);
     displaybyte(mystr[0]);
     displaybyte(mystr[1]);
     displaybyte(0xDF);
     displaybyte(0x43);
     displaybyte(0x20);
    }


    if(((PINC & 0x08)==0x00) && (flg==2))//Ok
    {
     line1();
     display("    Patient     ");
     line2(); 
     display("Observing System");
     line3();
     display("Heart Rate:000bpm");
     line4(); 
     display("Temperature:00.0C");
     ptemp=stemp;
     flg=0;
    }
//==============================Monitoring on/off==================================
    if(((PINC & 0x02)==0x00) && (flg==0))//Monitoring on
    {
     cleardisplay();
     line1();
     display("     Mobile     ");
     line2();
     display("   Monitoring   ");
     line3();
     display("       ON       ");
     delay_ms(3000);
     line1();
     display("    Patient     ");
     line2(); 
     display("Observing System");
     line3();
     display("Heart Rate:000bpm");
     line4(); 
     display("Temperature:00.0C");
     mon=1;
     msgflg=0;
    }
    if(((PINC & 0x04)==0x00) && (flg==0)) // Monitoring off
    {
     cleardisplay();
     line1();
     display("     Mobile     ");
     line2();
     display("   Monitoring   ");
     line3();
     display("      OFF       ");
     delay_ms(3000);
     line1();
     display("    Patient     ");
     line2(); 
     display("Observing System");
     line3();
     display("Heart Rate:000bpm");
     line4(); 
     display("Temperature:00.0C");
     mon=0;
    }
  
 }
}


void display(char string[16])
{
 int len,count;
  len = strlen(string);

   for (count=0;count<len;count++)
  {
    displaybyte(string[count]);
 }
}


void displaybyte(char D)
{
 //data is in Temp Register
  char D1;
  D1=D;
  D1=D1 & 0xF0;
  D1=D1 >> 4;  //Send MSB
  
  PORTB=PORTB & ~(0x40);
  PORTD=PORTD & ~(0x1C);

  PORTB |= ((D1 & 0x01) << 6);
  PORTD |= ((D1 & 0x02) << 3);
  PORTD |= ((D1 & 0x04) << 1);
  PORTD |= ((D1 & 0x08) >> 1);
  
 epulse();

  D1=D;
  D1=D1 & 0x0F;  //Send LSB

  PORTB=PORTB & ~(0x40);
  PORTD=PORTD & ~(0x1C);

  PORTB |= ((D1 & 0x01) << 6);
  PORTD |= ((D1 & 0x02) << 3);
  PORTD |= ((D1 & 0x04) << 1);
  PORTD |= ((D1 & 0x08) >> 1);
 
 epulse();
}


void dispinit(void)
{
 int count;
 char init[]={0x43,0x03,0x03,0x02,0x28,0x01,0x0C,0x06,0x02,0x02};
  
 PORTB &= ~(1<<RS);           // RS=0
 for (count = 0; count <= 9; count++)
  {
 displaybyte(init[count]);
  }
 PORTB |= 1<<RS;    //RS=1
}


void cleardisplay(void)
{
 PORTB &= ~(1<<RS);           // RS=0
    displaybyte(0x01);
 PORTB |= 1<<RS;    //RS=1
}

void line1(void)
{
 PORTB &= ~(1<<RS);           // RS=0
    displaybyte(0x80);
 PORTB |= 1<<RS;    //RS=1
}

void line2(void)
{
 PORTB &= ~(1<<RS);           // RS=0
    displaybyte(0xC0);
 PORTB |= 1<<RS;    //RS=1
}

void line3(void)
{
 PORTB &= ~(1<<RS);           // RS=0
    displaybyte(0x90);
 PORTB |= 1<<RS;    //RS=1
}

void line4(void)
{
 PORTB &= ~(1<<RS);           // RS=0
    displaybyte(0xD0);
 PORTB |= 1<<RS;    //RS=1
}

void epulse(void)
{
 PORTB |= 1<<E;
  delay_ms(1);
 PORTB &= ~(1<<E);
 delay_ms(1);
}

void delay_ms(unsigned int de)
{
unsigned int rr,rr1;
   for (rr=0;rr<de;rr++)
   {
  
  for(rr1=0;rr1<30;rr1++)   //395
  {
   asm("nop");
  }
   
   }
}


void USART_Transmit(char data )
{
 UDR = data;
 /* Wait for empty transmit buffer */
 while ( !( UCSRA & (1<<UDRE)) )
 ;
 /* Put data into buffer, sends the data */
 
}


void senddata(char string[16])
{
  int len,count;
  len = strlen(string);

   for (count=0;count<len;count++)
  {
    USART_Transmit(string[count]);
 }
}

void USART_Init()
{
/* Set baud rate */
 UBRRH = 0x00;  //12, 9600 Baud At 1MHz
 UBRRL =12; 
//Set double speed enabled 
  UCSRA |= (1<<U2X);
/* Enable receiver and transmitter */
 UCSRB = (1<<RXEN)|(1<<TXEN);
/* Set frame format: 8data, 2stop bit */
 UCSRC = (1<<URSEL)|(1<<USBS)|(3<<UCSZ0);
//Set interrupt on RX
//  UCSRB |= (1<<RXCIE);
}

void USART_Receive()
{
/* Wait for data to be received */
while ( !(UCSRA & (1<<RXC)) )
;
/* Get and return received data from buffer */
u8_data=UDR;
} 

void sendSMS()
{
     senddata("AT+CMGD=1");
     USART_Transmit(13);
     USART_Transmit(10);
     delay_ms(1000);
          
     senddata("AT+CMGF=1");
     USART_Transmit(13);
     USART_Transmit(10);
     delay_ms(1000);
     
     senddata("AT+CMGW=");
     USART_Transmit(34);
     senddata("+918108961633");  // Contact Number
     USART_Transmit(34);
     USART_Transmit(13);
     USART_Transmit(10);
     delay_ms(1000);
     
     senddata("Heart Rate :");
     sprintf(mystr, "%03d", heartrate);
     senddata(mystr);
     USART_Transmit(13);
     USART_Transmit(10);
     delay_ms(1000);
     senddata("Temperature :");
     sprintf(mystr, "%03d", (q*10));
     USART_Transmit(mystr[0]);
     USART_Transmit(mystr[1]);
     delay_ms(1000);
     USART_Transmit(26); //Cntrl+Z
     delay_ms(1000);
     delay_ms(1000);

     senddata("AT+CMSS=1");
     USART_Transmit(13);
     USART_Transmit(10);
     delay_ms(1000);
}
