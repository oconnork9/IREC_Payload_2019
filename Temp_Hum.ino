/* How to use the DHT-22 sensor with Arduino uno
   Temperature and humidity sensor
*/

//Libraries
#include <DHT.h>

//Constants
#define DHTPIN 7     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino

//Variables
int chk;
float hum;  //Stores humidity value
float temp; //Stores temperature value
int smokeA0 = A5;
// Your threshold value
int sensorThres = 400;

void setup()
{
  Serial.begin(9600);
  dht.begin();
}

void loop()
{
    delay(1000);
    int analogSensor = analogRead(smokeA0);
    //Read data and store it to variables hum and temp
    hum = dht.readHumidity();
    temp= dht.readTemperature();
    //Print temp and humidity values to serial monitor
    Serial.print("Pin A0: ");
    Serial.print(analogSensor);
    Serial.print(", Humidity: ");
    Serial.print(hum);
    Serial.print(" %, Temp: ");
    Serial.print(temp);
    Serial.println(" Celsius");
    delay(1000); //Delay 2 sec.
}

   
