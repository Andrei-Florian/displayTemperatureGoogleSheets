// Universum | Projects | Google SpreadSheets > AppendCurrentCondition

// Andrei Florian 3/JAN/2018

#include <Bridge.h> // library to communicate ovr Yun's bridge
#include <Temboo.h> // library to interface Temboo
#include "Accounts.h" // contains Temboo account information - user must set
#include <dht.h> // library to interact with DHT temperature sensor

char values[50]; // buffer to send to Temboo
int temp; // temperature as integer
int hum; // humidity as integer
int overview; // temp and hum
dht DHT; // class DHT variable to retrieve DHT data

int proDebug = 0; // TODO - debugging?

void setup()
{
  if(proDebug == 1)
  {
    delay(1000);
    Serial.begin(9600);
    while(!Serial);
  }
  
  delay(4000);
  Bridge.begin();

  if(proDebug == 1)
  {
    Serial.println("Universum");
    delay(500);
    Serial.println("  writeCurrentTempAndHumidity");
    delay(500);
    Serial.println("    Andrei Florian");
    Serial.println("");
  }
  
  delay(2000);
}

void loop()
{
  overview = DHT.read11(11); // Read the sensor output
  temp = DHT.temperature,1; // store temperature
  hum = DHT.humidity,1; // store humidity

  if(proDebug == 1)
  {
    Serial.println("");
    Serial.println("");
  }
  
  sprintf(values,"[[%02d,%02d,]]",temp,hum); // insert temp and hum in the json packet required

  if(proDebug == 1)
  {
    Serial.println("Running Choreo");
    delay(500);
    Serial.print("Inputted Values ");
    Serial.println(values);
    Serial.println("");
    delay(500);
  }

  TembooChoreo AppendValuesChoreo; // initialise which choreo to begin
  AppendValuesChoreo.begin(); // begin the choreo

  // Set Temboo account credentials - Set in Acounts.h
  AppendValuesChoreo.setAccountName(TEMBOO_ACCOUNT);
  AppendValuesChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
  AppendValuesChoreo.setAppKey(TEMBOO_APP_KEY);

  // Set Choreo inputs - Set in Accounts.h
  AppendValuesChoreo.addInput("RefreshToken", GOOGLE_CLIENT_REFRESH_TOKEN);
  AppendValuesChoreo.addInput("ClientSecret", GOOGLE_CLIENT_SECRET);
  AppendValuesChoreo.addInput("Values", values);
  AppendValuesChoreo.addInput("ClientID", GOOGLE_CLIENT_ID);
  AppendValuesChoreo.addInput("SpreadsheetID", GOOGLE_SPREADSHEET_ID);

  AppendValuesChoreo.setChoreo("/Library/Google/Sheets/AppendValues"); // identify choreo to run
  AppendValuesChoreo.run(); // run the choreo

  if(proDebug == 1)
  {
    while(AppendValuesChoreo.available()) // print results if available
    {
      char c = AppendValuesChoreo.read();
      Serial.print(c);
    }
  }
  
  AppendValuesChoreo.close(); // end choreo
  
  if(proDebug == 1)
  {
    Serial.println("");
    Serial.println("Choreo Executed");
    Serial.println("");
  }

  delay(60000); // wait a minute between executions
}

