#include <AD9854.h>
#include <SPI.h>
#include "BigNumber.h"


DDS_function math_DDS;

#define CS PM_4 //PA_5
#define UDCLK PA_6 //PD_2
#define IO_RESET PD_7 //PA_4
#define MRESET	PM_5
#define SYSCLOCK_1 60000000
#define DDS_NBITS  48

int SPI_delay = 150;

byte resp6[] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
byte resp4[] = {0x0, 0x0, 0x0, 0x0};

DDS DDS_JRO;

void setup() {
  BigNumber::begin();
  pinMode(CS, OUTPUT);
  pinMode(UDCLK, OUTPUT);
  pinMode(IO_RESET, OUTPUT);
  pinMode(MRESET, OUTPUT);

  Serial.begin(115200);
  Serial.println("#####################################");

  SPI.setModule(1);
  SPI.setBitOrder(MSBFIRST);
  SPI.begin();

  on(MRESET);
  delay(1);
  off(MRESET);
  delay(1);

  DDS_JRO.reset(IO_RESET, 100);
 
  //PIN Inicialization
  //Initial reset

  delay(1);
  //Set initial configuration to DDS using SPI
  off(CS);
  SPI.transfer(0x07);
  delayMicroseconds(SPI_delay);
  SPI.transfer(0x10);
  delayMicroseconds(SPI_delay);
  SPI.transfer(0x64);
  delayMicroseconds(SPI_delay);
  SPI.transfer(0x01);
  delayMicroseconds(SPI_delay);
  SPI.transfer(0x01);
  on(CS);

  delayMicroseconds(SPI_delay);

  //PIN Inicialization
  //Initial reset
  on(IO_RESET);
  delayMicroseconds(SPI_delay);
  off(IO_RESET);
  delayMicroseconds(SPI_delay);

  // Verify the information
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
  
  //Set off
  off(UDCLK);
  on(IO_RESET);
  delayMicroseconds(10);
  off(IO_RESET);
  delayMicroseconds(10);

  //Set initial configuration to frequency
  byte *freq ;
  freq = math_DDS._freq2binary(20000, SYSCLOCK_1);

  off(CS);
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

  delayMicroseconds(10);
  on(UDCLK);
  delayMicroseconds(10);
  off(UDCLK);
  delayMicroseconds(SPI_delay);



  Serial.println("Frequency set: ");
  Serial.print("[ ");
  Serial.print(*(freq + 0), HEX);
  Serial.print(" ,");
  Serial.print(*(freq + 1), HEX);
  Serial.print(" ,");
  Serial.print(*(freq + 2), HEX);
  Serial.print(" ,");
  Serial.print(*(freq + 3), HEX);
  Serial.print(" ,");
  Serial.print(*(freq + 4), HEX);
  Serial.print(" ,");
  Serial.print(*(freq + 5), HEX);
  Serial.println(" ]");


  delayMicroseconds(SPI_delay);
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


 	on(IO_RESET);



}

void loop() {



}

void on(int x) {
  digitalWrite(x, HIGH);
}

void off(int x) {
  digitalWrite(x, LOW);
}



