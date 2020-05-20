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



//DOŁĄCZAM:
const char *ssid     = "GombaNetwork";
const char *password = "Wrona#@!123";

const long utcOffsetInSeconds = 3600;


char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);
// KONIEC

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
 
/*
  for (int i=0; i++;){
   
    char filename[50];
    sprintf(filename, "measurement_%d.json",i);
    SD.remove (filename);
    if (!SD.remove(filename)){

      break;
    }

  }

*/
   WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }

  timeClient.begin();

  
}
/*
timeClient.update();
int hour = timeClient.getHours()+1;
int minutes = timeClient.getMinutes();
int seconds = timeClient.getSeconds();
*/
void saveMeasurementToFile( float hum, float temp, int r, int g, int b, int m_id) {
    ///////////////////////        
    timeClient.update();
    int hour = timeClient.getHours()+1;
    int minutes = timeClient.getMinutes();
    int seconds = timeClient.getSeconds();
    char mid[50];
    sprintf (mid, "%d\_\%d\_\%d", hour, minutes, seconds);
    /*
    Serial.println("mid:");    
    Serial.println (mid);
    Serial.println("hour:");
    Serial.println(hour);
    Serial.println("minutes:");
    Serial.println(minutes);
    Serial.println("seconds:");
    Serial.println(seconds);
    */
    char filename[50];
    sprintf(filename, "\measurement_\%s\.json", mid);
    //sprintf(filename, "meas_%d.json", m_id);
    //sprintf(filename, "filename");

    Serial.println("filename:");
    Serial.println(filename);    
    myFile = SD.open(filename, FILE_WRITE); // measurement_1.json => measurement_2020-04-22-17-28-01.11.json
    char buffer[100];
    sprintf (buffer, "{\"id\": %d, \"hum\": %f, \"temp\": %d, \"light\": { \"r\": %d, \"g\": %d, \"b\": %d } }", m_id, hum, temp, r, g, b );
    if (myFile){
      Serial.println("tu");
    myFile.print(buffer);
    myFile.close();
    }
    else{
      Serial.println ("nie moge utworzyc pliku");
    }
}

;
// Read sensor values for each color and print them to serial monitor
void loop()
{
  //SD.open();
  //File myFile = ("ugubugu.txt", FILE_WRITE);
  //myFile.print ("hello");
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
  //SD.remove("test.txt");
  //SD.remove("xxx.txt");
  
  //DOLACZAM:
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
  //KONIEC
}
