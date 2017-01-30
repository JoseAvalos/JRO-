#include <SPI.h>
#include "BigNumber.h"
#include <math.h>


#define CS PA_5
#define UDCLK PM_6
#define DDS_NBITS  48
#define INT64_C  0



int SPI_delay = 1;

byte resp6[] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
byte resp4[] = {0x0, 0x0, 0x0, 0x0};

void setup() {
  BigNumber::begin (); 
  Serial.begin(115200);
  SPI.setModule(1);
  SPI.setBitOrder(MSBFIRST);
  SPI.begin();

  pinMode(UDCLK, OUTPUT);
  pinMode(CS, OUTPUT);

  udclk_off();
  cs_on();
  delayMicroseconds(500);

  //Configuracion del SPI `
  cs_off();
  SPI.transfer(0x07);//07 //E0
  delayMicroseconds(SPI_delay);
  SPI.transfer(0x10);//10 // 08
  delayMicroseconds(SPI_delay);
  SPI.transfer(0x64);//64 //26
  delayMicroseconds(SPI_delay);
  SPI.transfer(0x01);//01 //80
  delayMicroseconds(SPI_delay);
  SPI.transfer(0x21);//21 //84
  cs_on();
  delayMicroseconds(500);


//
  byte *freq ;
  freq = freq2binary(600, 60000000);
//

  cs_off();
  SPI.transfer(0x02);
  delayMicroseconds(5);
  udclk_on();
  delayMicroseconds(SPI_delay);
  SPI.transfer(*(freq + 5));
  delayMicroseconds(SPI_delay);
  SPI.transfer(*(freq + 4));
  delayMicroseconds(SPI_delay);
  SPI.transfer(*(freq + 3));
  delayMicroseconds(SPI_delay);
  SPI.transfer(*(freq + 2));
  delayMicroseconds(SPI_delay);
  SPI.transfer(*(freq + 1));
  delayMicroseconds(SPI_delay);
  SPI.transfer(*(freq + 0));
  udclk_off();
  delayMicroseconds(SPI_delay);
  cs_on();
  delayMicroseconds(10);

 // while (1) {
    cs_off();
    SPI.transfer(0x87);
    delayMicroseconds(SPI_delay);
    resp4[0] = SPI.transfer(0x00);
    delayMicroseconds(SPI_delay);
    resp4[1] = SPI.transfer(0x00);
    delayMicroseconds(SPI_delay);
    resp4[2] = SPI.transfer(0x00);
    delayMicroseconds(SPI_delay);
    resp4[3] = SPI.transfer(0x00);
    delayMicroseconds(SPI_delay);
    cs_on();

    Serial.println("************************************************");
    Serial.println("Status: ");
    Serial.println(resp4[0], HEX);
    Serial.println(resp4[1], HEX);
    Serial.println(resp4[2], HEX);
    Serial.println(resp4[3], HEX);


    cs_off();
    SPI.transfer(0x82);
    delayMicroseconds(SPI_delay);
    resp6[0] = SPI.transfer(0x00);
    delayMicroseconds(SPI_delay);
    resp6[1] = SPI.transfer(0x00);
    delayMicroseconds(SPI_delay);
    resp6[2] = SPI.transfer(0x00);
    delayMicroseconds(SPI_delay);
    resp6[3] = SPI.transfer(0x00);
    delayMicroseconds(SPI_delay);
    resp6[4] = SPI.transfer(0x00);
    delayMicroseconds(SPI_delay);
    resp6[5] = SPI.transfer(0x00);
    delayMicroseconds(SPI_delay);
    cs_on();

    Serial.println("************************************************");
    Serial.println("Frecuency Tuning Word: ");
    Serial.println(resp6[0], HEX);
    Serial.println(resp6[1], HEX);
    Serial.println(resp6[2], HEX);
    Serial.println(resp6[3], HEX);
    Serial.println(resp6[4], HEX);
    Serial.println(resp6[5], HEX);
 // }
}

void loop() {
  //
}



void cs_on() {

  digitalWrite(CS, HIGH);
}



void cs_off() {

  digitalWrite(CS, LOW);
}



void udclk_on() {
  digitalWrite(UDCLK, HIGH);
}


void udclk_off() {

  digitalWrite(UDCLK, LOW);
}

byte * freq2binary(float freq, float mclock) {
  //long long int a=1;
  static byte bytevalue[6] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
  BigNumber a;
  a=pow64bits(2,5);
  
  Serial.println(a);
  
  /*
  double x=a/pow64bits(2,45);
  //double hola=pow(2,32);
  //Serial.println(hola);
  Serial.println(x);
  
  
  int n = 0;
  while (a!= 0) {
    bytevalue[n] =  byte(fmod(a,16));
    Serial.println(bytevalue[n]);
    a= round( a/ 16);
    n++;
  }
  */
  return bytevalue;
}


BigNumber pow64bits(int a, int b){
  
  char  hola[1];
  String a_string=String(a);
  a_string.toCharArray(hola,1) ;
  Serial.println(hola);
  
  BigNumber result="1";

  BigNumber base=hola;
 
  
  for (int i=0;i<b;i++){
    
    result=result*base;
    Serial.println(result);
    }
  
  return result;
  
  }
