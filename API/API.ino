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

   Description:
        This program shows the ethernet communication using json message
        for the commands: READ, WRITE, STATUS, START, STOP and CHANGEIP.

 ********************************************************************/
/*   Adaptation by Jose Avalos. January 2017
    Jicamarca Radio Observatory - JRO
 *******************************************************************/

// Energia Libraries
#include <SPI.h>
#include <Ethernet.h>
#include "Energia.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/debug.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"

// Extern libraries
#include <Base64.h>
#include <ArduinoJson.h>
#include <ArduinoHttpServer.h>
#include <AD9854.h>
#include <string>
#include <PinTiva.h>
#include "Tiva.h"
#include <lcd_tivac.h>

#define RESET_TIVA PL_4
#define MRESET  PE_4
#define OSK PC_4
#define CS PC_5
#define UDCLK PB_2
#define IO_RESET PB_3

#define SPI_module 2
#define Wire_module 1
#define SYSCLOCK 60000000

PIN LED_1(PN_1);
PIN LED_2(PN_0);
PIN LED_3(PF_4);
PIN LED_4(PF_0);
PIN PORT(PM_3);

IPAddress IP(10, 10, 50, 185);
byte MAC[] = {0x00, 0x1A, 0xB6, 0x02, 0xEC, 0x3E };
EthernetServer server(80);

DDS DDS_JRO(SYSCLOCK, CS, UDCLK, IO_RESET, MRESET);
API API_JRO(&DDS_JRO, IP, MAC);
LCD LCD_JRO(&DDS_JRO, &API_JRO);

bool status_dds=0;

void setup()
{
  
  SPI.setModule(SPI_module);
  SPI.setBitOrder(MSBFIRST);
  SPI.begin();

  Wire.setModule(Wire_module);
  
  server.begin();

  Serial.begin(115200);
  Serial.println("#####################################");
  Serial.print("Server is at ");
  Serial.println(Ethernet.localIP());
  Serial.println("Configuration for DDS has already inicializated.");
  //No icializa
  //LCD_JRO.init( 0x3c);
  initTimer(1);
  
  while (true)
  {
    if (DDS_JRO.init());
        break;
    LED_1.on();
    delay(5);
    LED_1.off();
  }
  LED_1.off();
  //    /*
  //    screen.putc(0x33);
  //    screen.putc(0x31);
  //
  DDS_JRO.io_reset();
  DDS_JRO.defaultSettings();
  int c=0;
  bool caso=0;
  while (DDS_JRO.verifyconnection())
  {
    status_dds= true;
    LED_1.on();
    int value = 0;
    
    EthernetClient _client = server.available();
    value = API_JRO.readcommand(_client);

    /*Case change IP*/
    if (value==99)
    {
      Serial.println("Changing Ethernet configuration.");
      Ethernet.setStaticIP(API_JRO._set_ip, API_JRO._set_gateway, API_JRO._set_subnet);
      Serial.println("Ethernet connection ready.");
      Serial.print("Server is at ");
      Serial.println(Ethernet.localIP());
    }

 
    if (DDS_JRO.isRFEnabled())
      LED_2.on();
    else
      LED_2.off();

    delay(400);

    // c+=1;
    // if(c=7){
    //   caso=!caso;
    //   if(caso)
    //     LED_1.on();
    //   else 
    //     LED_1.off();

    //   c=0;
    // }
   
  }
}

void loop(){}


void initTimer(unsigned Hz)
{
  SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
  IntMasterEnable();
  TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);
  unsigned long ulPeriod = (120000000/Hz);
  TimerLoadSet(TIMER1_BASE, TIMER_A, ulPeriod);
  IntEnable(INT_TIMER1A);
  TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
  TimerIntRegister(TIMER1_BASE, TIMER_A, Timer1IntHandler);
  TimerEnable(TIMER1_BASE, TIMER_A);
}

void Timer1IntHandler()
{
  TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
  Serial.println("hola");
  //if (status_dds)
    //LCD_JRO.showinformation();
  //else
    //LCD_JRO.text("No connection");
}


