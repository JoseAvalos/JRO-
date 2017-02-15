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
char* resp2;

DDS DDS_JRO(CS,UDCLK,IO_RESET,MRESET);

void setup() 
{
	SPI.setModule(1);
	SPI.setBitOrder(MSBFIRST);
	SPI.begin();
	Serial.begin(115200);
	Serial.println("#####################################");
	DDS_JRO.init();
	resp=DDS_JRO.readData(0x87,4);
	DDS_JRO.print(resp,4);
	//char* freq ;
	//freq = DDS_JRO.freq2binary(2000000);
	//DDS_JRO.writeData(0x02,6,freq);
	//DDS_JRO.wrFrequency1double(400000);
	//resp=DDS_JRO.readData(0x02,6);
	//DDS_JRO.print(resp,6);
	//delay(3000);
	DDS_JRO.io_reset();
	

	DDS_JRO.wrFrequency1(DDS_JRO.freq2binary(12480000));
		//DDS_JRO.writeControlRegister();
	resp2=DDS_JRO.readData(0x82,6);
	DDS_JRO.print(resp2,6);

}

void loop() 
{
	//DDS_JRO.wrFrequency1double(10000);
}


/*
	SPI.setModule(1);
	SPI.setBitOrder(MSBFIRST);
	SPI.begin();
	Serial.begin(115200);
	Serial.println("#####################################");
	DDS_JRO.init();
	DDS_JRO.wrFrequency1(DDS_JRO.freq2binary(DEFAULT_FREQ));
	DDS_JRO.print(DDS_JRO.rdControl(),4);
	DDS_JRO.print(DDS_JRO.rdFrequency1(),6);
	//DDS_JRO.wrFrequency1(DDS_JRO.freq2binary(1000000));
*/