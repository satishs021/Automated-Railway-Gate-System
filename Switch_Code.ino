#include <SoftwareSerial.h>
#include <TinyGPS.h>
SoftwareSerial SIM900(2, 3);//(2->Tx,3->Rx)
TinyGPS gps;//(0->Tx,1->Rx)

#define TRIGGER_PIN1  6
#define ECHO_PIN1     7
#define TRIGGER_PIN2  8
#define ECHO_PIN2     9
#define TRIGGER_PIN3  10
#define ECHO_PIN3     11
#define LIGHT_PIN     12
#define motorPin1  4  // Pin 10 of L293 FRONT motors 
#define motorPin2  5 // Pin  2 of L293 BACK motors
#define LIGHT_PIN     12//ouput Indicator

char phone_no[] = "7892094605"; //replace with phone no. to get sms

long duration1, duration2,duration3;  
int distance1, distance2,distance3; 

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
SIM900.begin(9600);
  // Configuring Ultrasonic Sensor       
  pinMode(TRIGGER_PIN1, OUTPUT);  
  pinMode(ECHO_PIN1, INPUT);  
  pinMode(LIGHT_PIN, OUTPUT);
  pinMode(TRIGGER_PIN2, OUTPUT);  
  pinMode(ECHO_PIN2, INPUT);
  pinMode(TRIGGER_PIN3, OUTPUT);  
  pinMode(ECHO_PIN3, INPUT); 
  pinMode(motorPin1,OUTPUT);
  pinMode(motorPin2,OUTPUT);
   // Set Motor as OUTPUT
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
 
}

void loop() {
   /*==================================
   *    Ultrasonic Sensor#1
   ==================================*/
  //clear TRIGGER_PIN1
  digitalWrite(TRIGGER_PIN1, LOW);  
  delayMicroseconds(2);          

  //set TRIGGER_PIN1 to High for 10 microseconds
  digitalWrite(TRIGGER_PIN1, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN1, LOW);

  //read the duration of the sound wave with ECHO_PIN1
  duration1 = pulseIn(ECHO_PIN1, HIGH);

  //calculate distance based on duration of ultrasound from triggerPin to echoPin
  distance1 = (duration1*0.034/2);

  //add thresholds to correct for sensor value errors
  //if (distance1 < 0 ) distance1 = 0;
  //if (distance1 > 200) distance1 = 200;
  
  /*==================================
   *    Ultrasonic Sensor#2
   ==================================*/
  //clear TRIGGER_PIN2 value, setting it to LOW for 2 microseconds
  digitalWrite(TRIGGER_PIN2, LOW);  
  delayMicroseconds(2);          

  //set TRIGGER_PIN2 to High for 10 microseconds
  digitalWrite(TRIGGER_PIN2, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN2, LOW);

  //read the duration of the sound wave with ECHO_PIN1
  duration2 = pulseIn(ECHO_PIN2, HIGH);

  //calculate distance based on duration of ultrasound from triggerPin to echoPin
  distance2 = (duration2*0.034/2);
 
  //add thresholds to correct for sensor value errors
  //if (distance2 < 0 ) distance2 = 0;
  //if (distance2 > 200) distance2 = 200;

/*==================================
   *    Ultrasonic Sensor#3
   ==================================*/
  //clear TRIGGER_PIN2 value, setting it to LOW for 2 microseconds
  digitalWrite(TRIGGER_PIN3, LOW);  
  delayMicroseconds(2);          

  //set TRIGGER_PIN2 to High for 10 microseconds
  digitalWrite(TRIGGER_PIN3, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN3, LOW);

  //read the duration of the sound wave with ECHO_PIN1
  duration3 = pulseIn(ECHO_PIN3, HIGH);

  //calculate distance based on duration of ultrasound from triggerPin to echoPin
  distance3 = (duration3*0.034/2);
 
  //add thresholds to correct for sensor value errors
  //if (distance3 < 0 ) distance3 = 0;
  //if (distance3 > 200) distance3 = 200;
   
  /*==================================
   *    Print to Serial Monitor
   ==================================*/
 
  Serial.print("distance1: ");
  Serial.print(distance1);
  Serial.print("cm, distance 2");
  Serial.print(distance2);
  Serial.println("cm ");
  Serial.print(distance3);
  Serial.println("cm ");

  /*==================================
   *    Control LED
   ==================================*/  

 while ((distance1 > 5 | distance2 > 5)| distance3 < 10) {
    digitalWrite(LIGHT_PIN, HIGH);
    Serial.println("Crack Detected");
    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, LOW);
    crackdetected();
    break;
  } 
    digitalWrite(LIGHT_PIN, LOW);
    digitalWrite(motorPin1, HIGH);
    digitalWrite(motorPin2, HIGH);
    delay(850);
    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, LOW);
    delay(50);
    
}

//Sub-Function for GPS and GSM 
void crackdetected()
{
   bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;
   for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (Serial.available())
    {
      char c = Serial.read();
      Serial.print(c);
      if (gps.encode(c)) 
        newData = true;  
    }
  }

    float flat, flon;
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);   
    SIM900.print("AT+CMGF=1\r"); 
    delay(400);
    SIM900.print("AT+CMGS=\"");
    SIM900.print(phone_no);
    SIM900.println("\"");
    
    delay(300);
    SIM900.print("Crack Detected @ https://www.google.com/maps/?q=");
     Serial.print(",");
   // Gsm.print("Latitude = ");
    SIM900.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
    //Gsm.print(" Longitude = ");
    SIM900.print(",");
    SIM900.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
    delay(200);
    SIM900.println((char)26); // End AT command with a ^Z, ASCII code 26
    delay(200);
    SIM900.println();
    delay(90000);

 
  Serial.println(failed);
 // if (chars == 0)
   // Serial.println("** No characters received from GPS: check wiring **");
}
