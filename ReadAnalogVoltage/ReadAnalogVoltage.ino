#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <k13.h>
#include <k13LCDChar.h>

#define melodyPin 9
#if defined(ARDUINO) && ARDUINO >= 100
#define printByte(args)  write(args);
#else
#define printByte(args)  print(args,BYTE);
#endif



LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 20 chars and 4 line display

int m7 = 0;
int pinData  = 2;
int clockPin = 3;
int latchPin = 4;
int buttonPushCounter = 0;   // counter for the number of button presses
int buttonState = 0;         // current state of the button
int lastButtonState = 0;   
int song = 0;
int Vo; // Integer value of voltage reading

double ff = 255;
double gg = 1023;
double ii = 0;
double l = 255;
double l1 = 255;
double m = 500;
double n = 0;
double o = 0;
double p = 2042;

long milos = 0;
long milos2 = 0;

float f2v = 0;
float R = 10000.0; // Fixed resistance in the voltage divider
float T = 0;

void setup() {
  
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  
  pinMode(7,INPUT); 
  pinMode(8,INPUT);
  pinMode(9,OUTPUT);
  
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
  
  pinMode(12,OUTPUT);
  pinMode(13,OUTPUT);  

  lcd.init(); 
  lcd.backlight();
  lcd.createChar(0, bell);
  lcd.createChar(1, note);
  lcd.createChar(2, clock);
  lcd.createChar(3, heart);
  lcd.createChar(4, duck);
  lcd.createChar(5, check);
  lcd.createChar(6, cross);
  lcd.createChar(7, retarrow);
  
  lcd.createChar(8, BordSmpHG);
  lcd.createChar(9, BordSmpBG);
  lcd.createChar(10, BordSmpHD);
  lcd.createChar(11, BordSmpBD);
  lcd.createChar(12, BordSmpH);
  lcd.createChar(13, BordSmpB);
  lcd.createChar(14, BordSmpMid);
  lcd.createChar(15, BordSmpG);
  lcd.createChar(16, BordSmpD);

  lcd.createChar(17, BordDblHG);
  lcd.createChar(18, BordDblBG);
  lcd.createChar(19, BordDblHD);
  lcd.createChar(20, BordDblBD);
  lcd.createChar(21, BordDblH);
  lcd.createChar(22, BordDblB);
  lcd.createChar(23, BordDblMid);
  lcd.createChar(24, BordDblG);
  lcd.createChar(25, BordDblD);
  Serial.begin(9600);
  lcd.home();
  lcd.setCursor(0, 0);
  lup();
//ini ethanol
}
void loop() 
{
    lup();//ethanol simul
    if (buttonPushCounter == 0)
        {
            Dplay();
         }
   if (buttonPushCounter == 1)
       {
            myMenu();
        }
   LedMeter();//vumetre à led vert orange rouge en fct luminosité
   LightOn();//mesure luminosité allumage phares
   MesurTemp();//mesure temp et garde en memoire valeur
   RfreshDp();//efface le contenu du LCD
   ButtMenu();//Change Mode affichage LCD avec bouton
}

void lup()//lis valeur potar, 
{
  float g =  analogRead(A1); 
  float ls = g*(255.0/1023.0);
  analogWrite(11, ls);//simule le signal du flex fuel sensor
  analogWrite(10, ls);//delivre un signal pwm correspondant à A1 
  if(g >= 500)
  {
     // Serial.println("0x01");
  }
}

void LedMeter()
{
  updateLeds(0);
}

void updateLeds( int iValeur )
{
  int yyh = analogRead(A3);
  float gul = 255.0/1023.0*analogRead(A3);
  if (yyh <=10)
  {
    gul = 0.0;  //Ouverture de la gâchette (Etape 1)
    Serial.println((int)yyh);
    digitalWrite(3, LOW);    
    shiftOut(2, 4, MSBFIRST, (int)gul);   //Injection d'une valeur (Etape 2)
    digitalWrite(3, HIGH);
  }
  if (yyh <=200 && yyh >10){ 
    gul = 1.0;  //Ouverture de la gâchette (Etape 1)
    //Serial.println((int)yyh);digitalWrite(3, LOW);
    shiftOut(2, 4, MSBFIRST, (int)gul);   //Injection d'une valeur (Etape 2)
    digitalWrite(3, HIGH);
       
  }
  if (yyh <=300 && yyh > 200){
    gul = 3.0;  //Ouverture de la gâchette (Etape 1)
    //Serial.println((int)yyh);
    digitalWrite(3, LOW);    
    shiftOut(2, 4, MSBFIRST, (int)gul);   //Injection d'une valeur (Etape 2)
    digitalWrite(3, HIGH);   
  }
  if (yyh <=400 && yyh > 300)
  {
    digitalWrite(3, LOW);    
    gul = 7.0;  //Ouverture de la gâchette (Etape 1)
    //Serial.println((int)yyh);
    shiftOut(2, 4, MSBFIRST, (int)gul);   //Injection d'une valeur (Etape 2)
    digitalWrite(3, HIGH);
  }
   if (yyh <=500 && yyh > 400)
   {
    gul = 15.0 ; 
    //Serial.println((int)yyh);
    digitalWrite(3, LOW);    
    shiftOut(2, 4, MSBFIRST, (int)gul);  
    digitalWrite(3, HIGH);
  }
   if (yyh >500 && yyh <=750  )
   {
    gul = 31 ; 
    //Serial.println((int)gul);
    digitalWrite(3, LOW);    
    shiftOut(2, 4, MSBFIRST, (int)gul);  
    digitalWrite(3, HIGH);
  }
  if (yyh >750 )
  {        
    gul = 127 ; 
    //Serial.println((int)gul);
    digitalWrite(3, LOW);
    shiftOut(2, 4, MSBFIRST, (int)gul);   
    digitalWrite(3, HIGH);
  }
 /* for (int iii = 0; iii < 256; iii =iii+iii+1)
    {
        digitalWrite(3, LOW);                //Ouverture de la gâchette (Etape 1)
        shiftOut(2, 4, MSBFIRST, iii);   //Injection d'une valeur (Etape 2)
        digitalWrite(3, HIGH);               //Fermeture de la gâchette (Etape 3)
       delay(50);
    }*/
  /*  for (int p = (int)gul; p >0; p =(p-1)/2)
    {
        digitalWrite(3, LOW);                //Ouverture de la gâchette (Etape 1)
        shiftOut(2, 4, MSBFIRST, p);   //Injection d'une valeur (Etape 2)
        digitalWrite(3, HIGH);               //Fermeture de la gâchette (Etape 3)
       delay(550);
    }*/
}

void LightOn()
{
  float rgm = analogRead(A3);
  rgm = 
/*  if (rgm <= 500.0)
  {
    //Alertz(); // alerte sonore allumage phares
    //  Serial.println("0x04"); //signale au logiciel Mode Nuit
    digitalWrite(12,HIGH);
  }else
    {
      digitalWrite(12,LOW);
    }*/
    Serial.println(rgm);
}

void MesurTemp()
{
        Vo = analogRead(A2);
        T= (5000.0/1023.0);
        T *= (float)Vo;
        T -= 500;
        T /=19.5;
}

void ButtMenu()//change Mode affichage avec bouton
{
  buttonState = digitalRead(8);
  // compare the buttonState to its previous state
  if (buttonState != lastButtonState) 
  {
    // if the state has changed, increment the counter
    if (buttonState == HIGH) 
    {
      // if the current state is HIGH then the button
      // wend from off to on:
      lcd.clear();
      buzz(9, 4180, 50);//beep 
      buttonPushCounter++;
      if (buttonPushCounter >= 2)
      {
        Alertz();//alerte fin de défilement
        buttonPushCounter = 0;
      }
      if (buttonPushCounter == 0)
      {
        //Serial.println("0x03"); //signale au logiciel Mode LCD 1
      }
      if (buttonPushCounter == 1)
      {
        //Serial.println("0x02"); //signale au logiciel Mode LCD 2
      }
    } 
  }
  // save the current state as the last state, 
  //for next time through the loop
  lastButtonState = buttonState;
}

void RfreshDp()
{
 if (millis()>=(milos + 3000))
 {
   milos = millis();
   lcd.clear();
 }
}

void Alertz()//3 beeps d'alerte
{
  buzz(9, 4180, 50);
  delay(40);
  buzz(9, 4180, 50);
  delay(40);
  buzz(9, 4180, 50);
}

void myMenu()
{
        lcd.setCursor(0, 0);
    lcd.print("Hello World:");
        lcd.setCursor(0, 1);
    lcd.print(millis());
        lcd.setCursor(19, 3);
    lcd.print(buttonPushCounter);
        lcd.setCursor(0, 2);
    lcd.print(analogRead(A3));
}

void Dplay()
{
    double t1 = pulseIn(7, HIGH);// Time how long it takes to go HIGH again
    double t2 = pulseIn(7, LOW); // and how long it takes to go low again.
    double t = t1 + t2;
    double f = 1000000.0 / t;
        n = (100/m)*f;
        o = (5/p)*t1;
       
        lcd.setCursor(0, 0);
    lcd.print("Hz:");
        lcd.setCursor(4, 0);
    lcd.print((int)f);
        lcd.setCursor(0, 1);
    lcd.print("ms:");
        lcd.setCursor(4, 1);
    lcd.print(t1);
        lcd.setCursor(9, 0);
    lcd.print("Eth:");
        lcd.setCursor(14, 0);
    lcd.print(n);
        lcd.setCursor(19, 0);
        lcd.print("%");
        lcd.setCursor(12, 1);
    lcd.print("VC:");
        lcd.setCursor(16, 1);
    lcd.print(o);
        lcd.setCursor(0, 2);
    lcd.print("IAT:");
        lcd.setCursor(5, 2);
    lcd.print(T);
        lcd.setCursor(10, 2);
    lcd.print("C");
        lcd.setCursor(19, 3);
    lcd.print(buttonPushCounter);

    /*int g = (int)n;
            int h = (int)o;
    //  analogWrite(6, g);
        
        if(h <=20){
            h = 20;
        }
    //  analogWrite(5, h);*/
    

  //lcd.setCursor(0, 0);
  //lcd.print(voltage);
  //delay(10);
}
void LCDSplash()
{
  /*
    lcd.createChar(0, bell);
  lcd.createChar(1, note);
  lcd.createChar(2, clock);
  lcd.createChar(3, heart);
  lcd.createChar(4, duck);
  lcd.createChar(5, check);
  lcd.createChar(6, cross);
  lcd.createChar(7, retarrow);
  
  lcd.createChar(8, BordSmpHG);
  lcd.createChar(9, BordSmpBG);
  lcd.createChar(10, BordSmpHD);
  lcd.createChar(11, BordSmpBD);
  lcd.createChar(12, BordSmpH);
  lcd.createChar(13, BordSmpB);
  lcd.createChar(14, BordSmpMid);
  lcd.createChar(15, BordSmpG);
  lcd.createChar(16, BordSmpD);

  */
  //********************cadre TABLEUR**********************
    lcd.setCursor(0, 0);//HG
    lcd.write(8);
        
        for(int gk = 1;gk<=18;gk++)//H
        {
          lcd.setCursor(gk, 0);
          lcd.write(12);
        }     
        
        lcd.setCursor(19, 0);//HD
        lcd.write(10);
        
        for(int gk = 1;gk<=2;gk++)//D
        {
          lcd.setCursor(19,gk);
          lcd.write(16);
        }  
        
        lcd.setCursor(19, 3);//BD
    lcd.write(11);

        for(int gk = 1;gk<=18;gk++)//B
        {
          lcd.setCursor(gk, 3);
          lcd.write(13);
        }
        
        lcd.setCursor(0, 3);//BG
    lcd.print(9);

         for(int gk = 1;gk<=2;gk++)//G
        {
          lcd.setCursor(0, gk);
          lcd.write(15);
        }
        //*********************************************
        
        lcd.setCursor(9, 0);
    lcd.print("Eth:");
        lcd.setCursor(14, 0);
    lcd.print(n);
        lcd.setCursor(19, 0);
        lcd.print("%");
        lcd.setCursor(12, 1);
    lcd.print("VC:");
        lcd.setCursor(16, 1);
    lcd.print(o);
        lcd.setCursor(0, 2);
    lcd.print("IAT:");
        lcd.setCursor(5, 2);
    lcd.print(T);
        lcd.setCursor(10, 2);
    lcd.print("C");
        lcd.setCursor(19, 3);
    lcd.print(buttonPushCounter);

    /*int g = (int)n;
            int h = (int)o;
    //  analogWrite(6, g);
        
        if(h <=20){
            h = 20;
        }
    //  analogWrite(5, h);*/
    

  //lcd.setCursor(0, 0);
  //lcd.print(voltage);
  //delay(10);
}
void buzz(int targetPin, long frequency, long length) //buzzer func
{
  long delayValue = 1000000/frequency/2;
  long numCycles = frequency * length/ 1000;
  for (long i=0; i < numCycles; i++)
  {
    digitalWrite(targetPin,HIGH);
    digitalWrite(13,HIGH);
    delayMicroseconds(delayValue);
    digitalWrite(targetPin,LOW);
    digitalWrite(13,LOW);
    delayMicroseconds(delayValue);
  }
}

void sing(int s)//Melodie
{      
   song = s;
   if(song==0)//Mario Melodie
    {
    int size = sizeof(melody) / sizeof(int);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Mario Melodie");
    lcd.setCursor(19, 0);
    lcd.write(0);
    for (int thisNote = 0; thisNote < size; thisNote++) 
      {
        int noteDuration = 1000/tempo[thisNote];
        buzz(melodyPin, melody[thisNote],noteDuration);
        int pauseBetweenNotes = noteDuration * 1.30;
        delay(pauseBetweenNotes);
        buzz(melodyPin, 0,noteDuration);
      }
    }
   if(song==1)//Mario Underworld
    {
     lcd.clear();
     lcd.setCursor(0, 0);
     lcd.print("Mario Underworld");
     int size = sizeof(underworld_melody) / sizeof(int);
     for (int thisNote = 0; thisNote < size; thisNote++) 
     {
         int noteDuration = 1000/underworld_tempo[thisNote];
         buzz(melodyPin, underworld_melody[thisNote],noteDuration);
         int pauseBetweenNotes = noteDuration * 1.30;
         delay(pauseBetweenNotes);
         buzz(melodyPin, 0,noteDuration);
      }
    }
  if(song==2)//Zelda Ullubulu
   {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Zelda Ullubulu");
  lcd.setCursor(19, 0);
  lcd.write(0);//affiche un cloche
  int size = sizeof(zelda_melody) / sizeof(int);
  for (int thisNote = 0; thisNote < size; thisNote++)
     {
       int noteDuration = 1000/zelda_tempo[thisNote];
       buzz(melodyPin, zelda_melody[thisNote],noteDuration);
       int pauseBetweenNotes = noteDuration * 1.30;
       delay(pauseBetweenNotes);
       buzz(melodyPin, 0,noteDuration);
    }
   }
  if(song==3)//STARWARS Melodie
   {
    int size = sizeof(starwars_melody) / sizeof(int);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("STARWARS Melodie");
    lcd.setCursor(19, 0);
    lcd.write(0);
    for (int thisNote = 0; thisNote < size; thisNote++) 
      {
        int noteDuration = 600/(starwars_tempo[thisNote]);
        buzz(melodyPin, starwars_melody[thisNote],noteDuration);
        int pauseBetweenNotes = noteDuration * 1.30;
        delay(pauseBetweenNotes);
        buzz(melodyPin, 0,noteDuration);
      }
    }
  if(song==4)//STARWARS Melodie
   {
    int size = sizeof(starwars_melody) / sizeof(int);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("STARWARS Melodie");
    lcd.setCursor(19, 0);
    lcd.write(0);
    for (int thisNote = 0; thisNote < size; thisNote++) 
      {
        int noteDuration = 600/(starwars_tempo[thisNote]);
        buzz(melodyPin, starwars_melody[thisNote],noteDuration);
        int pauseBetweenNotes = noteDuration * 1.30;
        delay(pauseBetweenNotes);
        buzz(melodyPin, 0,noteDuration);
      }
    }
    
}
