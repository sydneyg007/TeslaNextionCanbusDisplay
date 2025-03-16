void page1(){

CAN_frame_t rx_frame;   
  if (xQueueReceive(CAN_cfg.rx_queue, &rx_frame, 3 * portTICK_PERIOD_MS) == pdTRUE) {// Receive next CAN frame from queue
    for (int i = 0; i < rx_frame.FIR.B.DLC; i++) {//read can frame data
      if (rx_frame.MsgID == 0x145){//0x145 (325) ESP_status
        byte ESP_driverBrakeApply_update = (rx_frame.data.u8[3] & B01100000) >> 5; //29|2@1+ (1,0) [0|0]   0 "NotInit_orOff"(grey=3)   1 "Not_Applied"(black=5)   2 "Driver_applying_brakes"(green=6)   3 "Faulty_SNA"(red=4))
        if (ESP_driverBrakeApply_update != ESP_driverBrakeApply){//only print if changed
          ESP_driverBrakeApply = ESP_driverBrakeApply_update;
          //print ESP_driverBrakeApply
          if (ESP_driverBrakeApply==0)//Not initialised pedal colour grey=3
            {Serial2.print("pic 140,100,3"); Serial2.write("\xFF\xFF\xFF");} // show pic x, y, pic(id #)
          if (ESP_driverBrakeApply==1)//"Not_Applied"(black=5)
            {Serial2.print("pic 140,100,5"); Serial2.write("\xFF\xFF\xFF");} // show pic x, y, pic(id #)
          if (ESP_driverBrakeApply==2)//"Driver_applying_brakes"(green=6)
            {Serial2.print("pic 140,100,6"); Serial2.write("\xFF\xFF\xFF");} // show pic x, y, pic(id #)     
          if (ESP_driverBrakeApply==3)//"Faulty_SNA"(red=4))
            {Serial2.print("pic 140,100,4"); Serial2.write("\xFF\xFF\xFF");} // show pic x, y, pic(id #)     
        }//end ESP_driverBrakeApply print if changed
      byte ESP_brakeApply_update = (rx_frame.data.u8[3] & B10000000)>>7; //31|1@1+ (1,0) [0|0] ""  0 "INACTIVE" 1 "ACTIVE"

      //print ESP_brakeTorqueTarget
      unsigned int ESP_brakeTorqueTarget_update = (((rx_frame.data.u8[6] & B11111000)>>3) + (rx_frame.data.u8[7] << 5)) * 2; //51|13@1+ (2,0) [0|0] "Nm"    8191 "SNA";
      if (ESP_brakeTorqueTarget_update != ESP_brakeTorqueTarget){//only print if changed
        ESP_brakeTorqueTarget=ESP_brakeTorqueTarget_update;
        Serial2.print("b6.txt=\""); Serial2.print(ESP_brakeTorqueTarget); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF");
        }//end //print ESP_brakeTorqueTarget
      }//end 0x145 frames

    if (rx_frame.MsgID == 0x155){ //0x155 (341) ESP_wheelRotation
      byte ESP_wheelRotationQF_update = rx_frame.data.u8[5] & B00000001; //(mask left 7 bits) 40|1@1+ (1,0) [0|1] ""  Receiver                      1=valid, 0=invalid
      //print ESP_wheelRotationQF
      if (ESP_wheelRotationQF_update != ESP_wheelRotationQF){//print ESP_wheelRotationQF if changed
        ESP_wheelRotationQF=ESP_wheelRotationQF_update;
        if (ESP_wheelRotationQF==1){Serial2.print("b5.bco=1024");} else {Serial2.print("b5.bco=63488");} Serial2.write("\xFF\xFF\xFF");
        }//end print ESP_wheelRotationQF if changed
      //print ESP_standstillStatus
      byte ESP_standstillStatus = (rx_frame.data.u8[5] & B00000010) >> 1; //(mask left 6 bits and right 1 bit) 41|1@1+ (1,0) [0|1] ""  Receiver      1=standstill, 0=rolling
      if (ESP_standstillStatus==1){Serial2.print("b0.bco=56628");} if (ESP_standstillStatus==0){Serial2.print("b0.bco=46710");}
      Serial2.write("\xFF\xFF\xFF");
      //print ESP_vehicleSpeed
      unsigned int ESP_vehicleSpeed = ((rx_frame.data.u8[5] & B11111100) >> 2) + ((rx_frame.data.u8[6] & B00001111) << 4) ; //42|10@1+ (0.5,0) [0|511] "kph"                               1023 "SNA"
      Serial2.print("b0.txt=\""); Serial2.print((ESP_vehicleSpeed * .5),1); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF");
      }//end //0x155 (341) ESP_wheelRotation

    if (rx_frame.MsgID == 0x175){ //0x175 (373) ESP_wheelSpeed  
      //print ESP_wheelSpeedFL
      unsigned int ESP_wheelSpeedFL = rx_frame.data.u8[0] + ((rx_frame.data.u8[1] & B00011111)<<8);                                               //0|13@1+  (0.04,0) [0|327.64] "RPM" X
      Serial2.print("b1.txt=\""); Serial2.print((ESP_wheelSpeedFL * .04),2); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF");  
      //print ESP_wheelSpeedFR
      unsigned int ESP_wheelSpeedFR = ((rx_frame.data.u8[1] & B11100000) >> 5) + (rx_frame.data.u8[2] << 3) + ((rx_frame.data.u8[3] & B00000011) << 11);   //13|13@1+ (0.04,0) [0|327.64] "RPM" X
      Serial2.print("b2.txt=\""); Serial2.print((ESP_wheelSpeedFR * .04),2); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF"); 
      //print ESP_wheelSpeedRL
      unsigned int ESP_wheelSpeedRL = ((rx_frame.data.u8[3] & B11111100) >> 2) + ((rx_frame.data.u8[4] & B01111111) << 6);                        //26|13@1+ (0.04,0) [0|327.64] "RPM" X
      Serial2.print("b3.txt=\""); Serial2.print((ESP_wheelSpeedRL * .04),2); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF"); 
      //print ESP_wheelSpeedRR
      unsigned int ESP_wheelSpeedRR = ((rx_frame.data.u8[4] & B10000000) >> 7) + (rx_frame.data.u8[5] <<1) + ((rx_frame.data.u8[6] & B00001111) << 9);     //39|13@1+ (0.04,0) [0|327.64] "RPM" X
      Serial2.print("b4.txt=\""); Serial2.print((ESP_wheelSpeedRR * .04),2); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF");
      }//end 0x175 (373) ESP_wheelSpeed 

    if (rx_frame.MsgID == 0x185){ //0x185 (389) ESP_brakeTorque
      //print ESP_brakeTorqueFL
      unsigned int ESP_brakeTorqueFL = rx_frame.data.u8[0] + ((rx_frame.data.u8[1] & B00001111) << 8);          //0|12@1+ (2,0) [0|8190] "Nm"
      Serial2.print("b7.txt=\""); Serial2.print(ESP_brakeTorqueFL * 2); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF"); 
      //print ESP_brakeTorqueFR
      unsigned int ESP_brakeTorqueFR = ((rx_frame.data.u8[1] & B11110000) >> 4) + (rx_frame.data.u8[2] << 4);   //12|12@1+ (2,0) [0|8190] "Nm"
      Serial2.print("b8.txt=\""); Serial2.print(ESP_brakeTorqueFR * 2); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF");  
      //print ESP_brakeTorqueRL
      unsigned int ESP_brakeTorqueRL = rx_frame.data.u8[3] + ((rx_frame.data.u8[4] & B00001111) << 8);          //24|12@1+ (2,0) [0|8190] "Nm"
      Serial2.print("b9.txt=\""); Serial2.print(ESP_brakeTorqueRL * 2); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF");   
      //print ESP_brakeTorqueRR
      unsigned int ESP_brakeTorqueRR = ((rx_frame.data.u8[4] & B11110000) >> 4) + (rx_frame.data.u8[5] << 4);   //36|12@1+ (2,0) [0|8190] "Nm"
      Serial2.print("b10.txt=\""); Serial2.print(ESP_brakeTorqueRR * 2); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF"); 
      }//end 0x185 (389) ESP_brakeTorque
      
    }// end //read can frame data
  }//end Receive next CAN frame from queue
}//end page 1