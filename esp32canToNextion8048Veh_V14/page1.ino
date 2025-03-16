void page1(){

//ArduinoOTA.handle();

CAN_frame_t rx_frame;   
  if (xQueueReceive(CAN_cfg.rx_queue, &rx_frame, 3 * portTICK_PERIOD_MS) == pdTRUE) {// Receive next CAN frame from queue
    for (int i = 0; i < rx_frame.FIR.B.DLC; i++) {//read can frame data
      if (rx_frame.MsgID == 0x257){ //0x257 (599) DI_speed
        unsigned int DI_vehicleSpeed = ((rx_frame.data.u8[1]  & B11110000) >> 4) + (rx_frame.data.u8[2] << 4); //DI_vehicleSpeed: 12|12@1+ (0.08,-40) [0|0] "kph" X
        Serial2.print("b0.txt=\""); Serial2.print(((DI_vehicleSpeed*.08)-40)*.876,0); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF");   //corrected vehcicle speed
      }//end 0x257 (599) DI_speed
      if (rx_frame.MsgID == 0x315){ //0x315 (789) DI_temperature
        byte DI_statorT = rx_frame.data.u8[2] -40;   //DI_statorT: 16|8@1+ (1,-40) [0|0] "DegC" X
        Serial2.print("b15.txt=\""); Serial2.print(DI_statorT); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF");   //corrected vehicle speed
      }// end 0x315 (789) DI_temperature

      if (rx_frame.MsgID == 0x118){ //0x118 (280) DI_systemStatus
        byte DI_gear = ((rx_frame.data.u8[2] & B11100000) >> 5) ;   //DI_gear: 21|3@1+ (1,0) [0|0] "" 0 "INVALID" 1 "P" 2 "R" 3 "N" 4 "D" 7 "SNA";
        Serial2.print("b6.txt=\""); 
        if (DI_gear==1){Serial2.print("P");}
        if (DI_gear==2){Serial2.print("R");} 
        if (DI_gear==3){Serial2.print("N");} 
        if (DI_gear==4){Serial2.print("D");} 
        Serial2.print("\""); Serial2.write("\xFF\xFF\xFF");   //corrected vehicle speed
      }// end 0x315 (789) DI_temperature
      
      /*below is not acurate!
      if (rx_frame.MsgID == 0x108){ //0x108 (264) DI_torque
        unsigned int DI_axleSpeed = rx_frame.data.u8[5] + (rx_frame.data.u8[6] << 8);//DI_axleSpeed: 40|16@1- (0.1,0) [0|0] "RPM"
        Serial2.print("b12.txt=\""); Serial2.print((DI_axleSpeed * .1), 0); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF"); 
        unsigned int DI_torqueActual = (((rx_frame.data.u8[3] & B11111000) >> 3) + (rx_frame.data.u8[4] << 5)) * 2;   //DI_torqueActual: 27|13@1- (2,0) [0|0] "Nm"
        Serial2.print("b13.txt=\""); Serial2.print(DI_torqueActual); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF"); 
        unsigned int DI_kwCalc = (DI_torqueActual * (DI_axleSpeed *.1)) /9549.29677;
        Serial2.print("b1.txt=\""); Serial2.print(DI_kwCalc); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF"); 
      }//end 0x108 (264) DI_torque
      */
      if (rx_frame.MsgID == 0x352){ //850 ID352BMS_energyStatus
      byte energyBuffer = ((rx_frame.data.u8[6] & B10000000)>>7) + ((rx_frame.data.u8[7] & B01111111)<<1);//55|8@1+ (0.1,0) [0|25.4] "KWh"
      Serial2.print("b8.txt=\" "); Serial2.print(((energyBuffer * .1)/.3),1); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF");
      int EnergyRemaining = ((rx_frame.data.u8[1] & B11111000)>>3) + ((rx_frame.data.u8[2] & B00111111)<<5);//11|11@1+ (0.1,0) [0|204.6] "KWh"
      Serial2.print("b3.txt=\" "); Serial2.print((EnergyRemaining * .1),1); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF");
      //range left if 300wh/km
      Serial2.print("b4.txt=\" "); Serial2.print(((EnergyRemaining * .1)/.3),0); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF");    
      }//end 850 ID352BMS_energyStatus

      if (rx_frame.MsgID == 0x33A){ //0x33A (818) UI_rangeSOC
        //unsigned int UI_expectedRange = rx_frame.data.u8[0] + ((rx_frame.data.u8[1] & B00000011) << 8);       //UI_expectedRange : 0|10@1+ (1.6,0) [0|1023] "km"
        //Serial2.print("b4.txt=\""); Serial2.print((UI_expectedRange * 1.6),0); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF");
        byte UI_usableSOC = rx_frame.data.u8[7] & B01111111; //UI_usableSOC : 56|7@1+ (1,0) [0|127] "%" X
        Serial2.print("b11.txt=\""); Serial2.print(UI_usableSOC); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF");
        Serial2.print("h2.val="); Serial2.print(UI_usableSOC*3);Serial2.write("\xFF\xFF\xFF");
      } //end 0x33A (826) UI_rangeSOC

      if (rx_frame.MsgID == 0x332){ //0x332 (826) BMS_bmbMinMax
        if((rx_frame.data.u8[0] & B00000011) == 0){ // BMS_bmbMinMaxMultiplexer M: 0|2@1+ (1,0) [0|0] ""   0 "THERM_MUX0" 1 "VOLT_MUX1" 2 "END"
          byte BMS_thermistorTMin = rx_frame.data.u8[3];         // BMS_thermistorTMin m0: 24|8@1+ (0.5,-40) [0|0] "DegC"
          Serial2.print("b14.txt=\""); Serial2.print(((BMS_thermistorTMin * 0.5)-40),0); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF");
          byte BMS_thermistorTMax = rx_frame.data.u8[2];        // BMS_thermistorTMax m0: 16|8@1+ (0.5,-40) [0|0] "DegC"
          Serial2.print("b13.txt=\""); Serial2.print(((BMS_thermistorTMax * 0.5)-40),0); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF");
          Serial2.print("h3.val="); Serial2.print((((BMS_thermistorTMin * 0.5)-40)*5),0); Serial2.write("\xFF\xFF\xFF");
          Serial2.print("h3.hig="); Serial2.print(BMS_thermistorTMax-BMS_thermistorTMin); Serial2.write("\xFF\xFF\xFF");// change slider thickness based on temp difference
        }// end BMS_bmbMinMaxMultiplexer (0 "THERM_MUX0")
        if((rx_frame.data.u8[0] & B00000011) == 1){ // BMS_bmbMinMaxMultiplexer M: 0|2@1+ (1,0) [0|0] ""   0 "THERM_MUX0" 1 "VOLT_MUX1" 2 "END"
          unsigned int BMS_brickVoltageMin = rx_frame.data.u8[2] + ((rx_frame.data.u8[3] & B00001111) << 8);   // BMS_brickVoltageMin m1: 16|12@1+ (0.002,0) [0|0] "V"
          Serial2.print("b10.txt=\""); Serial2.print(BMS_brickVoltageMin * .002); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF");
          unsigned int BMS_brickVoltageMax = ((rx_frame.data.u8[0] & B11111100) >> 2) + ((rx_frame.data.u8[1] & B00111111) << 6);  // BMS_brickVoltageMax m1: 2|12@1+ (0.002,0) [0|0] "V" 
          Serial2.print("b9.txt=\""); Serial2.print(BMS_brickVoltageMax * .002); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF"); 
        }// end BMS_bmbMinMaxMultiplexer (1 "VOLT_MUX1")
      }//end 0x332 (826) BMS_bmbMinMax
      
      if (rx_frame.MsgID == 0x3D8){ //0x3D8 (984) UI_elevationStatus
        unsigned int UI_elevation = rx_frame.data.u8[0] + ((rx_frame.data.u8[1] & B00111111) << 8);  //UI_elevation: 0|14@1+ (1,0) [0|0] "m"
        Serial2.print("b7.txt=\""); Serial2.print(UI_elevation); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF"); 
      }//end 0x3D8 (984) UI_elevationStatus

      if (rx_frame.MsgID == 0x126){ //0x126 (294) DI_hvBusStatus
        unsigned int DI_voltage = rx_frame.data.u8[0] + ((rx_frame.data.u8[1] & B00000011) << 8); //DI_voltage : 0|10@1+ (0.5,0) [0|500] "V"
        int DI_current = ((rx_frame.data.u8[1] & B11111100) >> 2) + ((rx_frame.data.u8[2]  & B00011111) << 6); //DI_current : 10|11@1+ (1,0) [0|2047] "A"
        int DI_kilowatts = ((DI_voltage * .5) * DI_current)/1000;
        Serial2.print("b1.txt=\""); Serial2.print(DI_kilowatts); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF");
        //Serial2.print("b2.txt=\""); Serial2.print(DI_current); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF");
        Serial2.print("h1.val="); Serial2.print(100 + DI_kilowatts); Serial2.write("\xFF\xFF\xFF");
      }//end 0x126 (294) DI_hvBusStatus

      if (rx_frame.MsgID == 0x2B3){ //0x2B3 (691) VCRIGHT_logging1Hz
        if((rx_frame.data.u8[0] & B00001111) == 1){ //VCRIGHT_cabinTempInterior m1 : 24|8@1+ (0.5,-40) [-40|80] "C" Index 0 "HVAC_TEMP_SENSORS_AND_ESTIMATES_1" 1 "HVAC_TEMP_SENSORS_AND_ESTIMATES_2" 2 "HVAC_HUMIDITY_HEATER" 3 "HVAC_STATUS_AIRFLOW" 4 "HVAC_COMFORT_SOLAR" 5 "HEATER_AND_DUCT_TARGETS" 6 "HVAC_ACTUATOR_VOLTAGES" 7 "HVAC_ACTUATOR_ENDSTOP" 8 "HVAC_ACTUATOR_ZEROSTOP" 9 "HVAC_MISCELLANEOUS_1" 10 "HVAC_MISCELLANEOUS_2" 11 "END";
        byte VCRIGHT_cabinTempInterior = rx_frame.data.u8[3];
        Serial2.print("b5.txt=\""); Serial2.print(((VCRIGHT_cabinTempInterior*.5)-40),0); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF");
        }//end VCRIGHT_cabinTempInterior m1
      }//end 0x2B3 (691) VCRIGHT_logging1Hz

      if (rx_frame.MsgID == 0x273){ //627 ID273UI_vehicleControl
      byte displayBrightnessLevel = (rx_frame.data.u8[4])/2;//32|8@1+ (0.5,0) [0|127] "%"
      Serial2.print("dim="); Serial2.print(brightness); Serial2.write("\xFF\xFF\xFF"); //set screen brightness
      //Serial2.print("b30.txt=\" "); Serial2.print(displayBrightnessLevel/2); Serial2.print("%\""); Serial2.write("\xFF\xFF\xFF");
      }

    }// end //read can frame data
  }//end Receive next CAN frame from queue
}//end page 1