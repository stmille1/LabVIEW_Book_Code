

String Command;
String SubCommand;
String Data;

float Temp = 30;  //Current Temperature
float Noise = 0;  //Random noise in the simulated temperature measurement 
float PWR = 0;    //Current Set power of the external device.  Always start at zero when the arduino is first powered on
float PWRTemporary = 0;  //Temp power setting do ensure requested value is in range. 
float TempError = 0; //Depending on how far from the "correct" simulated temperature depending on applied power, this value will be added to the temperature during each loop iteration.
float TempC = 30; //Calculated "correct" simulated temperature 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.setTimeout(50);

}

void loop() {
  // put your main code here, to run repeatedly:

  Noise = (random(-100, 100)) / 1000.00;  //Generate random temperature noise during each loop iteration


  TempC = 0.66*PWR + 30;  //Calculate what the temperature should be based on the applied power.

  TempError = TempC - Temp;
  Temp = Temp + TempError/100 + Noise;


  //Check for Messages coming from LabVIEW
  Command = Serial.readString();
  SubCommand = Command.substring(0, 5);
  Data = Command.substring(5);

  if (Command.length() == 0) {  //If message is blank, do nothing 
  }

  else {      //If message is not blank, parse the message and respond appropriately.

    if (SubCommand == "*IDN?")   //If receive command "*IDN?" respond with the device identification information
    {
      Serial.print("Power Supply P100: S.Miller Industries: Serial# 012345\r");
    }

    else {
      if (SubCommand == "Temp?")   //If receive command "Temp?" respond with the current temperature
      {
        Serial.print(Temp);
        Serial.print("\r");
      }
      else {
        if (SubCommand == "Powr?")   //If receive command "Powr?" respond with the current power setting
        {
          Serial.print(PWR);
          Serial.print("\r");
        }

        else {
          PWRTemporary = Data.toFloat();
          if ((SubCommand == "Powr:") && (PWRTemporary <100.0001) && (PWRTemporary >-0.0001))   //If receive command "Powr:", we need to change the current power setting of the device but only if the request is in range (i.e. 0..100).
          {
           
            PWR = PWRTemporary;
            Serial.print("Power Set");
            Serial.print("\r");
          }
          else {
            Serial.print("Invalid Command");   //If an unrecognized command is received, ignore it and respond with an "Invalid Command" message.
            Serial.print("\r");
          }
        }
      }
    }
  }

delay(100);
}
