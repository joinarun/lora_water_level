#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
WiFiManager wm;
WiFiManagerParameter custom_mqtt_server("server", "mqtt server", "", 40);
int Relay  = 2;                 // check pin mapping

void setup() {
    WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP    
    // put your setup code here, to run once:
    Serial.begin(115200);
    Serial.println("On Startup delay for 4 mins)");
    delay(2000);  //4mins is 240 sec i.e 240000 mili sec

    pinMode(Relay, OUTPUT);       // declare Relay as output
    
    //reset settings - wipe credentials for testing
    //wm.resetSettings();
    wm.addParameter(&custom_mqtt_server);
    wm.setConfigPortalBlocking(false);
    wm.setSaveParamsCallback(saveParamsCallback);

    //automatically connect using saved credentials if they exist
    //If connection fails it starts an access point with the specified name
    if(wm.autoConnect("AutoConnectAP")){
        Serial.println("connected...yeey :)");
    }
    else {
        Serial.println("Configportal running");
    }
}

void loop() {
    wm.process();
    // put your main code here, to run repeatedly:
    Serial.println("main loop running");
    delay(2000); 

    //read tank level
    int level;
    level = ReadSens_and_Condition();

    if(level < LowerLimit){
      //start motor
      digitalWrite (Relay, HIGH);
      //start a loop to monitor rise in water and timeout 
      //if any one of the condition is met then stop motor
    }
    
}

void saveParamsCallback () {
  Serial.println("Get Params:");
  Serial.print(custom_mqtt_server.getID());
  Serial.print(" : ");
  Serial.println(custom_mqtt_server.getValue());
}

//function to read tank level 
int ReadSens_and_Condition(){
  int i;
  int sval = 0;
  for (i = 0; i < 5; i++){
    sval = sval + analogRead(0);    // sensor on analog pin 0
  }
  sval = sval / 5;    // average
  return sval;
}
