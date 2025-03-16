void page3(){//parking sensors
CAN_frame_t rx_frame;   
  if (xQueueReceive(CAN_cfg.rx_queue, &rx_frame, 3 * portTICK_PERIOD_MS) == pdTRUE) {// Receive next CAN frame from queue
    for (int i = 0; i < rx_frame.FIR.B.DLC; i++) {//read can frame data
      //front park sensors
      if (rx_frame.MsgID == 0x20E){//0x20E (526) PARK_sdiFront                          0 "BLOCKED" 1 "NEAR_DETECTION" 500 "NO_OBJECT_DETECTED" 511 "SNA";
        unsigned int PARK_sdiSensor1RawDistData_update = rx_frame.data.u8[0] + ((rx_frame.data.u8[1] & B00000001)<<8); //0|9@1+ (1,0) [0|0] "cm"
        if (PARK_sdiSensor1RawDistData_update != PARK_sdiSensor1RawDistData){//only print if changed
          PARK_sdiSensor1RawDistData = PARK_sdiSensor1RawDistData_update;
          //print to nextion
        Serial2.print("b12.txt=\""); Serial2.print(PARK_sdiSensor1RawDistData); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF");  
        }//end print if changed
        unsigned int PARK_sdiSensor2RawDistData_update = ((rx_frame.data.u8[1] & B11111110)>>1) + ((rx_frame.data.u8[2] & B00000011)<<7); //9|9@1+ (1,0) [0|0] "cm"
        if (PARK_sdiSensor2RawDistData_update != PARK_sdiSensor2RawDistData){//only print if changed
          PARK_sdiSensor2RawDistData = PARK_sdiSensor2RawDistData_update;
          //print to nextion
        Serial2.print("b13.txt=\""); Serial2.print(PARK_sdiSensor2RawDistData); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF");  
        }//end print if changed
        unsigned int PARK_sdiSensor3RawDistData_update = ((rx_frame.data.u8[2] & B11111100)>>2) + ((rx_frame.data.u8[3] & B00000111)<<6); //18|9@1+ (1,0) [0|0] "cm"
        if (PARK_sdiSensor3RawDistData_update != PARK_sdiSensor3RawDistData){//only print if changed
          PARK_sdiSensor3RawDistData = PARK_sdiSensor3RawDistData_update;
          //print to nextion
        Serial2.print("b14.txt=\""); Serial2.print(PARK_sdiSensor3RawDistData); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF");  
        }//end print if changed
        unsigned int PARK_sdiSensor4RawDistData_update = ((rx_frame.data.u8[3] & B11111000)>>3) + ((rx_frame.data.u8[4] & B00001111)<<5); //27|9@1+ (1,0) [0|0] "cm"
        if (PARK_sdiSensor4RawDistData_update != PARK_sdiSensor4RawDistData){//only print if changed
          PARK_sdiSensor4RawDistData = PARK_sdiSensor4RawDistData_update;
          //print to nextion
        Serial2.print("b15.txt=\""); Serial2.print(PARK_sdiSensor4RawDistData); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF");  
        }//end print if changed
        unsigned int PARK_sdiSensor5RawDistData_update = ((rx_frame.data.u8[4] & B11110000)>>4) + ((rx_frame.data.u8[5] & B00011111)<<4); //36|9@1+ (1,0) [0|0] "cm"
        if (PARK_sdiSensor5RawDistData_update != PARK_sdiSensor5RawDistData){//only print if changed
          PARK_sdiSensor5RawDistData = PARK_sdiSensor5RawDistData_update;
          //print to nextion
        Serial2.print("b16.txt=\""); Serial2.print(PARK_sdiSensor5RawDistData); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF");  
        }//end print if changed
        unsigned int PARK_sdiSensor6RawDistData_update = ((rx_frame.data.u8[5] & B11100000)>>5) + ((rx_frame.data.u8[6] & B00111111)<<3); //45|9@1+ (1,0) [0|0] "cm"
        if (PARK_sdiSensor6RawDistData_update != PARK_sdiSensor6RawDistData){//only print if changed
          PARK_sdiSensor6RawDistData = PARK_sdiSensor6RawDistData_update;
          //print to nextion
        Serial2.print("b17.txt=\""); Serial2.print(PARK_sdiSensor6RawDistData); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF");  
        }//end print if changed
      }//end //0x20E (526) PARK_sdiFront  
      
      //rear park sensors
      if (rx_frame.MsgID == 0x22E){//0x22E (558) PARK_sdiRear                         0 "BLOCKED" 1 "NEAR_DETECTION" 500 "NO_OBJECT_DETECTED" 511 "SNA";
        unsigned int PARK_sdiSensor7RawDistData_update = rx_frame.data.u8[0] + ((rx_frame.data.u8[1] & B00000001)<<8); //0|9@1+ (1,0) [0|0] "cm"
        if (PARK_sdiSensor7RawDistData_update != PARK_sdiSensor7RawDistData){//only print if changed
          PARK_sdiSensor7RawDistData = PARK_sdiSensor7RawDistData_update;
          //print to nextion
        Serial2.print("b18.txt=\""); Serial2.print(PARK_sdiSensor7RawDistData); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF");  
        }//end print if changed
        unsigned int PARK_sdiSensor8RawDistData_update = ((rx_frame.data.u8[1] & B11111110)>>1) + ((rx_frame.data.u8[2] & B00000011)<<7); //9|9@1+ (1,0) [0|0] "cm"
        if (PARK_sdiSensor8RawDistData_update != PARK_sdiSensor8RawDistData){//only print if changed
          PARK_sdiSensor8RawDistData = PARK_sdiSensor8RawDistData_update;
          //print to nextion
        Serial2.print("b19.txt=\""); Serial2.print(PARK_sdiSensor8RawDistData); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF");  
        }//end print if changed
        unsigned int PARK_sdiSensor9RawDistData_update = ((rx_frame.data.u8[2] & B11111100)>>2) + ((rx_frame.data.u8[3] & B00000111)<<6); //18|9@1+ (1,0) [0|0] "cm"
        if (PARK_sdiSensor9RawDistData_update != PARK_sdiSensor9RawDistData){//only print if changed
          PARK_sdiSensor9RawDistData = PARK_sdiSensor9RawDistData_update;
          //print to nextion
        Serial2.print("b20.txt=\""); Serial2.print(PARK_sdiSensor9RawDistData); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF");  
        }//end print if changed
        unsigned int PARK_sdiSensor10RawDistData_update = ((rx_frame.data.u8[3] & B11111000)>>3) + ((rx_frame.data.u8[4] & B00001111)<<5); //27|9@1+ (1,0) [0|0] "cm"
        if (PARK_sdiSensor10RawDistData_update != PARK_sdiSensor10RawDistData){//only print if changed
          PARK_sdiSensor10RawDistData = PARK_sdiSensor10RawDistData_update;
          //print to nextion
        Serial2.print("b21.txt=\""); Serial2.print(PARK_sdiSensor10RawDistData); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF");  
        }//end print if changed
        unsigned int PARK_sdiSensor11RawDistData_update = ((rx_frame.data.u8[4] & B11110000)>>4) + ((rx_frame.data.u8[5] & B00011111)<<4); //36|9@1+ (1,0) [0|0] "cm"
        if (PARK_sdiSensor11RawDistData_update != PARK_sdiSensor11RawDistData){//only print if changed
          PARK_sdiSensor11RawDistData = PARK_sdiSensor11RawDistData_update;
          //print to nextion
        Serial2.print("b22.txt=\""); Serial2.print(PARK_sdiSensor11RawDistData); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF");  
        }//end print if changed
        unsigned int PARK_sdiSensor12RawDistData_update = ((rx_frame.data.u8[5] & B11100000)>>5) + ((rx_frame.data.u8[6] & B00111111)<<3); //45|9@1+ (1,0) [0|0] "cm"
        if (PARK_sdiSensor12RawDistData_update != PARK_sdiSensor12RawDistData){//only print if changed
          PARK_sdiSensor12RawDistData = PARK_sdiSensor12RawDistData_update;
          //print to nextion
        Serial2.print("b23.txt=\""); Serial2.print(PARK_sdiSensor12RawDistData); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF");  
        }//end print if changed
      }//end 0x22E (558) PARK_sdiRear   
    }// end //read can frame data
  }//end Receive next CAN frame from queue
}//end page 3

