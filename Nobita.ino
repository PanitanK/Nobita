///////////////////////////////////////////////

/* This Motion capture device is based on Arduino Wifi Rev2 
 *  with  LSM6DS3TR IMU ( Inertia Measurement Unit ) at maximum sample rate of 104 Hz ( 9.615 ms per period )
 *  But I did put some delay between each sample period 
 *  
 *  There are 2 modes 
 *  
 *  1. Motion capture mode 
 *   1.1 Insert an SD card 
 *   1.2 Use up and down button to select your prefer class 
 *   1.3 Press the Select button to record
 *   1.4 The 3 left digits will blink Std ( Steady ) meaning it is reccording 
 *  
 *  2. UART Live mode 
 *   2.1 There will be a python Program named "Nobita_Serial.py" Can be found from this link  
 *   2.2 Follow the procedure in github
 *   2.3 If 4 Digit display word UART and 6 variable appear in a Terminal everything should work just fine 
 *  
 *  LSM6DS3TR IMU datasheet  https://www.verical.com/datasheet/stmicroelectronics-misc-sensors-LSM6DS3TR-C-7107181.pdf
 *  Arduino Wifi Rev2 website  https://docs.arduino.cc/hardware/uno-wifi-rev2
 *  Github https://github.com/PanitanK/Nobita
 * 
 * 
 *  Delay adjustment 
 *  
 *  use Ctrl + F to search and locate these 3 delays
 *  
 *  1. LED DISPLAY Delay  dt0     ( default 50ms interval or 20 Hz )
 *  2. Motion Capture Delay  dt1   ( default 100ms interval or 10 Hz )
 *  3. UART Data Transfer delay dt2 ( default 50ms interval or 20 Hz )
 *  
 *  LED DISPLAY always work in this case so 
 *  
 *  total interval for Motion capture = dt0 + dt1
 *  total interval for UART LIVE  = dt0 + dt2
 *  
 *  Adjust it to your preference 
 * 
 * 
 * Created by Panitan Kwankaew 
 * School of Engineering , Bangkok University 
 */
 
///////////////////////////////////////////////









#include <Arduino_LSM6DS3.h>
#include <SPI.h>
#include <SD.h>
#include <TM1650.h>
#include <stdlib.h>

TM1650 d;
File myFile , myFile2;
int j;
  
String buttonState;     
int triggerState = 0;
int timestamp;
volatile int i = 1;
String msg;
int flag = 0;

String stat = "5EL" ; 

void setup() {

    d.displayOff();
  d.setBrightness(TM1650_MIN_BRIGHT);
  d.displayOn();
  Serial.begin(9600);
  Wire.begin(); //Join the bus as master
   d.init();

  if (!IMU.begin()) {
    //Serial.println(" failed!");
    while (1);
  }
  //Serial.println(" done");
  //Serial.print("Accelerometer sample rate = ");
  //Serial.print(IMU.accelerationSampleRate());
  //Serial.println(" Hz");

  while (!Serial) {
   
  }
  //Serial.print("Initializing SD card...");
  if (!SD.begin(7)) {

    //Serial.println("NO SD-CARD FOUND");
  }
  //Serial.println("initialization done.");

}

void loop() {
  String FILENAME ,OUTPUTTOMYFILE , FILENAME2 ,OUTPUTTOMYFILE2 ;
    buttonState = checkbutton(); 

  ////////////////////////////////
   // LED DISPLAY 
  ////////////////////////////////
  if (buttonState == "KEY_UP") 
  {
    i++;
    stat = "SEL";
     if ( i > 9 ) {
      i = 1;
      } 
    while (buttonState == "KEY_UP"){
      buttonState = checkbutton();
      
      }
  }

  else if (buttonState == "KEY_DOWN") 
  {
    i--;
    stat = "SEL";
     while (buttonState == "KEY_DOWN"){
      buttonState = checkbutton();
      }
    if ( i < 1 ) {
      i = 9;
      } 
  }  



char Display[5] ; 
String s1 = String(stat);
String s2 = String(i);
String s3 = s1+s2;

s3.toCharArray(Display, 5);



d.displayString(Display);
delay(50); /* LED DISPLAY Delay  dt0 */


////////////////////////////////////////////////////////////////////////////////////////////
// SD CARD ( Record MODE )
/////////////////////////////////////////////////////////////////////////////////////////



  
  if (buttonState == "KEY_SELECT") {
    FILENAME2 = "Record.csv"  ;
    timestamp = millis();
    if (SD.exists(FILENAME2) ){
      Serial.print("hahahahahahahahah");
      }

    else {
    
    myFile2 = SD.open(FILENAME2, FILE_WRITE);
    if (myFile2) {
    myFile2.print("x_acceleration_in(g),y_acceleration_in(g),z_acceleration_in(g),x_omega,y_omega,z_omega,Class");
    myFile2.print("\n");
    myFile2.close();
    }
      }
    
    while (buttonState == "KEY_SELECT"){
    buttonState = checkbutton();
    d.displayString("5td");
    float xA, yA, zA , xG , yG , zG;
    if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(xA, yA, zA);
    }
    if (IMU.gyroscopeAvailable()) {
    IMU.readGyroscope(xG, yG, zG);
}
    OUTPUTTOMYFILE2 =  String(xA) +","+ String(yA)+ "," + String(zA)+ "," + String(xG) + "," + String(yG) + "," + String(zG)+"," + String(i) ;
  myFile2 = SD.open(FILENAME2, FILE_WRITE);
    if (myFile2) {
    myFile2.println(OUTPUTTOMYFILE2);
    myFile2.close();
  }
 d.displayString("   ");
  delay(100); /* Motion Capture Delay  dt1 */
}
}


////////////////////////////////////////////////////////////////////////////////////////////
// LIVE MODE ( ON UART MODE )
/////////////////////////////////////////////////////////////////////////////////////////

if ( Serial.available()) {
  stat = "UART";
  while (Serial.available()) {
    msg = Serial.read();
    Serial.println(msg);
  }

    delay(50); /* UART Data Transfer delay dt2 */

        float xA, yA, zA , xG , yG , zG;
    if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(xA, yA, zA);
    }
    if (IMU.gyroscopeAvailable()) {
    IMU.readGyroscope(xG, yG, zG);
}
    Serial.println(  String(xA) +","+ String(yA)+ "," + String(zA)+ "," + String(xG) + "," + String(yG) + "," + String(zG) ) ; 

    if ( !Serial.available()) {
    Serial.begin(9600);
    }
  }

} // END OF LOOP fx //





String checkbutton() {
    unsigned int keyVal;
    keyVal=analogRead(A0);
    if(keyVal<150)return("KEY_DOWN");//0V-down
    else if(keyVal<350)return("KEY_LEFT");//1V-LEFT
    else if(keyVal<550)return("KEY_UP");//2V-up
    else if(keyVal<750)return("KEY_RIGHT");//3V-right
    else if(keyVal<950)return("KEY_SELECT");//4V-select
    else return("KEY_NULL");
  }





  
