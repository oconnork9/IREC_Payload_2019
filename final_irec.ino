#include <SPI.h>
#include <SD.h>
#include <DHT.h>

float temp;
float humid;
int timeS;

#define DHTPIN 7     // what pin we're connected to - Digital 7
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino
unsigned long duration;
unsigned long starttime;
unsigned long sampletime_ms = 2000; 
unsigned long lowpulseoccupancy = 0;
float ratio = 0;
float concentration = 0;
int pin = 8;
int MQ2 = A0;
File myFile;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  dht.begin();
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.print("Initializing SD card...");

  if (!SD.begin(53)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("test.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to test.txt...");
    myFile.println("Index, Temperature, Humidity, Carbon Monoxide, Smoke, Particulate Matter");   // print the headings
    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }

  // re-open the file for reading:
  myFile = SD.open("test.txt");
  if (myFile) {
    Serial.println("test.txt:");

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
    timeS = 0;

    pinMode(8,INPUT);
    starttime = millis(); 
}

void loop() {
    int analogSensor = analogRead(MQ2);
    
    int val;
    val=analogRead(A0);//Read Gas value from analog 0
    
    temp = dht.readTemperature();
    temp = temp * 1.8 + 32;
    humid = dht.readHumidity();
    
    delay(1000); //Delay 2 sec.
    myFile = SD.open("test.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to test.txt...");
    myFile.print(timeS);
    myFile.print(", ");
    myFile.print(temp);
    myFile.print(", ");
    myFile.print(humid);
    myFile.print(", ");
    myFile.print(val,DEC);
    myFile.print(", ");
    myFile.print(analogSensor);
    myFile.print(", ");
    myFile.println(concentration);
    delay(1000);
    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }

  // re-open the file for reading:
  myFile = SD.open("test.txt");
  if (myFile) {
    Serial.println("test.txt:");

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }

  timeS++;


  //code for particulate matter sensor
  duration = pulseIn(pin, LOW);
  lowpulseoccupancy = lowpulseoccupancy+duration;
  if ((millis()-starttime) >= sampletime_ms) //if the sampel time = = 30s
  {
    ratio = lowpulseoccupancy/(sampletime_ms*10.0);  
    concentration = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62; 
    //Serial.print("Concentration = ");
    //Serial.print(concentration);
    //Serial.println(" pcs/0.01cf");
    //Serial.println("\n");
    lowpulseoccupancy = 0;
    starttime = millis();
  }
  
  delay(2000);
}
