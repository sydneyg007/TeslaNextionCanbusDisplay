//V01 printing corrected vehicle speed ...working
//V02 adding motor temp ...working
//v03 adding range and soc  ...working
//v04 adding batt temps and cell voltages   ...working
//v05 adding altitude  ...working
//v06 adding heating indicator    ...working
//v07 adding Di voltage current (and calculting KW)  ...working
//v08 adding cabin temperature   ...working
//v09 adding OTA and Nextion page 1 screen
//v10 removed wait for wifi
//V11 improving OTA reliability changed kms remaining on main screen to range left if 300wh/km
//V12 to go with Nextion V4 remaining KWH on main screen
//v14 adding PRND display and auto screen dim

//esp32 Serial2 pins: Tx=GPIO 17(yellow to nextion RX), RX=GPIO16(blue to nextion TX to receive touch)
#include "src/ESP32CAN.h"        //esp32_can library
#include "src/CAN_config.h"        //esp32_can library

#include "OTA.h"

CAN_device_t CAN_cfg;               // CAN Config

const int rx_queue_size = 3;       // Receive Queue size (10 default)

//Nextion
byte menu = 0;

void setup() {
Serial2.begin(115200); //nextian Serial port (set this to what it is set to in the "program.s" tab of the nextion editor)

CAN_cfg.speed = CAN_SPEED_500KBPS;
CAN_cfg.tx_pin_id = GPIO_NUM_22;
CAN_cfg.rx_pin_id = GPIO_NUM_23;
CAN_cfg.rx_queue = xQueueCreate(rx_queue_size, sizeof(CAN_frame_t));

ESP32Can.CANInit(); // Init CAN Module
delay(1000);
Serial2.print("xstr 0,0,240,60,5,WHITE,BLACK,0,1,1,\"Startup...\""); Serial2.write("\xFF\xFF\xFF");//xstr x, y, w, h, fontid, fontcolor, backcolor(or pic#), xcenter, ycenter, sta, string
delay(100);
menu = 1;
Serial2.print("page 1"); Serial2.write("\xFF\xFF\xFF");
delay(100);
}

void loop() {

if (menu==0)
  {
  page0();
  }//end menu 1 actions

if (menu==1)
  {
  page1();
  }//end menu 1 actions

if (menu==2)
  {
  page2();
  }//end menu 2 actions

if (menu==3)
  {
  page3();
  }//end menu 3 actions

  if (menu==5)
  {
  page5();
  }//end menu 5 actions

readfromNex();

}//end loop
