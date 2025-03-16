void page0(){

//ArduinoOTA.handle();

CAN_frame_t rx_frame;

if (xQueueReceive(CAN_cfg.rx_queue, &rx_frame, 3 * portTICK_PERIOD_MS) == pdTRUE) {// Receive next CAN frame from queue
  for (int i = 0; i < rx_frame.FIR.B.DLC; i++) {//read can frame data

    if (debugMenu==1){
      if (rx_frame.MsgID == 0x108){//0x108 DI_torque
        DI_axleSpeedraw = rx_frame.data.u8[5] + (rx_frame.data.u8[6]<<8); //*.1
        if ((DI_axleSpeedraw != 65535) && (DI_axleSpeedraw < 32768)){D_esp_wheelspeedraw = DI_axleSpeedraw/3.2;} //going forwards
        if ((DI_axleSpeedraw != 65535) && (DI_axleSpeedraw > 32768)){D_esp_wheelspeedraw = (65535 - DI_axleSpeedraw)/3.2;} //going backwards
        Serial2.print("b1.txt=\""); Serial2.print(DI_axleSpeedraw); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF");
        //Serial2.print("b2.txt=\""); Serial2.print(DI_axleSpeedraw/10); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF"); //DI_axleSpeedraw (rpm calculated)     
        //Serial2.print("b3.txt=\""); Serial2.print(D_esp_wheelspeedraw); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF"); //Derived esp_wheelspeed (raw)   
        }
      if (rx_frame.MsgID == 0x175){ //0x175 (373) ESP_wheelSpeed
        //ESP_wheelSpeedFR
        ESP_wheelSpeedFR = ((rx_frame.data.u8[1] & B11100000) >> 5) + (rx_frame.data.u8[2] << 3) + ((rx_frame.data.u8[3] & B00000011) << 11);   //13|13@1+ (0.04,0) [0|327.64] "RPM" X
        Serial2.print("b4.txt=\""); Serial2.print(ESP_wheelSpeedFR/25); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF");
        //ESP_wheelSpeedRL
        ESP_wheelSpeedRL = ((rx_frame.data.u8[3] & B11111100) >> 2) + ((rx_frame.data.u8[4] & B01111111) << 6);                        //26|13@1+ (0.04,0) [0|327.64] "RPM" X
        Serial2.print("b5.txt=\""); Serial2.print(ESP_wheelSpeedRL/25); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF");
        //print ESP_wheelSpeedRR
        ESP_wheelSpeedRR = ((rx_frame.data.u8[4] & B10000000) >> 7) + (rx_frame.data.u8[5] <<1) + ((rx_frame.data.u8[6] & B00001111) << 9);// : 39|13@1+ (0.04,0) [0|327.64] "RPM" 8191 "SNA";
        Serial2.print("b6.txt=\""); Serial2.print(ESP_wheelSpeedRR/25); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF");
        ESP_wheelSpeedFL = rx_frame.data.u8[0] + (rx_frame.data.u8[1] & B00011111);// : 0|13@1+ (0.04,0) [0|327.64] "RPM" 8191 "SNA";
        Serial2.print("b7.txt=\""); Serial2.print(ESP_wheelSpeedFL/25); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF");
        }//end 0x175 (373) ESP_wheelSpeed 
      if (rx_frame.MsgID == 0x155){ 
        ESP_standstillStatus = (rx_frame.data.u8[5] & B00000010)>>1;// : 41|1@1+ (1,0) [0|1] ""     1 "STANDSTILL" 0 "ROLLING" ;
        Serial2.print("b9.txt=\""); if(ESP_standstillStatus==1){Serial2.print("Standstill");} if(ESP_standstillStatus==0){Serial2.print("Rolling");} Serial2.print("\""); Serial2.write("\xFF\xFF\xFF"); 
        ESP_vehicleSpeed = ((rx_frame.data.u8[5] & B11111100)>>2) + (rx_frame.data.u8[6] & B00001111) ;// : 42|10@1+ (0.5,0) [0|511] "kph" 1023 "SNA" ;
        Serial2.print("b8.txt=\""); if((ESP_vehicleSpeed/2)== 1023){Serial2.print("SNA");} else {Serial2.print(ESP_vehicleSpeed/2);} Serial2.print("\""); Serial2.write("\xFF\xFF\xFF");
        }//end 0x155 messages
      }//end debug menu 1 messages

    if (debugMenu==2){
      if (rx_frame.MsgID == 0x155){ 
        ESP_wheelAngleFL = rx_frame.data.u8[0];//: 0|8@1+ (1.40625,0) [0|358.59375] "deg" X  
        Serial2.print("b1.txt=\""); Serial2.print(ESP_wheelAngleFL); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF");
        
        ESP_wheelAngleFR = rx_frame.data.u8[1];//: 8|8@1+ (1.40625,0) [0|358.59375] "deg" X
        Serial2.print("b2.txt=\""); Serial2.print(ESP_wheelAngleFR); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF");
        
        ESP_wheelAngleRL = rx_frame.data.u8[2];//: 16|8@1+ (1.40625,0) [0|358.59375] "deg" X
        Serial2.print("b3.txt=\""); Serial2.print(ESP_wheelAngleRL); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF");
        
        ESP_wheelAngleRR = rx_frame.data.u8[3];//: 24|8@1+ (1.40625,0) [0|358.59375] "deg" X
        Serial2.print("b4.txt=\""); Serial2.print(ESP_wheelAngleRR); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF"); 
        
        ESP_wheelRotationRR = rx_frame.data.u8[4] & B00000011;//: 32|2@1+ (1,0) [0|3] ""       3 "UNKNOWN" 2 "STANDSTILL" 1 "BACKWARD" 0 "FORWARD" ;
        Serial2.print("b5.txt=\""); 
        if (ESP_wheelRotationRR == 0 ){Serial2.print("Forward");} if (ESP_wheelRotationRR == 1){Serial2.print("Backward");} if (ESP_wheelRotationRR == 2){Serial2.print("Standstill");} if (ESP_wheelRotationRR == 3){Serial2.print("Unknown");}
        Serial2.print("\""); Serial2.write("\xFF\xFF\xFF");
        
        ESP_wheelRotationRL = (rx_frame.data.u8[4] & B00001100)>>2;//: 34|2@1+ (1,0) [0|3] ""       3 "UNKNOWN" 2 "STANDSTILL" 1 "BACKWARD" 0 "FORWARD" ;
        Serial2.print("b6.txt=\""); 
        if (ESP_wheelRotationRL == 0 ){Serial2.print("Forward");} if (ESP_wheelRotationRL == 1){Serial2.print("Backward");} if (ESP_wheelRotationRL == 2){Serial2.print("Standstill");} if (ESP_wheelRotationRL == 3){Serial2.print("Unknown");}
        Serial2.print("\""); Serial2.write("\xFF\xFF\xFF");
        
        ESP_wheelRotationFR = (rx_frame.data.u8[4] & B00110000)>>4;//: 36|2@1+ (1,0) [0|3] ""       3 "UNKNOWN" 2 "STANDSTILL" 1 "BACKWARD" 0 "FORWARD" ;
        Serial2.print("b7.txt=\""); 
        if (ESP_wheelRotationFR == 0 ){Serial2.print("Forward");} if (ESP_wheelRotationFR == 1){Serial2.print("Backward");} if (ESP_wheelRotationFR == 2){Serial2.print("Standstill");} if (ESP_wheelRotationFR == 3){Serial2.print("Unknown");}
        Serial2.print("\""); Serial2.write("\xFF\xFF\xFF");
        
        ESP_wheelRotationFL = (rx_frame.data.u8[4] & B11000000)>>6;//: 38|2@1+ (1,0) [0|3] ""       3 "UNKNOWN" 2 "STANDSTILL" 1 "BACKWARD" 0 "FORWARD" ;
        Serial2.print("b8.txt=\""); 
        if (ESP_wheelRotationFL == 0 ){Serial2.print("Forward");} if (ESP_wheelRotationFL == 1){Serial2.print("Backward");} if (ESP_wheelRotationFL == 2){Serial2.print("Standstill");} if (ESP_wheelRotationFL == 3){Serial2.print("Unknown");}
        Serial2.print("\""); Serial2.write("\xFF\xFF\xFF");
        
        ESP_wheelRotationQF = rx_frame.data.u8[5] & B00000001;//: 40|1@1+ (1,0) [0|1] ""       1 "VALID" 0 "INVALID" ;
        Serial2.print("b9.txt=\""); 
        if (ESP_wheelRotationQF == 0 ){Serial2.print("Invalid");} if (ESP_wheelRotationQF == 1 ){Serial2.print("Valid");}
        Serial2.print("\""); Serial2.write("\xFF\xFF\xFF");
        }//end 0x155 messages
      }//end debug menu 2 messages

    }// end //read can frame data
  }//end Receive next CAN frame from queue
}//end page0