#include <SPI.h>
#include "BigNumber.h"

#define CS PA_5
#define UDCLK PD_2
#define IO_RESET PA_4

#define DDS_NBITS  48

int SPI_delay = 150;

byte resp6[] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
byte resp4[] = {0x0, 0x0, 0x0, 0x0};

void setup() {
  //Inicialization
  BigNumber::begin();
  //Serial init
  Serial.begin(115200); \
  Serial.println("#####################################");
  //SPI general configuration
  SPI.setModule(1);
  SPI.setBitOrder(MSBFIRST);
  SPI.begin();
  //PIN
  pinMode(CS, OUTPUT);
  pinMode(UDCLK, OUTPUT);
  pinMode(IO_RESET, OUTPUT);
  //Set off
  off(CS);
  off(UDCLK);
  delayMicroseconds(100);
  on(CS);
  delayMicroseconds(100);

  //Set initial to DDS using SPI
  off(CS);
  SPI.transfer(0x07);
  delayMicroseconds(SPI_delay);
  SPI.transfer(0x10);
  delayMicroseconds(SPI_delay);
  SPI.transfer(0x64);
  delayMicroseconds(SPI_delay);
  SPI.transfer(0x00);
  delayMicroseconds(SPI_delay);
  SPI.transfer(0x21);
  on(CS);

  on(IO_RESET);
  delayMicroseconds(SPI_delay);
  off(IO_RESET);
  delayMicroseconds(SPI_delay);

  off(CS);
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
  on(CS);

  Serial.println("Read information set: ");
  Serial.print("[ ");
  Serial.print(resp4[0], HEX);
  Serial.print(" ,");
  Serial.print(resp4[1], HEX);
  Serial.print(" ,");
  Serial.print(resp4[2], HEX);
  Serial.print(" ,");
  Serial.print(resp4[3], HEX);
  Serial.println(" ]");



  delayMicroseconds(100);

  //SET frequency
  byte *freq ;
  freq = freq2binary(100000, 60000000);
  delayMicroseconds(10);

  off(CS);
  delayMicroseconds(10);
  off(UDCLK);
  delayMicroseconds(10);

  on(IO_RESET);
  delayMicroseconds(10000);
  off(IO_RESET);
  delayMicroseconds(SPI_delay);


  on(UDCLK);
  delayMicroseconds(SPI_delay);
  off(UDCLK);
  delayMicroseconds(SPI_delay);



  SPI.transfer(0x02);
  delayMicroseconds(SPI_delay);
  SPI.transfer(*(freq + 0));
  delayMicroseconds(SPI_delay);
  SPI.transfer(*(freq + 1));
  delayMicroseconds(SPI_delay);
  SPI.transfer(*(freq + 2));
  delayMicroseconds(SPI_delay);
  SPI.transfer(*(freq + 3));
  delayMicroseconds(SPI_delay);
  SPI.transfer(*(freq + 4));
  delayMicroseconds(SPI_delay);
  SPI.transfer(*(freq + 5));
  delayMicroseconds(SPI_delay);
  on(CS);
  delayMicroseconds(SPI_delay);

  on(IO_RESET);
  delayMicroseconds(10000);
  off(IO_RESET);
  delayMicroseconds(SPI_delay);


}

void loop() {





  /*
    off(CS);
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
    on(CS);

    Serial.println("Read frecuency Tuning Word: ");
    Serial.print("[ ");
    Serial.print(resp6[0], HEX);
    Serial.print(" ,");
    Serial.print(resp6[1], HEX);
    Serial.print(" ,");
    Serial.print(resp6[2], HEX);
    Serial.print(" ,");
    Serial.print(resp6[3], HEX);
    Serial.print(" ,");
    Serial.print(resp6[4], HEX);
    Serial.print(" ,");
    Serial.print(resp6[5], HEX);
    Serial.println(" ]");
  */
}

byte *freq2binary(float freq, float mclock) {
  static byte bytevalue[6] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
  BigNumber DesiredOut = freq;
  BigNumber SYSCLK = mclock;
  BigNumber a = 0;
  BigNumber b = 256; // 2 bytes
  BigNumber x = 1;
  a = DesiredOut * pow64bits(2, 48) / SYSCLK;
  int n = 0;
  //Serial.print("[ ");

  while (a != 0) {
    bytevalue[n] =  byte(a % b);
    a =  a / b;
    //Serial.print(bytevalue[n], HEX);
    //Serial.print(", ");
    n++;
  }
  //Serial.println(" ]");
  /*
    Serial.println("Write frecuency Tuning Word: ");
    Serial.print("[ ");
    Serial.print(bytevalue[0], HEX);
    Serial.print(" ,");
    Serial.print(bytevalue[1], HEX);
    Serial.print(" ,");
    Serial.print(bytevalue[2], HEX);
    Serial.print(" ,");
    Serial.print(bytevalue[3], HEX);
    Serial.print(" ,");
    Serial.print(bytevalue[4], HEX);
    Serial.print(" ,");
    Serial.print(bytevalue[5], HEX);
    Serial.println(" ]");
    return bytevalue;
  */
}

BigNumber pow64bits(int a, int b) {
  BigNumber result = 1;
  BigNumber base = a;
  for (int i = 0; i < b; i++) {
    result = result * base;
  }
  return result;
}

void on(int x) {
  digitalWrite(x, HIGH);
}
void off(int x) {
  digitalWrite(x, LOW);
}
