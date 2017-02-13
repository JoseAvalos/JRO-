#include <AD9854.h>
#include <SPI.h>
#include "BigNumber.h"

#define CS PM_4 
#define UDCLK PA_6 
#define IO_RESET PD_7 
#define MRESET	PM_5
#define SPI_module 1
#define SYSCLOCK 60000000
#define DDS_NBITS  48

int SPI_delay = 150;//us
char* resp;

DDS DDS_JRO(CS,UDCLK,IO_RESET,MRESET,SPI_module,SPI_delay);
DDS_function DDS_tools;

void setup() {

  Serial.begin(115200);
  Serial.println("#####################################");

	DDS_JRO.init();
	DDS_JRO.io_reset();
  delayMicroseconds(SPI_delay);

  // Verify the information
  resp=DDS_JRO.readData(0x87,4);
  DDS_tools.print(resp,4);
  
  char* freq ;
  freq = DDS_tools.freq2binary(5000000, SYSCLOCK);
	DDS_JRO.writeData(0x02,6,freq);


 // delete[] resp;
  delayMicroseconds(SPI_delay);
	resp=DDS_JRO.readData(0x02,6);
	DDS_tools.print(resp,6);
	on(IO_RESET);
	while(1);
}

void loop() {


	
}

void on(int x) {
  digitalWrite(x, HIGH);
}

void off(int x) {
  digitalWrite(x, LOW);
}



