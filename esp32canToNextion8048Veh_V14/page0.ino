void page0(){

CAN_frame_t rx_frame;

if (xQueueReceive(CAN_cfg.rx_queue, &rx_frame, 3 * portTICK_PERIOD_MS) == pdTRUE) {// Receive next CAN frame from queue
  for (int i = 0; i < rx_frame.FIR.B.DLC; i++) {//read can frame data

      if (rx_frame.MsgID == 0x1D6){ //0x1D6 (470) ID1D6DI_limits
        byte DI_limitRegenPower = ((rx_frame.data.u8[0] & B10000000) >> 7); // 7|1@1+ (1,0) [0|1] ""  Receiver
        Serial2.print("b2.txt=\" "); Serial2.print(DI_limitRegenPower); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF");
        
        byte DI_limitRegenTorque = ((rx_frame.data.u8[1] & B00001000) >> 3); // 7|1@1+ (1,0) [0|1] ""  Receiver
        Serial2.print("b7.txt=\" "); Serial2.print(DI_limitRegenTorque); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF");
        
        byte DI_limitTCRegen = ((rx_frame.data.u8[1] & B00100000) >> 5); // 7|1@1+ (1,0) [0|1] ""  Receiver
        Serial2.print("b12.txt=\" "); Serial2.print(DI_limitTCRegen); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF");        
      }//end 0x1D6 (470) ID1D6DI_limits

      if (rx_frame.MsgID == 0x334){ //0x334 (820) ID334UI_powertrainControl
      byte UI_regenTorqueMax = (rx_frame.data.u8[3]); //24|8@1+ (0.5,0) [0|100] "%"  Receiver
      Serial2.print("b17.txt=\" "); Serial2.print(UI_regenTorqueMax/2); Serial2.print("%\""); Serial2.write("\xFF\xFF\xFF");   
      }//end 0x334 (820) ID334UI_powertrainControl

      if (rx_frame.MsgID == 0x3E2){ //994 ID3E2VCLEFT_lightStatus
      int frontRideHeight = ((rx_frame.data.u8[3] & B11110000)>>4) + (rx_frame.data.u8[4]& B00001111); //28|8@1- (1,0) [-127|127] "mm"
      Serial2.print("b22.txt=\" "); Serial2.print(frontRideHeight); Serial2.print("mm\""); Serial2.write("\xFF\xFF\xFF"); 

      int rearRideHeight = ((rx_frame.data.u8[4] & B11110000)>>4) + (rx_frame.data.u8[5]& B00001111); //36|8@1- (1,0) [-127|127] "mm"
      Serial2.print("b27.txt=\" "); Serial2.print(rearRideHeight); Serial2.print("mm\""); Serial2.write("\xFF\xFF\xFF");
      }//end 994 ID3E2VCLEFT_lightStatus

      if (rx_frame.MsgID == 0x273){ //627 ID273UI_vehicleControl
      byte displayBrightnessLevel = (rx_frame.data.u8[4]);//32|8@1+ (0.5,0) [0|127] "%"
      Serial2.print("b30.txt=\" "); Serial2.print(displayBrightnessLevel/2); Serial2.print("%\""); Serial2.write("\xFF\xFF\xFF");
      }

      if (rx_frame.MsgID == 0x352){ //850 ID352BMS_energyStatus
      byte energyBuffer = ((rx_frame.data.u8[6] & B10000000)>>7) + ((rx_frame.data.u8[7] & B01111111)<<1);//55|8@1+ (0.1,0) [0|25.4] "KWh"
      Serial2.print("b4.txt=\" "); Serial2.print((energyBuffer * .1),1); Serial2.print("kwh\""); Serial2.write("\xFF\xFF\xFF");

      int EnergyRemaining = ((rx_frame.data.u8[1] & B11111000)>>3) + ((rx_frame.data.u8[2] & B00111111)<<5);//11|11@1+ (0.1,0) [0|204.6] "KWh"
      Serial2.print("b9.txt=\" "); Serial2.print((EnergyRemaining * .1),1); Serial2.print("kwh\""); Serial2.write("\xFF\xFF\xFF");

      //range left if 300wh/km
      Serial2.print("b14.txt=\" "); Serial2.print(((EnergyRemaining*.1)/.3),1); Serial2.print("km\""); Serial2.write("\xFF\xFF\xFF");    

      }//end 850 ID352BMS_energyStatus
    }// end //read can frame data
  }//end Receive next CAN frame from queue


}//end page0