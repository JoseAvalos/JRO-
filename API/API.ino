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
//#include <iostream>
#include <string>

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


IPAddress IP(10, 10, 50, 179);
byte MAC[] ={0x00, 0x1A, 0xB6, 0x02, 0xEC, 0x3E };
EthernetServer server(80);

char* resp;
char* freq ;

DDS DDS_JRO(SYSCLOCK,CS,UDCLK,IO_RESET,MRESET);
API API_JRO(&DDS_JRO,IP,MAC);  


void setup()
{
  SPI.setModule(SPI_module);
  SPI.setBitOrder(MSBFIRST);
  SPI.begin();

  server.begin();

  Serial.begin(115200);
  Serial.println("#####################################");
  Serial.print("Server is at ");
  Serial.println(Ethernet.localIP());
  Serial.println("Configuration for DDS has already inicializated.");

  DDS_JRO.init();
  DDS_JRO.io_reset();
  DDS_JRO.wrFrequency1(DDS_JRO.freq2binary(DEFAULT_FREQ/4));
}

void loop()
{
  EthernetClient _client = server.available();
  int DDS_case;
  DDS_case=API_JRO.readcommand(_client);
  
  // if(DDS_case!=0)
  // {
  //   // ArduinoHttpServer::StreamHttpRequest<50000> httpRequest(_client);
  //   // ArduinoHttpServer::StreamHttpReply _httpReply(_client, "application/json");  
  //   // char* data = (char*) httpRequest.getBody();
  
  //   Serial.print("DDS_case ");
  //   Serial.println(DDS_case);
    
  //   /*
  //   -------------------------------------
  //   |DDS_case   |         SITUATION       |
  //   -------------------------------------
  //   |     0    | Default no detected     |
  //   -------------------------------------
  //   |     1     | READ                    |
  //   -------------------------------------
  //   |     2     | STATUS                  |
  //   -------------------------------------
  //   |     3     |` GET - NO IDENTIFY       |
  //   -------------------------------------
  //   |     4     | WRITE                   |
  //   -------------------------------------
  //   |     5     | START                   |
  //   -------------------------------------
  //   |     6     | STOP                    |
  //   -------------------------------------
  //   |     7     | POST - NO IDENTIFY      |
  //   -------------------------------------
  //   */
  //   // switch ( DDS_case ) 
  //   // {
  //   //   case 1:
  //   //     _httpReply.send("{\"read\":\"ok\"}");
  //   //     break;
      
  //   //   case 2:
  //   //     {
  //   //       if(DDS_JRO.verifyconnection())
  //   //       {
  //   //         JsonObject&  dds_status = jsonBuffer.createObject();
  //   //         dds_status["Conection"] = "YES";
  //   //         dds_status["Clock"]= SYSCLOCK ;
  //   //         dds_status["Frequency1"] =DDS_JRO.binary2freq(DDS_JRO.rdFrequency1())*DDS_JRO.getMultiplier();
  //   //         dds_status["Frequency2"] =DDS_JRO.binary2freq(DDS_JRO.rdFrequency2())*DDS_JRO.getMultiplier();
  //   //         dds_status["Multiplier"] =double(DDS_JRO.getMultiplier());
  //   //         dds_status["Clock"] =DDS_JRO.getclock();

  //   //         String hola1;
  //   //         dds_status.printTo(hola1);
  //   //         const String& msg_json = hola1;
  //   //         _httpReply.send(msg_json);
  //   //       }
  //   //       else
  //   //       {
  //   //         _httpReply.send("{\"Conection\":\"NO\"}");
  //   //       }

  //   //     }
  //   //     break;

  //   //   case 4:
  //   //     {
  //   //       //StaticJsonBuffer<4000> jsonBuffer_1;
  //   //       //JsonObject& jsondata = jsonBuffer_1.parseObject(data);
  //   //       //String hola2;
  //   //       //jsondata.printTo(hola2);
  //   //       //Serial.println(hola2);
          
  //   //       //double freq_1 =double(jsondata["frequency1"]);
  //   //       //Serial.println(freq_1);
  //   //       //char* prueba= DDS_JRO.freq2binary(freq_1);
  //   //       //DDS_JRO.print(prueba,8);
  //   //       //int exito= DDS_JRO.wrFrequency1(prueba);
  //   //       //Serial.println(exito);
  //   //       _httpReply.send("{\"write\":\"2\"}");
  //   //     }
  //   //     break;

  //   //   case 5:
  //   //     {
  //   //       _httpReply.send("{\"start\":\"ok\"}");
  //   //     }
  //   //     break;

  //   //   case 6:
  //   //     {
  //   //     _httpReply.send("{\"stop\":\"ok\"}");
  //   //     }
  //   //     break;
      
  //   //   default:
  //   //     break;
  //   // }
  // }
}

