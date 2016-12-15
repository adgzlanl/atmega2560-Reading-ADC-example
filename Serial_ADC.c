#include <avr/io.h>
#include <math.h>

#define FOSC 16000000// 8Mhz Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1
uint16_t value2,value3;
void USART_Transmit( unsigned char data );
void USART_Init( unsigned int ubrr);
unsigned char USART_Receive( void );
void SendString(char mydata[20]);
/*******************************************************/
int main( void )
{
  char mychar1[5];       //variable to hold ASCII values
  char mychar2[5];
  USART_Init (MYUBRR);
  InitADC();
  
  while(1)
  {


    value2=ReadADC(0);
    value3=ReadADC(2);
    //10-bit=1024 value for 2.56V
    //Vin=(5/1024)*ADC
    //We know that flowting point operation slow down the microcontroller
    //So we take it as  Vin=(2560/1024)*ADC
    //as we have multiplied it by 1000 so what ever comes we put fixed decimal point 0.000
    
    SendString("Voltage1:");
    sprintf(mychar1,"%04d",value2*5);  //This will convert integer into ASCII array
    USART_Transmit(mychar1[0]);  //First Digit
    USART_Transmit('.');        //Decimal Point
    USART_Transmit(mychar1[1]);  //Second Digit
    USART_Transmit(mychar1[2]);
    USART_Transmit(mychar1[3]);
    SendString(" Voltage2:");
    sprintf(mychar2,"%04d",value3*5);  //This will convert integer into ASCII array
    USART_Transmit(mychar2[0]);  //First Digit
    USART_Transmit('.');        //Decimal Point
    USART_Transmit(mychar2[1]);  //Second Digit
    USART_Transmit(mychar2[2]);
    USART_Transmit(mychar2[3]);
    USART_Transmit(13);
    USART_Transmit(10);
    //Similarly display for second channel
/*
    ADMUX=0xC1;    //Select Channel 1 and Internal Ref 2.56V
    ADCSRA=0xC7;   //ADEN=1, ADSC=1 Start Conversion, ADFR=0,ADIF=0, Prescalar=128 Slow for Voltage Measurement
    while (!(ADCSRA & (1<<ADIF))); //Wait until Conversion Completes
    value=2.5*ADC;
    ADCSRA |= 1<<4; //Clear ADIF Flag


    SendString(" Voltage2:");
    sprintf(mychar,"%04d",value);  //This will convert integer into ASCII array
    USART_Transmit(mychar[0]);  //First Digit
    USART_Transmit('.');        //Decimal Point
    USART_Transmit(mychar[1]);  //Second Digit
    USART_Transmit(mychar[2]);
    USART_Transmit(mychar[3]);
    USART_Transmit(13); //Line break
    */
  }
}

/*******************************************************/


void InitADC()
{
    // Select Vref=AVcc
    ADMUX |= (1<<REFS0);
    //set prescaller to 128 and enable ADC 
    ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)|(1<<ADEN);    
}


uint16_t ReadADC(uint8_t ADCchannel)
{
    //select ADC channel with safety mask
    ADMUX = (ADMUX & 0xF0) | (ADCchannel & 0x0F);
    //single conversion mode
    ADCSRA |= (1<<ADSC);
    // wait until ADC conversion is complete
    while( ADCSRA & (1<<ADSC) );
   return ADC;
}


void USART_Init( unsigned int ubrr)
{
  /* Set baud rate */
  UBRR0H = (unsigned char)(ubrr>>8);
  UBRR0L = (unsigned char)ubrr;
  /* Enable receiver and transmitter */
  UCSR0B = (1<<RXEN0)|(1<<TXEN0);
  /* Set frame format: 8data, 2stop bit */
  UCSR0C = (1<<USBS0)|(3<<UCSZ00);
}

/*******************************************************/
void USART_Transmit( unsigned char data )
{
  /* Wait for empty transmit buffer */
  while ( !( UCSR0A & (1<<UDRE0)) );
  /* Put data into buffer, sends the data */
  UDR0 = data;
}
/*******************************************************/
unsigned char USART_Receive( void )
{
  /* Wait for data to be received */
  while ( !(UCSR0A & (1<<RXC0)) );
  /* Get and return received data from buffer */
  return UDR0;
}
/*******************************************************/
void SendString(char mydata[20])
{
  int i;
  for(i=0;i<strlen(mydata);i++)
  {
    USART_Transmit(mydata[i]);
  }
}
/*******************************************************/