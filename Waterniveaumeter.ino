// ---------------------------------------------------------------------------
// Watertank niveau meting Scouting BvH
// ---------------------------------------------------------------------------
#include <avr/wdt.h>
#include <NewPing.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#if defined(ARDUINO) && ARDUINO >= 100
#define printByte(args)  write(args);
#else
#define printByte(args)  print(args,BYTE);
#endif
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
uint8_t blok[8] = {0xff,0xff,0xff,0xff,0xff,0xff,0xff};

#define TRIGGER_PIN  12  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     11  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 120 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
int afstandcm = 0;
unsigned int liters = 0;
int fracties = 0;
byte meetfout = 0;

#define INHOUDTANK 1000
#define AFSTANDVOL 10

void setup() {
  lcd.createChar(1, blok);
  wdt_enable(WDTO_4S);
  Serial.begin(115200); // Open serial monitor at 115200 baud to see ping results.
  lcd.init();         
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Scouting BvH");
  lcd.setCursor(0,1);
  lcd.print("WaterNiveau V1.0");
  delay(2000);
  lcd.clear();
}

void loop() {
  wdt_reset();
  InhoudMeten();
  lcd.setCursor(0,0);
  if(meetfout < 11){
    lcd.print("Inhoud: ");lcd.print("      ");
    lcd.setCursor(8,0);
    lcd.print(liters);lcd.print("L");
    lcd.setCursor(0,1);
    lcd.print("[");lcd.setCursor(15,1);lcd.print("]");
    lcd.setCursor(1,1);
    for(int x = 1; x <= fracties; x++){ 
      lcd.printByte(1);
    }
    lcd.setCursor(fracties+1,1);
    for(int x = 1; x <= 14-fracties; x++){ 
      lcd.print(" ");
    }
  }else{
    lcd.clear();
    lcd.print("Storing...");
  }
}

void InhoudMeten(){
  unsigned int uS = sonar.ping_median(20); // Send ping, get ping time in microseconds (uS).
  if(sonar.convert_cm(uS) == 8 || sonar.convert_cm(uS) == 0){
    afstandcm = afstandcm;
    meetfout++;
  }else{   
    afstandcm = sonar.convert_cm(uS);
    meetfout = 0;
  }
  liters = INHOUDTANK - ((afstandcm - AFSTANDVOL) * 10);
  fracties = liters/72;
  if(fracties > 14)fracties = 14;
  Serial.print("Ping: ");Serial.print(afstandcm);Serial.println("cm");
  Serial.print("Inhoud: ");Serial.print(liters);Serial.println("L");
}
  
