#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include "SparkFunISL29125.h"
#include <DHT.h>
#include <SPI.h>
#include <SD.h>

#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht (DHTPIN,DHTTYPE);
File myFile;
// Declare sensor object
SFE_ISL29125 RGB_sensor;

int chk;
float hum;
float temp;
int m_id=0;

// WiFi setup:
const char *ssid     = "wifi name";
const char *password = "wifi password";

const long utcOffsetInSeconds = 3600;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

void setup()
{
  // Initialize serial communication
  Serial.begin(115200);
  dht.begin();
  // Initialize the ISL29125 with simple configuration so it starts sampling
  if (RGB_sensor.init())
  {
    Serial.println("Sensor Initialization Successful\n\r");
  }
   Serial.print("Initializing SD card...");

  if (!SD.begin(0)) {
    Serial.println("initialization failed!");
  }
  else {
    
    Serial.println("initialization done.");
  }
 
   WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }

  timeClient.begin();

  
}

void saveMeasurementToFile( float hum, float temp, int r, int g, int b, int m_id) {        
    timeClient.update();
    int hour = timeClient.getHours()+1;
    int minutes = timeClient.getMinutes();
    int seconds = timeClient.getSeconds();
    char mid[50];
    sprintf (mid, "%d\_\%d\_\%d", hour, minutes, seconds);
   
    char filename[50];
    sprintf(filename, "\measurement_\%s\.json", mid);

    Serial.println("filename:");
    Serial.println(filename);    
    myFile = SD.open(filename, FILE_WRITE); 
    char buffer[100];
    sprintf (buffer, "{\"id\": %d, \"hum\": %f, \"temp\": %d, \"light\": { \"r\": %d, \"g\": %d, \"b\": %d } }", m_id, hum, temp, r, g, b );
    if (myFile){
    myFile.print(buffer);
    myFile.close();
    }
    else{
      Serial.println ("failed to create a file");
    }
}

;
// Read sensor values for each color and print them to serial monitor
void loop()
{

  hum = dht.readHumidity();
  temp = dht.readTemperature();
  // Read sensor values (16 bit integers)
  unsigned int r = RGB_sensor.readRed();
  unsigned int g = RGB_sensor.readGreen();
  unsigned int b = RGB_sensor.readBlue();

  saveMeasurementToFile( hum, temp, r, g, b, m_id);
  m_id++;
  Serial.println (m_id);
  delay (3000);// Print out readings, change HEX to DEC if you prefer decimal output
   timeClient.update();

  Serial.print(daysOfTheWeek[timeClient.getDay()]);
  Serial.print(", ");
  Serial.print(timeClient.getHours()+1);
  Serial.print(":");
  Serial.print(timeClient.getMinutes());
  Serial.print(":");
  Serial.println(timeClient.getSeconds());
  //Serial.println(timeClient.getFormattedDate());
  Serial.print("Measurement ID: ");
  Serial.println(m_id);
  Serial.print("Humidity: ");
  Serial.print(hum);
  Serial.print(" %, Temp: ");
  Serial.print(temp);
  Serial.println(" Celsius");
  Serial.print("Red: "); 
  Serial.println(r,DEC);
  Serial.print("Green: "); 
  Serial.println(g,DEC);
  Serial.print("Blue: "); 
  Serial.println(b,DEC);
  Serial.println();
}
