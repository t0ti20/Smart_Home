
/*--------------------------------------------------------------*/
/*---------------- Define ----------------*/
/*--------------------------------------------------------------*/
/*----------------------------------------------------------*/
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define REST      0
int tempo = 180;
int buzzer = 3;
int melody[] = 
{
  NOTE_E5, 8, NOTE_D5, 8, NOTE_FS4, 4, NOTE_GS4, 4, 
  NOTE_CS5, 8, NOTE_B4, 8, NOTE_D4, 4, NOTE_E4, 4, 
  NOTE_B4, 8, NOTE_A4, 8, NOTE_CS4, 4, NOTE_E4, 4,
  NOTE_A4, 2, 
};
int notes = sizeof(melody) / sizeof(melody[0]) / 2;
int wholenote = (60000 * 4) / tempo;
int divider = 0, noteDuration = 0;
/*----------------------------------------------------------*/
#define master "2005"
#define sw 2 
#define buzzer 3 
#define doorl  A2
#define leds   A1
#define temp   A0
#define smoke  A3
#define fan    4 
/*--------------------------------------------------------------*/
/*---------------- Temperature Sensor ----------------*/
/*--------------------------------------------------------------*/
#include <SimpleDHT.h>
int pinDHT11 = temp;
SimpleDHT11 dht11(pinDHT11);
/*--------------------------------------------------------------*/
/*---------------- Servo ----------------*/
/*--------------------------------------------------------------*/
#include <Servo.h>
Servo door;
/*--------------------------------------------------------------*/
/*---------------- Keypad ----------------*/
/*--------------------------------------------------------------*/
#include <Keypad.h>
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {6, 7, 8, 9}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {10, 11, 12, 13}; //connect to the column pinouts of the keypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 
/*--------------------------------------------------------------*/
/*---------------- LiquidCrystal_I2C ----------------*/
/*--------------------------------------------------------------*/
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);
/*--------------------------------------------------------------*/
/*---------------- Initial ----------------*/
/*--------------------------------------------------------------*/
int state;
bool ledf=0;
bool fanf=0;
bool doorf=0;
/*--------------------------------------------------------------*/
/*--------------------------------------------------------------*/
/*--------------------------------------------------------------*/
/*--------------------------------------------------------------*/
/*--------------------------------------------------------------*/
/*--------------------------------------------------------------*/
/*--------------------------------------------------------------*/
/*--------------------------------------------------------------*/
/*--------------------------------------------------------------*/
/*--------------------------------------------------------------*/
void setup() {
    /*----------------------------------------------------------*/

    /*----------------------------------------------------------*/
    Serial.begin(9600);
    pinMode(sw,INPUT_PULLUP);
    pinMode(buzzer,OUTPUT);
    pinMode(smoke,INPUT);
    pinMode(leds,OUTPUT);
    pinMode(doorl,OUTPUT);
    pinMode(fan,OUTPUT);
    digitalWrite(buzzer,0);
    digitalWrite(leds,1);
    digitalWrite(doorl,0);
    digitalWrite(fan,fanf);
    door_control(doorf);
    door.attach(5);
    lcd.init();
    lcd.backlight();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("<< Smart Home >>");
    lcd.setCursor(0,1);
    lcd.print("________________");
    sound();
}

void sound(void)
{
  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) 
  {
    divider = melody[thisNote + 1];
    if (divider > 0) 
    {
      noteDuration = (wholenote) / divider;
    }
    else if (divider < 0) 
    {
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5;
    }
    tone(buzzer, melody[thisNote], noteDuration * 0.9);
    delay(noteDuration);
    noTone(buzzer);
  }
}


void loop() 
{
  char key ;
  bool f=1;
  access();
  while(f)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("A)Lights B)Fan");
    lcd.setCursor(0,1);
    lcd.print("C)Temp   D)Door");
    smoke_test();
    bluetooth();
    state=digitalRead(sw);
    while(state==0)
     {
       digitalWrite(buzzer,1);
       state=digitalRead(sw);
     }
     digitalWrite(buzzer,0);
     do
      {
        key= customKeypad.getKey ();
      }while(key == NO_KEY);
     if(key=='A')
       {
          digitalWrite(leds,ledf);
          ledf=!ledf;
       }
      else if(key=='B')
        {
         digitalWrite(fan,fanf);
          fanf=!fanf;
        }
      else if(key=='C')
        {
          temp_reading();
         smoke_test();
        }
     else if(key=='D')
       {
         doorf=!doorf;
         door_control(doorf);
       }
      else {f=0;}
   }
}



void door_control(int x)
{
  if(x==1)
  {
    door.write(100);
    delay(666);
    door.write(90);
  }
  if(x==0)
  {
    door.write(80);
    delay(500);
    door.write(90);
  }
}

void smoke_test(void)
{
  int x=analogRead(smoke);
  int i=0;
  if(x>300)
  {
    i++;
    while(x>300)
    {
     if(i==1)door_control(1);
     digitalWrite(doorl,1);
     digitalWrite(fan,1);
     digitalWrite(buzzer,1);
     delay(750);
     digitalWrite(buzzer,0);
     delay(300);
     Serial.println(x);
     x=analogRead(smoke);
    }
     digitalWrite(doorl,0);
     digitalWrite(fan,fanf);
     digitalWrite(buzzer,0);
     door_control(doorf);
     Serial.println(x);
  }
}


void temp_reading(void)
{
  byte temperature = 0;
  byte humidity = 0;
  int err = SimpleDHTErrSuccess;
    lcd.clear();
    lcd.setCursor(0,0);
  if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    lcd.print("Reading failed");
  }
  else
  {
    lcd.print("Temp : ");
    lcd.print((int)temperature);
    lcd.print(" (C)"); 
    lcd.setCursor(0,1);
    lcd.print("humid : ");
    lcd.print((int)humidity);
    lcd.print(" (H)");
  }
  delay(5000);
}







void humi_send(void)
{
  // read without samples.
  byte temperature = 0;
  int x=0;
  byte humidity = 0;
  int err = SimpleDHTErrSuccess;
  if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
  }
  else
  {
    x=int(humidity);
  }
  Serial.print(x);
}



void temp_send(void)
{
  // read without samples.
  byte temperature = 0;
  int x=0;
  byte humidity = 0;
  int err = SimpleDHTErrSuccess;
  if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
  }
  else
  {
    x=int(temperature);
  }
  Serial.print(x);
}




void bluetooth(void)
{
 int incoming_value ;
 if(Serial.available()>0)
 {
    incoming_value =Serial.read();
    if(incoming_value=='1')
    {
        digitalWrite(leds,ledf);
        ledf=!ledf;
    }
    else if(incoming_value=='2')
    {
        digitalWrite(fan,fanf);
        fanf=!fanf;
    }
    else if(incoming_value=='3')
    {
        door_control(doorf);
        doorf=!doorf;
    }
    else if(incoming_value=='4')
    {
      temp_send();
    }
    else if(incoming_value=='5')
    {
      humi_send();
    }
    else if(incoming_value=='6')
    {
      sound();
    }
 }
}




void access (void)
{
  const String password = master;
  String password_entered = "";
  int flag=1;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Enter Password : ");
  while(flag)
  {
  lcd.setCursor(0,1);
  while(password_entered.length() <4)
  {
    char key = customKeypad.getKey ();
    state=digitalRead(sw);
    while(state==0)
    {
      digitalWrite(buzzer,1);
      state=digitalRead(sw);
    }
    digitalWrite(buzzer,0);
    bluetooth();
    smoke_test();
    if(key != NO_KEY)
    {
      password_entered += key;
      lcd.print('*');
      digitalWrite(buzzer,1);
      delay(100);
      digitalWrite(buzzer,0);
    }
  }
    if(password_entered == password)
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Password OK!");
      digitalWrite(buzzer,1);
      delay(750);
      digitalWrite(buzzer,0);
      delay(250);
      flag=0;
    }
    else
    {
      lcd.clear();
      lcd.print("Wrong Password!");
      for(int i=0 ; i<5 ; i++)
      {
      digitalWrite(buzzer,1);
      delay(100);
      digitalWrite(buzzer,0);
      delay(100);
      }
      password_entered = "";
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Enter Password : ");
    }
  }
}
