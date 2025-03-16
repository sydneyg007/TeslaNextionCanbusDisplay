void page4(){

CAN_frame_t rx_frame;   
  if (xQueueReceive(CAN_cfg.rx_queue, &rx_frame, 3 * portTICK_PERIOD_MS) == pdTRUE) {// Receive next CAN frame from queue
    for (int i = 0; i < rx_frame.FIR.B.DLC; i++) {//read can frame data

      if (rx_frame.MsgID == 0x257){ //0x257 (599) DI_speed
        unsigned int DI_vehicleSpeed = ((rx_frame.data.u8[1]  & B11110000) >> 4) + (rx_frame.data.u8[2] << 4); //DI_vehicleSpeed: 12|12@1+ (0.08,-40) [0|0] "kph" X
        Serial2.print("b0.txt=\""); Serial2.print(((DI_vehicleSpeed*.08)-40)*.876,0); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF");   //corrected vehcicle speed
      }//end 0x257 (599) DI_speed

      if (rx_frame.MsgID == 0x3f3){//0x3f3 (1011) UI_odo       
        unsigned int odo = rx_frame.data.u8[0] + (rx_frame.data.u8[1]<<8) + (rx_frame.data.u8[2]<<16); //UI_odometer: 0|24@1+ (0.1,0) [0|0] "km"
        //print to nextion
        Serial2.print("b2.txt=\""); Serial2.print((((odo*.88)*.1) + 249938),0); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF"); //tesla odometer scaled plus landy original kms 
      }//end 0x3f3 (1011) UI_odo
      
    }// end //read can frame data
  }//end Receive next CAN frame from queue
}//end page 4