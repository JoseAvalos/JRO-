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

void setup() 
{
	Serial.begin(115200);
  Serial.println("#####################################");
	DDS_JRO.init();
	DDS_JRO.io_reset();
  resp=DDS_JRO.readData(0x87,4);
  DDS_tools.print(resp,4);
  char* freq ;
  freq = DDS_tools.freq2binary(20000000, SYSCLOCK);
	DDS_JRO.writeData(0x02,6,freq);
	resp=DDS_JRO.readData(0x02,6);
	DDS_tools.print(resp,6);
}

void loop() 
{

}
