//by Mans Rademaker 2023
//ArduinoIDE Boards ATTiny/25/45/85 ATTiny85 clock 16Mhz intern / programmer Arduino as ISP
//help urls
//https://www.arduinoslovakia.eu/blog/2018/10/attiny85-a-displej-tm1637?lang=enhttps://www.arduinoslovakia.eu/blog/2018/10/attiny85-a-displej-tm1637?lang=en
//https://github.com/avishorp/TM1637
//https://simple-circuit.com/wp-content/uploads/2023/05/tm1637-display-module-circuit-schematic-diagram.png

#include  <TM1637Display.h>
#define   CLK 4                //PB4=D4=pin3
#define   DIO 3                //PB3=D3=pin2
TM1637Display display(CLK, DIO);

#include  <DHT.h>
#define   DHTTYPE    DHT22     // DHT 22 (AM2302)
#define   tempPin 1            // dht on PB1 pin
DHT dht   (tempPin, DHTTYPE);  // Define Temp Sensor

#define   myDelay 5000
float     ftemp,fhum,meting;
float     gtemp[5],ghum[5];
int       mytemp,myhum;
int       counter,total;

const uint8_t SEG_NONE[] = {
  SEG_C | SEG_E | SEG_G,                           // n
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,   // O
  SEG_C | SEG_E | SEG_G,                           // n
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G            // E
  };
// Create % symbol:
const uint8_t percent[] = {
  SEG_A | SEG_B | SEG_F | SEG_G,                    // Circle high
  SEG_C | SEG_D | SEG_E | SEG_G };                  // Circle low
// Create circle high symbol(degree):
const uint8_t circleh[] = {SEG_A | SEG_B | SEG_F | SEG_G };
  
void setup() {
  display.setBrightness(0x02);
  display.clear ();
  pinMode       (tempPin,INPUT);
  dht.begin     ();
}

void loop() {
  mytemp  =   dht.readTemperature()*10;
  myhum   =   dht.readHumidity();
  gtemp[counter] = mytemp;
  if (total>4){                                     // use average after 5 measurements
    mytemp=0;
    for (int n=0;n<5;n++){
      mytemp+=gtemp[counter];
    }
    mytemp=mytemp/5;
  }
  ghum[counter] = myhum;
  if (total>4){
    myhum=0;
    for (int n=0;n<5;n++){
      myhum+=ghum[counter];
    }
    myhum=myhum/5;
  }

  display.clear();
  if (mytemp==0){
    display.setSegments     (SEG_NONE);
  } else {
    display.showNumberDecEx (mytemp, 0b001000000, true,3,0);//20.9_
    display.setSegments     (circleh,1,3);
  } 
  delay                     (myDelay);

  display.clear();
  if (myhum==0){
    display.setSegments     (SEG_NONE);
  } else {
    //void showNumberDec    (num, leading_zero, length, pos);
    display.showNumberDec   (myhum, false, 2, 0);
    display.setSegments     (percent,2,2);
    delay                   (myDelay); 
  }
  counter++;if (counter>4)  {counter=0;}
  total++;  if (total>9999) {total=5;}
}
