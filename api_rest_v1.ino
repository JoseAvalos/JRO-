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
/*  Original file by Joaquin
    Adaptation by Jose Avalos. January 2017
    Jicamarca Radio Observatory - JRO
 *******************************************************************/

#include <SPI.h>
#include <Ethernet.h>

#include <Base64.h>
#include <ArduinoJson.h>
#include <ArduinoHttpServer.h>

// Use MAC Address according to the TIVA  target
byte mac[] = {0x00, 0x1A, 0xB6, 0x02, 0xEC, 0x3E };

// Set ethernet instruction for communication
IPAddress ip(10, 10, 50, 129);
IPAddress mydns(10, 10, 10, 1);
IPAddress gateway(10, 10, 10, 1);
IPAddress subnet(255, 255, 255, 0);

// Default value
EthernetServer server(80);

// Define API commands
#define GET_NUMBER 2
#define POST_NUMBER 3

String get_operation[GET_NUMBER] = { "read", "status"};
String post_operation[POST_NUMBER] = {"write", "start", "stop"};


void setup()
{
  // Default value for Serial
  Serial.begin(115200);
  Serial.println("Starting Ethernet Connection...");

  Ethernet.begin(mac, ip, mydns, gateway, subnet);
  server.begin();

  Serial.println("Ethernet connection ready.");
  Serial.print("Server is at ");
  Serial.println(Ethernet.localIP());

  Serial.println("Configuration for DDS has already inicializated.");


}

void loop()
{
  EthernetClient client = server.available();
  if (client.connected())
  {

    ArduinoHttpServer::StreamHttpRequest<50000> httpRequest(client);
    ArduinoHttpServer::StreamHttpReply httpReply(client, "application/json");

    if (httpRequest.readRequest())
    {
      char* data = (char*) httpRequest.getBody();
      int data_length = (int) httpRequest.getContentLength();
      Serial.println( httpRequest.getResource()[0] );
      Serial.println(data);

      ArduinoHttpServer::MethodEnum method( ArduinoHttpServer::MethodInvalid );
      method = httpRequest.getMethod();

      if ( method == ArduinoHttpServer::MethodGet)
      {
        /*******************************************************
           READ
         ******************************************************/
        if (httpRequest.getResource()[0] == get_operation[0])
        {
          httpReply.send("{\"read\":\"ok\"}");
        }
        /*******************************************************
          STATUS
        ******************************************************/
        else if (httpRequest.getResource()[0] == get_operation[1])
        {
          httpReply.send("{\"status\":\"ok\"}");
        }
        else
        {
          httpReply.send("{\"msg\":\"wrong command\"}");
        }
      }
      else if ( method == ArduinoHttpServer::MethodPost)
      {
        /*******************************************************
          WRITE
        ******************************************************/
        if (httpRequest.getResource()[0] == post_operation[0])
        {
          httpReply.send("{\"write\":\"ok\"}");
        }
        /*******************************************************
          START 
        ******************************************************/
        else if (httpRequest.getResource()[0] == post_operation[1])
        {
          httpReply.send("{\"start\":\"ok\"}");
        }
        /*******************************************************
          STOP
        ******************************************************/
        else if (httpRequest.getResource()[0] == post_operation[2])
        {
          httpReply.send("{\"stop\":\"ok\"}");
        }
        else
        {
          ArduinoHttpServer::StreamHttpErrorReply httpReply(client, httpRequest.getContentType());
          httpReply.send(httpRequest.getErrorDescrition());
        }
      }

    }
    else
    {
      ArduinoHttpServer::StreamHttpErrorReply httpReply(client, httpRequest.getContentType());
      httpReply.send(httpRequest.getErrorDescrition());
    }
  }
  client.stop();
}


