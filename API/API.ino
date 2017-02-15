/*********************************************************************

        API REST FOR DDS V.2.1

 *********************************************************************
   FileName:        api_rest_v1.ino
   Dependencies:    <Base64.h>
                    <ArduinoJson.h>
                    <ArduinoHttpServer.h>

   Processor:       TIVA TM4C1294C


 *********************************************************************
   Platform: Energia based in Arduino

   Features demonstrated:
      - UART perepheral library usage

   Description:
        This program shows the ethernet communication using json message
        for the commands: READ, WRITE, STATUS, START, STOP.

 ********************************************************************/
/*   Adaptation by Jose Avalos. January 2017
    Jicamarca Radio Observatory - JRO
 *******************************************************************/
// Energia Libraries
#include <SPI.h>
#include <Ethernet.h>

// Extern libraries
#include <Base64.h>
#include <ArduinoJson.h>
#include <ArduinoHttpServer.h>
#include <AD9854.h>
#include "Tiva.h"
#include "BigNumber.h"

#define CS PM_4 
#define UDCLK PA_6 
#define IO_RESET PD_7 
#define MRESET  PM_5
#define SPI_module 1
#define SYSCLOCK 60000000
#define DDS_NBITS  48
#define DEFAULT_FREQ 49920000

  
EthernetServer server(80);

char mac_tiva[6] ={0x00, 0x1A, 0xB6, 0x02, 0xEC, 0x3E };
char IP[4]={10,10,50,199};
int SPI_delay = 150;//us
char* resp;
char* freq ;


DDS DDS_JRO(CS,UDCLK,IO_RESET,MRESET);
API API_DDS(&DDS_JRO);

void setup()
{
  SPI.setModule(1);
  SPI.setBitOrder(MSBFIRST);
  SPI.begin();
  Serial.begin(115200);
  server.begin();
  Serial.print("Server is at ");
  Serial.println(Ethernet.localIP());
  Serial.println("Configuration for DDS has already inicializated.");
  DDS_JRO.init();
  //freq = DDS_JRO.freq2binary(40000000);
  DDS_JRO.wrFrequency1(freq);
  freq = DDS_JRO.freq2binary(12480000);
  //DDS_JRO.wrFrequency1(freq);

}

void loop()
{
  EthernetClient _client = server.available();
  int DDS_case;
  DDS_case=API_DDS.readcommand(_client);
  ArduinoHttpServer::StreamHttpRequest<50000> httpRequest(_client);
  ArduinoHttpServer::StreamHttpReply httpReply(_client, "application/json");
        
  //Serial.print("DDS_case ");
  //Serial.println(DDS_case);
  /*
  -------------------------------------
  |DDS_case   |         SITUATION       |
  -------------------------------------
  |     0    | Default no detected     |
  -------------------------------------
  |     1     | READ                    |
  -------------------------------------
  |     2     | STATUS                  |
  -------------------------------------
  |     3     |` GET - NO IDENTIFY       |
  -------------------------------------
  |     4     | WRITE                   |
  -------------------------------------
  |     5     | START                   |
  -------------------------------------
  |     6     | STOP                    |
  -------------------------------------
  |     7     | POST - NO IDENTIFY      |
  -------------------------------------
  */
}

