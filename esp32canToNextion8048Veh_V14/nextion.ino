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
    if (componentNumber==6) //OTA button
      {setupOTA("FTINextionVehicleCan", "ssid", "pw");
      Serial2.print("b5.bco=65535"); Serial2.write("\xFF\xFF\xFF");
      Serial2.print("b5.pco=63488"); Serial2.write("\xFF\xFF\xFF");
      page5();
      menu=5;
      }
    //to page 1 button press
    if (componentNumber==1) 
      {Serial2.print("page 1"); Serial2.write("\xFF\xFF\xFF");
      menu=1;
      }
    pressEvent=0;
    }//end page 0 press events

  //page 1 buttons--------------------------------------------------------------------------------------------------------------------------------------------------------------------------    
  if ((pageNumber==1)&&(pressEvent==1)) //if component on page pressed
    {
    //To page 0 button press
    if (componentNumber==26) 
      {
      Serial2.print("page 0"); Serial2.write("\xFF\xFF\xFF");
      menu=0;
      Serial2.print("b1.txt=\" DI_limitRegenPower:"); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF");
      Serial2.print("b6.txt=\" DI_limitRegenTorque:"); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF");
      Serial2.print("b11.txt=\" DI_limitTCRegen:"); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF");
      Serial2.print("b16.txt=\" UI_regenTorqueMax:"); Serial2.print("%\""); Serial2.write("\xFF\xFF\xFF");
      Serial2.print("b21.txt=\" frontRideHeight:"); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF");
      Serial2.print("b26.txt=\" rearRideHeight:"); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF");
      Serial2.print("b31.txt=\" displayBrightnessLevel:"); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF");
      Serial2.print("b31.txt=\" OTA:"); Serial2.print(WiFi.localIP()); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF");

      Serial2.print("b3.txt=\" energyBuffer:"); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF"); 
        Serial2.print("b4.font=1");Serial2.write("\xFF\xFF\xFF");
      Serial2.print("b8.txt=\" EnergyRemaining:"); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF");
        Serial2.print("b9.font=1");Serial2.write("\xFF\xFF\xFF");
      Serial2.print("b13.txt=\" Range @300wh/km:"); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF");
        Serial2.print("b14.font=1");Serial2.write("\xFF\xFF\xFF");
      Serial2.print("b18.txt=\" :"); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF");
      Serial2.print("b23.txt=\" :"); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF");
      Serial2.print("b28.txt=\" :"); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF");
      Serial2.print("b33.txt=\" :"); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF");

      Serial2.print("b5.txt=\" OTA Set "); Serial2.print("\""); Serial2.write("\xFF\xFF\xFF");      
      }   
    //to page 2 button press
    if (componentNumber==7) 
      {

      }
    pressEvent=0;
    }//end page 1 press events


    
  }//end while serial available
}//end read from nex
