//IMPORTANT! Use esp32 boards version 2.0.17 or earlier
// ArduinoOTA.setPassword("admin");

//V02 corrected for ESP32 Wroom - working receiving frames and printing to serial
//V03 print to Nextion - working but need to add filter - it can't keep up
//V04 code tidy up
//V05 lower queue time and only print if changed - Think that due sending from savvycan is getting delayed in sending
//v06 adding extra esp data ... working
//V07 adding touch input from nextion to change pages ...working
//V08 adding park sensor data ...working
//V10 adding page 1 debug screen with DIaxlespeed to ESP speed
//V11 adding ota 
//v13 adding more 0x155 and 0x175 items
//v14 removed wait for wifi - Tested with Savvycan working well 
//v15 fixing ota problems
//v16 adding menu4 screen for speed and odo (LandRover 274059kms, Tesla 27411kms) 12% calibration
//v17 trying to make ota more reliable

//esp32 Serial2 pins: Tx=GPIO 17(yellow to nextion RX), RX=GPIO16(blue to nextion TX to receive touch)
#include "src/ESP32CAN.h"        //esp32_can library
#include "src/CAN_config.h"        //esp32_can library

#include "OTA.h"

//need to create these holding variables so that I can refer to them to only print when changed
byte ESP_driverBrakeApply = 0x00; //0=not initialised
byte ESP_brakeApply;
unsigned int ESP_brakeTorqueTarget;

//parking sensors
//front
unsigned int PARK_sdiSensor1RawDistData;
unsigned int PARK_sdiSensor2RawDistData;
unsigned int PARK_sdiSensor3RawDistData;
unsigned int PARK_sdiSensor4RawDistData;
unsigned int PARK_sdiSensor5RawDistData;
unsigned int PARK_sdiSensor6RawDistData;

//rear
unsigned int PARK_sdiSensor7RawDistData;
unsigned int PARK_sdiSensor8RawDistData;
unsigned int PARK_sdiSensor9RawDistData;
unsigned int PARK_sdiSensor10RawDistData;
unsigned int PARK_sdiSensor11RawDistData;
unsigned int PARK_sdiSensor12RawDistData;

CAN_device_t CAN_cfg;               // CAN Config

const int rx_queue_size = 3;       // Receive Queue size (10 default)

//0x108
int DI_axleSpeedraw;
int D_esp_wheelspeedraw;

//0x155 (341) ESP_wheelRotation
byte ESP_wheelAngleFL;//: 0|8@1+ (1.40625,0) [0|358.59375] "deg" X
byte ESP_wheelAngleFR;//: 8|8@1+ (1.40625,0) [0|358.59375] "deg" X
byte ESP_wheelAngleRL;//: 16|8@1+ (1.40625,0) [0|358.59375] "deg" X
byte ESP_wheelAngleRR;//: 24|8@1+ (1.40625,0) [0|358.59375] "deg" X
byte ESP_wheelRotationRR;//: 32|2@1+ (1,0) [0|3] ""       3 "UNKNOWN" 2 "STANDSTILL" 1 "BACKWARD" 0 "FORWARD" ;
byte ESP_wheelRotationRL;//: 34|2@1+ (1,0) [0|3] ""       3 "UNKNOWN" 2 "STANDSTILL" 1 "BACKWARD" 0 "FORWARD" ;
byte ESP_wheelRotationFR;//: 36|2@1+ (1,0) [0|3] ""       3 "UNKNOWN" 2 "STANDSTILL" 1 "BACKWARD" 0 "FORWARD" ;
byte ESP_wheelRotationFL;//: 38|2@1+ (1,0) [0|3] ""       3 "UNKNOWN" 2 "STANDSTILL" 1 "BACKWARD" 0 "FORWARD" ;
byte ESP_wheelRotationQF;//: 40|1@1+ (1,0) [0|1] ""       1 "VALID" 0 "INVALID" ;
byte ESP_standstillStatus;// : 41|1@1+ (1,0) [0|1] ""     1 "STANDSTILL" 0 "ROLLING" ;
int ESP_vehicleSpeed;// : 42|10@1+ (0.5,0) [0|511] "kph" 1023 "SNA" ;
byte ESP_wheelRotationCounter;// : 52|4@1+ (1,0) [0|15] ""  Receiver
byte ESP_wheelRotationChecksum;// : 56|8@1+ (1,0) [0|255] ""  Receiver

//0x175 (373) ESP_wheelSpeed
int ESP_wheelSpeedRR;// : 39|13@1+ (0.04,0) [0|327.64] "RPM" 8191 "SNA";
int ESP_wheelSpeedRL;// : 26|13@1+ (0.04,0) [0|327.64] "RPM" 8191 "SNA";
int ESP_wheelSpeedFR;// : 13|13@1+ (0.04,0) [0|327.64] "RPM" 8191 "SNA";
int ESP_wheelSpeedFL;// : 0|13@1+ (0.04,0) [0|327.64] "RPM" 8191 "SNA";
byte ESP_wheelSpeedCounter;// : 52|4@1+ (1,0) [0|15] ""  Receiver
byte ESP_wheelSpeedChecksum;// : 56|8@1+ (1,0) [0|255] ""  Receiver

//Nextion
byte menu = 0;
byte debugMenu = 1;

//unsigned int odoStart = 249938; //12% adjustment. Tesla 27411 so (27411*.88) = 24121. LandRover 274059-24121= 249938 (add Tesla ODO (frame * .88) to this number)

void setup() {
  //Serial.begin(9600); //debug serial

  Serial2.begin(115200); //nextian Serial port
  //uncomment below to Change Nextion Baud rate
  //Serial2.begin(9600); //default Nextion baud rate
  //delay(1000); //let the display settle down
  //Serial2.print("bauds=115200"); Serial2.write("\xFF\xFF\xFF"); //write new baud rate to Nextion 
  //Serial2.end(); //End serial
  //Serial2.begin(115200); //Restart serial at new baud rate
  //delay(1000); //let the display settle down
  



  //Serial.begin(115200);
  CAN_cfg.speed = CAN_SPEED_500KBPS;
  CAN_cfg.tx_pin_id = GPIO_NUM_22;
  CAN_cfg.rx_pin_id = GPIO_NUM_23;
  CAN_cfg.rx_queue = xQueueCreate(rx_queue_size, sizeof(CAN_frame_t));

  ESP32Can.CANInit(); // Init CAN Module
  delay(1000);//delay so that display can boot up before sending data to it.
  Serial2.print("xstr 0,0,240,60,5,WHITE,BLACK,0,1,1,\"Startup...\""); Serial2.write("\xFF\xFF\xFF");//xstr x, y, w, h, fontid, fontcolor, backcolor(or pic#), xcenter, ycenter, sta, string
  delay(100);
  menu = 1;
  Serial2.print("page 1"); Serial2.write("\xFF\xFF\xFF");
  delay(100);
}

void loop() {

if (menu==0)//esp
  {
  page0();
  }//end menu 1 actions

if (menu==1)//esp
  {
  page1();
  }//end menu 1 actions

if (menu==2)//tpms
  {
  page2();
  }//end menu 2 actions

if (menu==3)//parking sensors
  {
  page3();
  }//end menu 3 actions

  if (menu==4)//speed and odo
  {
  page4();
  }//end menu 4 actions

  if (menu==5)//speed and odo
  {
  page5();
  }//end menu 4 actions

readfromNex();

}//end loop
