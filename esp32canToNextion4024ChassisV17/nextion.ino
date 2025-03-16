void readfromNex() 
{
int array[17];
while (Serial2.available() > 0) 
  {
  for (int i = 1 ; i < 17; i++) 
    {
    array[i] =  Serial2.read();
    //Serial.println(array[i]);
    delay(20);
    }
  array[17] = '\0';
  byte pageNumber = array[2];
  byte componentNumber = array[3];
  byte pressEvent = array[4];
  byte numericalData = array[9];

  //page 0 buttons--------------------------------------------------------------------------------------------------------------------------------------------------------------------------    
  if ((pageNumber==0)&&(pressEvent==1)) //if component on page pressed
    {
    if (componentNumber==20) //OTA button
      {setupOTA("FTINextionChassisCan", "this", "Polkiu09");
      Serial2.print("b19.bco=65535"); Serial2.write("\xFF\xFF\xFF");
      Serial2.print("b19.pco=63488"); Serial2.write("\xFF\xFF\xFF");
      page5();
      menu=5;
      }

    //to page 1 button press
    if (componentNumber==1) 
      {
      Serial2.print("page 1"); Serial2.write("\xFF\xFF\xFF");
      menu=1;
      }
    if (componentNumber==3) 
      {
      debugMenu++;

      if(debugMenu==3){debugMenu=1;}
      Serial2.print("b0.txt=\"DI_axleSpeedraw: \""); Serial2.write("\xFF\xFF\xFF");
      Serial2.print("b10.txt=\"    : \""); Serial2.write("\xFF\xFF\xFF");
      Serial2.print("b12.txt=\"    : \""); Serial2.write("\xFF\xFF\xFF");
      Serial2.print("b13.txt=\"ESP_wheelSpeedFR: \""); Serial2.write("\xFF\xFF\xFF");
      Serial2.print("b14.txt=\"ESP_wheelSpeedRL: \""); Serial2.write("\xFF\xFF\xFF");
      Serial2.print("b15.txt=\"ESP_wheelSpeedRR: \""); Serial2.write("\xFF\xFF\xFF");
      Serial2.print("b16.txt=\"ESP_wheelSpeedFL: \""); Serial2.write("\xFF\xFF\xFF");
      Serial2.print("b17.txt=\"ESP_vehicleSpeed: \""); Serial2.write("\xFF\xFF\xFF");
      Serial2.print("b18.txt=\"ESP_standstillStatus: \""); Serial2.write("\xFF\xFF\xFF");
      Serial2.print("b19.txt=\" OTA:  "); Serial2.print(WiFi.localIP()); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF");
      //clear data fields
      Serial2.print("b1.txt=\"  >>\""); Serial2.write("\xFF\xFF\xFF");
      Serial2.print("b2.txt=\"\""); Serial2.write("\xFF\xFF\xFF");
      Serial2.print("b3.txt=\"\""); Serial2.write("\xFF\xFF\xFF");
      Serial2.print("b4.txt=\"\""); Serial2.write("\xFF\xFF\xFF");
      Serial2.print("b5.txt=\"\""); Serial2.write("\xFF\xFF\xFF");
      Serial2.print("b6.txt=\"\""); Serial2.write("\xFF\xFF\xFF");
      Serial2.print("b7.txt=\"\""); Serial2.write("\xFF\xFF\xFF");
      Serial2.print("b8.txt=\"\""); Serial2.write("\xFF\xFF\xFF");
      Serial2.print("b9.txt=\"\""); Serial2.write("\xFF\xFF\xFF");
      }

      if(debugMenu==2){
      Serial2.print("b0.txt=\"ESP_wheelAngleFL: \""); Serial2.write("\xFF\xFF\xFF");
      Serial2.print("b10.txt=\"ESP_wheelAngleFR: \""); Serial2.write("\xFF\xFF\xFF");
      Serial2.print("b12.txt=\"ESP_wheelAngleRL: \""); Serial2.write("\xFF\xFF\xFF");
      Serial2.print("b13.txt=\"ESP_wheelAngleRR: \""); Serial2.write("\xFF\xFF\xFF");
      Serial2.print("b14.txt=\"ESP_wheelRotationRR: \""); Serial2.write("\xFF\xFF\xFF");
      Serial2.print("b15.txt=\"ESP_wheelRotationRL: \""); Serial2.write("\xFF\xFF\xFF");
      Serial2.print("b16.txt=\"ESP_wheelRotationFR: \""); Serial2.write("\xFF\xFF\xFF");
      Serial2.print("b17.txt=\"ESP_wheelRotationFL: \""); Serial2.write("\xFF\xFF\xFF");  
      Serial2.print("b18.txt=\"ESP_wheelRotationQF: \""); Serial2.write("\xFF\xFF\xFF");        
      Serial2.print("b19.txt=\" ");  Serial2.print("\""); Serial2.write("\xFF\xFF\xFF");
      //clear data fields
      Serial2.print("b1.txt=\"  >>\""); Serial2.write("\xFF\xFF\xFF");
      Serial2.print("b2.txt=\"\""); Serial2.write("\xFF\xFF\xFF");
      Serial2.print("b3.txt=\"\""); Serial2.write("\xFF\xFF\xFF");
      Serial2.print("b4.txt=\"\""); Serial2.write("\xFF\xFF\xFF");
      Serial2.print("b5.txt=\"\""); Serial2.write("\xFF\xFF\xFF");
      Serial2.print("b6.txt=\"\""); Serial2.write("\xFF\xFF\xFF");
      Serial2.print("b7.txt=\"\""); Serial2.write("\xFF\xFF\xFF");
      Serial2.print("b8.txt=\"\""); Serial2.write("\xFF\xFF\xFF");
      Serial2.print("b9.txt=\"\""); Serial2.write("\xFF\xFF\xFF");
      }

 pressEvent=0;
    }//end page 0 press events

  //page 1 buttons--------------------------------------------------------------------------------------------------------------------------------------------------------------------------    
  if ((pageNumber==1)&&(pressEvent==1)) //if component on page pressed
    {
    //To page 0 button press
    if (componentNumber==21) 
      {
      Serial2.print("page 0"); Serial2.write("\xFF\xFF\xFF");
      menu=0;
      Serial2.print("b0.txt=\"DI_axleSpeedraw: \""); Serial2.write("\xFF\xFF\xFF");
      Serial2.print("b10.txt=\"    : \""); Serial2.write("\xFF\xFF\xFF");
      Serial2.print("b12.txt=\"    : \""); Serial2.write("\xFF\xFF\xFF");
      Serial2.print("b13.txt=\"ESP_wheelSpeedFR: \""); Serial2.write("\xFF\xFF\xFF");
      Serial2.print("b14.txt=\"ESP_wheelSpeedRL: \""); Serial2.write("\xFF\xFF\xFF");
      Serial2.print("b15.txt=\"ESP_wheelSpeedRR: \""); Serial2.write("\xFF\xFF\xFF");
      Serial2.print("b16.txt=\"ESP_wheelSpeedFL: \""); Serial2.write("\xFF\xFF\xFF");
      Serial2.print("b17.txt=\"ESP_vehicleSpeed: \""); Serial2.write("\xFF\xFF\xFF");
      Serial2.print("b18.txt=\"ESP_standstillStatus: \""); Serial2.write("\xFF\xFF\xFF");
      Serial2.print("b19.txt=\" OTA:  "); Serial2.print(WiFi.localIP()); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF");
      //clear data fields
      Serial2.print("b1.txt=\"  >>\""); Serial2.write("\xFF\xFF\xFF");
      Serial2.print("b2.txt=\"\""); Serial2.write("\xFF\xFF\xFF");
      Serial2.print("b3.txt=\"\""); Serial2.write("\xFF\xFF\xFF");
      Serial2.print("b4.txt=\"\""); Serial2.write("\xFF\xFF\xFF");
      Serial2.print("b5.txt=\"\""); Serial2.write("\xFF\xFF\xFF");
      Serial2.print("b6.txt=\"\""); Serial2.write("\xFF\xFF\xFF");
      Serial2.print("b7.txt=\"\""); Serial2.write("\xFF\xFF\xFF");
      Serial2.print("b8.txt=\"\""); Serial2.write("\xFF\xFF\xFF");
      Serial2.print("b9.txt=\"\""); Serial2.write("\xFF\xFF\xFF");
      } 
            
    //to page 2 button press
    if (componentNumber==20) 
      {
      Serial2.print("page 2"); Serial2.write("\xFF\xFF\xFF");
      menu=2;
      }

    pressEvent=0;
    }//end page 1 press events

  //page 2 buttons-------------------------------------------------------------------------------------------------------------------------------------------------------------------------- 
  if ((pageNumber==2)&&(pressEvent==1)) //if component on page pressed
    {
    //to page 3 button press
    if (componentNumber==28) 
      {
      Serial2.print("page 3"); Serial2.write("\xFF\xFF\xFF");
      menu=3;
      }
    //back to page 1 button press
    if (componentNumber==29) 
      {
      Serial2.print("page 1"); Serial2.write("\xFF\xFF\xFF");
      menu=1;
      }
    pressEvent=0;
    }//end page 1 press events

  //page 3 buttons-------------------------------------------------------------------------------------------------------------------------------------------------------------------------- 
  if ((pageNumber==3)&&(pressEvent==1)) //if component on page pressed
    {
    //back to page 2 button press
    if (componentNumber==30) 
      {
      Serial2.print("page 2"); Serial2.write("\xFF\xFF\xFF");
      menu=2;
      }
    //to page 4 button press
    if (componentNumber==31) 
      {
      Serial2.print("page 4"); Serial2.write("\xFF\xFF\xFF");
      menu=4;
      }

    pressEvent=0;
    }//end page 3 press events

  //page 4 buttons-------------------------------------------------------------------------------------------------------------------------------------------------------------------------- 
  if ((pageNumber==4)&&(pressEvent==1)) //if component on page pressed
    {
    //back to page 3 button press
    if (componentNumber==6) 
      {
      Serial2.print("page 3"); Serial2.write("\xFF\xFF\xFF");
      menu=3;
      }
    //to page 4 button press
    if (componentNumber==7) 
      {
      Serial2.print("page 0"); Serial2.write("\xFF\xFF\xFF");
      menu=0;
      }

    pressEvent=0;
    }//end page 3 press events
    
  }//end while serial available
}//end read from nex