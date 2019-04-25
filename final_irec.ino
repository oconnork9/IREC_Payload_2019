// import libraries for sensors
#include <SPI.h>
#include <SD.h>
#include <DHT.h>

// instantiate variables
float temp;
float humid;
int timeS;

// set pins
#define DHTPIN 7                                // Temp / Humidity Sensor - "PWM" pin 7
#define DHTTYPE DHT22                           // DHT 22 (AM2302)
DHT dht(DHTPIN, DHTTYPE);                       // initialize DHT sensor for normal 16mhz Arduino

// particulate matter sensor
unsigned long duration;
unsigned long starttime;
unsigned long sampletime_ms = 2000;
unsigned long lowpulseoccupancy = 0;
float ratio = 0;
float concentration = 0;
int pin = 8;                                    // "PWM" pin 8

int MQ2 = A1;                                   // MQ2 - Analog pin 1
int MQ9 = A0;                                   // MQ9 - Analog pin 0

File myFile;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);                           // 9600 baud rate
  dht.begin();                                  // initialize dht sensor
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.print("Initializing SD card...");      // update message

  if (!SD.begin(53)) {
    Serial.println("initialization failed!");   // failure message
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
    myFile.close();                             // close the file
    Serial.println("done.");                    // closing message
  } else {
    Serial.println("error opening test.txt");   // if the file didn't open, print an error
  }

  // re-open the file for reading:
  myFile = SD.open("test.txt");
  if (myFile) {
    Serial.println("test.txt:");

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    myFile.close();                             // close the file
  } else {
    Serial.println("error opening test.txt");   // if the file didn't open, print an error:
  }
    timeS = 0;                                  // time variable
    pinMode(8,INPUT);                           // initialize "PWM" pin 8
    starttime = millis();
}

void loop() {
    int MQ2d = analogRead(MQ2);                 // read from the MQ2
    int MQ9d = analogRead(MQ9);                 // read from the MQ9
    
    temp = dht.readTemperature();               // read the temp
    temp = temp * 1.8 + 32;                     // convert to F
    humid = dht.readHumidity();                 // read the humidity
    
    delay(1000);                                // delay before writing
    myFile = SD.open("test.txt", FILE_WRITE);   // open the text file to write

  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to test.txt...");
    myFile.print(timeS);
    myFile.print(", ");
    myFile.print(temp);
    myFile.print(", ");
    myFile.print(humid);
    myFile.print(", ");
    myFile.print(MQ2,DEC);
    myFile.print(", ");
    myFile.print(MQ9,DEC);
    myFile.print(", ");
    myFile.println(concentration);
    delay(1000);
    myFile.close();                             // close the file
    Serial.println("done.");
  } else {
    Serial.println("error opening test.txt");   // if the file didn't open, print an error
  }

  // re-open the file for reading:
  myFile = SD.open("test.txt");
  if (myFile) {
    Serial.println("test.txt:");
    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    myFile.close();                             // close the file
  } else {
    Serial.println("error opening test.txt");   // if the file didn't open, print an error
  }

  timeS++;                                      // increment time variable

  //code for particulate matter sensor
  duration = pulseIn(pin, LOW);
  lowpulseoccupancy = lowpulseoccupancy+duration;
  if ((millis()-starttime) >= sampletime_ms)    // if the sample time = = 30s
  {
    ratio = lowpulseoccupancy/(sampletime_ms*10.0);  
    concentration = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62;
    lowpulseoccupancy = 0;
    starttime = millis();
  }
  
  delay(2000); // wait until restarting loop
}
