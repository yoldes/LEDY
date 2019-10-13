/*
  ______________________________________________________________________________
  
                    PROJET INFORMATIQUE - ENSIM 2A 2018/2019
                    
  ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
                          PABLO MARTIN ET NUMA GALIPOT
  ______________________________________________________________________________

  BIENVENU.E.S SUR LE CODE ARDUINO DE LEDY, LE CLAVIER QUI VOUS APPREND A JOUER.
  DES LEDs S'ALLUMENT, VOUS APPUYEZ. SOYEZ REACTIF !
  ------------------------------------------------------------------------------
  1) SELECTIONNEZ VOTRE MODE (LIBRE/APPRENTISSAGE)
  2) CHOISISSEZ VOTRE SON
  3) SELECTIONNEZ VOTRE CHANSON (SI MODE APPRENTISSAGE)
  4) CHOISISSEZ VOTRE TEMPO
  5) APPUYEZ SUR OK POUR JOUER ! LEDY READY !

*/

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//       BIBLIOTHEQUES         ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include <Adafruit_NeoPixel.h>
#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include <Wire.h>
#include <SeeedOLED.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//        DEFINITIONS          ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*----------------------------------------------TOUCHES CLAVIER-----------------------------------------------------*/

#define BUTTON_PIN_DO    A14              //ATTRIBUTION PINS ENTREES (TOUCHES)
#define BUTTON_PIN_DOs   A12
#define BUTTON_PIN_RE    A10
#define BUTTON_PIN_REs   A8
#define BUTTON_PIN_MI    A6
#define BUTTON_PIN_FA    33
#define BUTTON_PIN_FAs   12
#define BUTTON_PIN_SOL   31
#define BUTTON_PIN_SOLs  10
#define BUTTON_PIN_LA    8
#define BUTTON_PIN_LAs   6
#define BUTTON_PIN_SI    4
#define BUTTON_PIN_do    2


/*----------------------------------------------LEDs CLAVIER-----------------------------------------------------*/

#define PIXEL_PIN_DO    A15               //ATTRIBUTION PINS SORTIES (LEDs)
#define PIXEL_PIN_DOs   A13
#define PIXEL_PIN_RE    A11
#define PIXEL_PIN_REs   A9
#define PIXEL_PIN_MI    A7
#define PIXEL_PIN_FA    A5
#define PIXEL_PIN_FAs   13
#define PIXEL_PIN_SOL   A3
#define PIXEL_PIN_SOLs  11
#define PIXEL_PIN_LA    9
#define PIXEL_PIN_LAs   7
#define PIXEL_PIN_SI    5
#define PIXEL_PIN_do    3


/*----------------------------------------------CHOIX MODE-----------------------------------------------------*/

int selecteur_mode = 45;                //DEFINITION PIN SELECTEUR DU MODE


/*-------------------------------------------BOUTON DE VALIDATION OK / BOUTON DE PAUSE-----------------------------------------------------*/

int OKbouton = 43;                      //DEFINITION PIN BOUTON VALIDATION
int PAUSEbouton = 47;



/*----------------------------------------------TEMPO-----------------------------------------------------*/

int FADER_TEMPO = A4;                  //ATTRIBUTION PIN FADER TEMPO

float tempo_max = 120;
int valeur_fader_tempo;

float doublecroche = 1 / 4;
float croche = 1 / 2;
float crochepointee = croche + doublecroche;
float noire = 1;
float noirepointee = 1.5;
float blanche = 2;
float blanchepointee = 3;
float ronde = 4;

void tempo(float tempo_max) {          //DEFINITION TEMPO
  SeeedOled.clearDisplay();
  while (Ok() != 1){
    Ok();
    //valeur_fader_tempo = analogRead(FADER_TEMPO);            //LIRE 
    int pot_sons_chansons = A0;
    valeur_fader_tempo = analogRead(pot_sons_chansons);
    float BPM = (valeur_fader_tempo) * (tempo_max / 983.0);
       
    float tempo = 60000.0/BPM;
    doublecroche = tempo / 4;
    croche = tempo / 2;
    crochepointee = croche + doublecroche;
    noire = tempo * 1;
    noirepointee = tempo * 1.5;
    blanche = tempo * 2;
    blanchepointee = tempo * 3;
    ronde = tempo * 4;    
       
    SeeedOled.setTextXY(0, 0);
    SeeedOled.putString(" CHOIX DU TEMPO ");
    SeeedOled.setTextXY(1, 0);
    SeeedOled.putString("                ");
    SeeedOled.setTextXY(2, 0);
    SeeedOled.putString(" TEMPO :        ");
    SeeedOled.setTextXY(3, 5);
    SeeedOled.putFloat(int(BPM));        
  }
  SeeedOled.clearDisplay();
}


/*----------------------------------------------POTENTIOMETRE CHOIX-----------------------------------------------------*/

#define pot_sons_chansons A0                 //DEFINITIONS PIN POT CHOIX SON/CHANSON
#define ADC_REF 5
#define GROVE_VCC 5
#define ANGLE_MAX 300

float Angle(int Pin_Pot) {
  int valeur_potentiometre = analogRead(Pin_Pot);
  float voltage = (float)valeur_potentiometre * ADC_REF / 1023;
  float degres = (voltage * ANGLE_MAX) / GROVE_VCC;
  return degres;
}


/*----------------------------------------------LEDs CLAVIER-----------------------------------------------------*/

#define PIXEL_COUNT 60
Adafruit_NeoPixel stripDO = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN_DO, NEO_GRB + NEO_KHZ800);         //CODE PRIMAIRE D'ALLUMAGE DES LEDs
Adafruit_NeoPixel stripDOs = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN_DOs, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripRE = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN_RE, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripREs = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN_REs, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripMI = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN_MI, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripFA = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN_FA, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripFAs = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN_FAs, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripSOL = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN_SOL, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripSOLs = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN_SOLs, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripLA = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN_LA, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripLAs = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN_LAs, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripSI = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN_SI, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripdo = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN_do, NEO_GRB + NEO_KHZ800);


uint8_t color_pos = 85;                                        //COULEUR LEDs : 0=ROUGE / 30=JAUNE / 75=VERT / 120=BLEU / 220=VIOLET

uint32_t Wheel(byte WheelPos) {                                 //CODE COULEUR LEDs
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return stripDO.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return stripDO.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return stripDO.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

byte etatLedDO = LOW;                                            //ETAT LEDs
byte etatLedDOs = LOW;
byte etatLedRE = LOW;
byte etatLedREs = LOW;
byte etatLedMI = LOW;
byte etatLedFA = LOW;
byte etatLedFAs = LOW;
byte etatLedSOL = LOW;
byte etatLedSOLs = LOW;
byte etatLedLA = LOW;
byte etatLedLAs = LOW;
byte etatLedSI = LOW;
byte etatLeddo = LOW;


/*----------------------------------------------TOUCHES CLAVIER-----------------------------------------------------*/

float oldStateDO = 0;                                          //ANCIEN ETAT DES TOUCHES
float oldStateDOs = 0;
float oldStateRE = 0;
float oldStateREs = 0;
float oldStateMI = 0;
bool oldStateFA = LOW;
bool oldStateFAs = LOW;
bool oldStateSOL = LOW;
bool oldStateSOLs = LOW;
bool oldStateLA = LOW;
bool oldStateLAs = LOW;
bool oldStateSI = LOW;
bool oldStatedo = LOW;

unsigned long S = 0;
unsigned long dateDernierChangementDO = 0;                      //DATE DERNIER CHANGEMENTS TOUCHES
unsigned long dateDernierChangementDOs = 0;
unsigned long dateDernierChangementRE = 0;
unsigned long dateDernierChangementREs = 0;
unsigned long dateDernierChangementMI = 0;
unsigned long dateDernierChangementFA = 0;
unsigned long dateDernierChangementFAs = 0;
unsigned long dateDernierChangementSOL = 0;
unsigned long dateDernierChangementSOLs = 0;
unsigned long dateDernierChangementLA = 0;
unsigned long dateDernierChangementLAs = 0;
unsigned long dateDernierChangementSI = 0;
unsigned long dateDernierChangementdo = 0;


SoftwareSerial mySoftwareSerial(16, 17); // TX, RX                  //ATTRIBUTION PINS MODULE MP3
DFRobotDFPlayerMini myDFPlayer;                                     //https://www.dfrobot.com/wiki/index.php/DFPlayer_Mini_SKU:DFR0299
void printDetail(uint8_t type, int value);

int Son;                          //DEFINITION DE L'INSTRUMENT JOUE (Son = 1 = Piano, Son = 2 = Batterie)



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//        SETUP           /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup() {

  Serial.begin(9600);                              //DEFINITION DU DEBIT DES DONNEES ECHANGEES

  /*----------------------------------------------TOUCHES CLAVIER-----------------------------------------------------*/

  pinMode(BUTTON_PIN_DO, INPUT);                   //PINMODE TOUCHES CLAVIER
  pinMode(BUTTON_PIN_DOs, INPUT);
  pinMode(BUTTON_PIN_RE, INPUT);
  pinMode(BUTTON_PIN_REs, INPUT);
  pinMode(BUTTON_PIN_MI, INPUT);
  pinMode(BUTTON_PIN_FA, INPUT_PULLUP);
  pinMode(BUTTON_PIN_FAs, INPUT_PULLUP);
  pinMode(BUTTON_PIN_SOL, INPUT_PULLUP);
  pinMode(BUTTON_PIN_SOLs, INPUT_PULLUP);
  pinMode(BUTTON_PIN_LA, INPUT_PULLUP);
  pinMode(BUTTON_PIN_LAs, INPUT_PULLUP);
  pinMode(BUTTON_PIN_SI, INPUT_PULLUP);
  pinMode(BUTTON_PIN_do, INPUT_PULLUP);


  /*----------------------------------------------LEDs CLAVIER-----------------------------------------------------*/

  stripDO.begin();           //INITIALISATION FONCTION ALLUMAGE LED
  stripDO.clear();
  stripDO.show();
  stripDOs.begin();
  stripDOs.clear();
  stripDOs.show();
  stripRE.begin();
  stripRE.clear();
  stripRE.show();
  stripREs.begin();
  stripREs.clear();
  stripREs.show();
  stripMI.begin();
  stripMI.clear();
  stripMI.show();
  stripFA.begin();
  stripFA.clear();
  stripFA.show();
  stripFAs.begin();
  stripFAs.clear();
  stripFAs.show();
  stripSOL.begin();
  stripSOL.clear();
  stripSOL.show();
  stripSOLs.begin();
  stripSOLs.clear();
  stripSOLs.show();
  stripLA.begin();
  stripLA.clear();
  stripLA.show();
  stripLAs.begin();
  stripLAs.clear();
  stripLAs.show();
  stripSI.begin();
  stripSI.clear();
  stripSI.show();
  stripdo.begin();
  stripdo.clear();
  stripdo.show();


  /*----------------------------------------CHOIX MODE, CHOIX SON, CHANSON, OK, TEMPO-------------------------------------------------*/

  pinMode(selecteur_mode, INPUT);                                //DEFINITION PINMODE INTERRUPTEUR CHOIX MODE
  pinMode(pot_sons_chansons, INPUT);                             //DEFINITION PINMODE POT CHANSON
  pinMode(OKbouton, INPUT);                                      //DEFINITION PINMODE BOUTON VALIDATION
  pinMode(PAUSEbouton, INPUT);                                   //DEFINITION PINMODE BOUTON PAUSE
  pinMode(FADER_TEMPO, INPUT);                                   //DEFINITION PINMODE FADER TEMPO


 /*--------------------------------------------------------MODULE MP3----------------------------------------------------------------*/

  mySoftwareSerial.begin(9600);                                     //CONNECTION MODULE MP3
  Serial.println(F("INITIALISATION DU MODULE MP3"));
  myDFPlayer.begin(mySoftwareSerial);
  Serial.println(F("MODULE MP3 CONNECTE"));
  myDFPlayer.volume(30);                                      //VOLUME entre 0 et 30


 /*----------------------------------------------------------ECRAN----------------------------------------------------------------*/

  Wire.begin();                                               //INITIALISATION ECRAN
  SeeedOled.init();
  SeeedOled.clearDisplay();
  SeeedOled.setNormalDisplay();
  SeeedOled.setPageMode();
  SeeedOled.setTextXY(3, 0);
  SeeedOled.putString("   LEDY READY   ");                    //MESSAGE ECRAN INITIALISATION



 /*--------------------------------------------------------ANIMATION DES LEDs CLAVIER DEMARRAGE----------------------------------------------------------------*/

  bienvenus();                                                //FONCTION DE DEMARRAGE : ALLUMAGE LEDs LES UNES APRES LES AUTRES
}





////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//        LOOP          ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void loop() {

  Choix_Mode();
  
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//         FONCTIONS           //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void decompte1(){
  allumeTOUT();
  SeeedOled.setTextXY(0, 0);
  SeeedOled.putString("                ");
  SeeedOled.setTextXY(1, 0);
  SeeedOled.putString("       /|       ");
  SeeedOled.setTextXY(2, 0);
  SeeedOled.putString("      / |       ");
  SeeedOled.setTextXY(3, 0);
  SeeedOled.putString("        |       ");
  SeeedOled.setTextXY(4, 0);
  SeeedOled.putString("        |       ");
  SeeedOled.setTextXY(5, 0);
  SeeedOled.putString("                ");
}

void decompte2(){
  allumeTOUT();
  SeeedOled.setTextXY(0, 0);
  SeeedOled.putString("       __        ");
  SeeedOled.setTextXY(1, 0);
  SeeedOled.putString("      /  |      ");
  SeeedOled.setTextXY(2, 0);
  SeeedOled.putString("        /       ");
  SeeedOled.setTextXY(3, 0);
  SeeedOled.putString("       /        ");
  SeeedOled.setTextXY(4, 0);
  SeeedOled.putString("      /___      ");
  SeeedOled.setTextXY(5, 0);
  SeeedOled.putString("                ");
}

void decompte3(){
  allumeTOUT();
  SeeedOled.setTextXY(0, 0);
  SeeedOled.putString("       __       ");
  SeeedOled.setTextXY(1, 0);
  SeeedOled.putString("      /  |      ");
  SeeedOled.setTextXY(2, 0);
  SeeedOled.putString("        _       ");
  SeeedOled.setTextXY(3, 0);
  SeeedOled.putString("         |      ");
  SeeedOled.setTextXY(4, 0);
  SeeedOled.putString("      ___|      ");
  SeeedOled.setTextXY(5, 0);
  SeeedOled.putString("                ");
}
  
void decompte4(){
  allumeTOUT(); 
  SeeedOled.setTextXY(0, 0);
  SeeedOled.putString("                ");
  SeeedOled.setTextXY(1, 0);
  SeeedOled.putString("        /|      ");
  SeeedOled.setTextXY(2, 0);
  SeeedOled.putString("       / |      ");
  SeeedOled.setTextXY(3, 0);
  SeeedOled.putString("      /__|_     ");
  SeeedOled.setTextXY(4, 0);
  SeeedOled.putString("         |      ");
  SeeedOled.setTextXY(5, 0);
  SeeedOled.putString("                ");
}




bool Pause(){
  return digitalRead(PAUSEbouton);
}
bool Ok(){
  return digitalRead(OKbouton);
}



/*--------------------------------------------------------ANIMATION DES LEDs CLAVIER DEMARRAGE----------------------------------------------------------------*/

void bienvenus() {
  int Son = 1;
  color_pos = 0;
  allumeDO(0, Son);
  color_pos = 20;
  allumeDOs(0, Son);
  color_pos = 40;
  allumeRE(0, Son);
  color_pos = 60;
  allumeREs(0, Son);
  color_pos = 80;
  allumeMI(0, Son);
  color_pos = 100;
  allumeFA(0, Son);
  color_pos = 120;
  allumeFAs(0, Son);
  color_pos = 140;
  allumeSOL(0, Son);
  color_pos = 160;
  allumeSOLs(0, Son);
  color_pos = 180;
  allumeLA(50, Son);
  color_pos = 200;
  allumeLAs(0, Son);
  color_pos = 220;
  allumeSI(0, Son);
  color_pos = 240;
  allumedo(0, Son);
  color_pos = 254;
  allumeSI(0, Son);
  color_pos = 240;
  allumeLAs(0, Son);
  color_pos = 220;
  allumeLA(0, Son);
  color_pos = 200;
  allumeSOLs(0, Son);
  color_pos = 180;
  allumeSOL(0, Son);
  color_pos = 160;
  allumeFAs(0, Son);
  color_pos = 140;
  allumeFA(0, Son);
  color_pos = 120;
  allumeMI(0, Son);
  color_pos = 100;
  allumeREs(0, Son);
  color_pos = 80;
  allumeRE(0, Son);
  color_pos = 60;
  allumeDOs(0, Son);
  color_pos = 40;
  allumeDO(0, Son);
}


 /*--------------------------------------------------------FONCTION CLAVIER----------------------------------------------------------------*/

void clavier(int Son) {

  float newStateDO = analogRead(BUTTON_PIN_DO);                              //LECTURE ETAT DES TOUCHES
  float newStateDOs = analogRead(BUTTON_PIN_DOs);
  float newStateRE = analogRead(BUTTON_PIN_RE);
  float newStateREs = analogRead(BUTTON_PIN_REs);
  float newStateMI = analogRead(BUTTON_PIN_MI);
  bool newStateFA = digitalRead(BUTTON_PIN_FA);
  bool newStateFAs = digitalRead(BUTTON_PIN_FAs);
  bool newStateSOL = digitalRead(BUTTON_PIN_SOL);
  bool newStateSOLs = digitalRead(BUTTON_PIN_SOLs);
  bool newStateLA = digitalRead(BUTTON_PIN_LA);
  bool newStateLAs = digitalRead(BUTTON_PIN_LAs);
  bool newStateSI = digitalRead(BUTTON_PIN_SI);
  bool newStatedo = digitalRead(BUTTON_PIN_do);

  if (newStateDO >= 100 && oldStateDO == 0) {                            //APPUI SUR LA TOUCHE DO
    newStateDO = analogRead(BUTTON_PIN_DO);
    if (newStateDO >= 10) {
      myDFPlayer.playFolder(Son, 1);      //Lit le son 1 (= DO) dans le fichier Son.
    }  
  }
  if (newStateDOs >= 100 && oldStateDOs == 0) {                          //APPUI SUR LA TOUCHE DO#
    newStateDOs = analogRead(BUTTON_PIN_DOs);
    if (newStateDOs >= 10) {
      myDFPlayer.playFolder(Son, 2);      //Lit le son 2 (= DO#) dans le fichier Son.
    }
  }
  if (newStateRE >= 100 && oldStateRE == 0) {                            //APPUI SUR LA TOUCHE RE
    newStateRE = analogRead(BUTTON_PIN_RE);
    if (newStateRE >= 10) {
      myDFPlayer.playFolder(Son, 3);
    }
  }
  if (newStateREs >= 100 && oldStateREs == 0) {                          //APPUI SUR LA TOUCHE RE#
    newStateREs = analogRead(BUTTON_PIN_REs);
    if (newStateREs >= 10) {
      myDFPlayer.playFolder(Son, 4);
    }
  }
  if (newStateMI >= 100 && oldStateMI == 0) {                            //APPUI SUR LA TOUCHE MI
    newStateMI = analogRead(BUTTON_PIN_MI);
    if (newStateMI >= 10) {
      myDFPlayer.playFolder(Son, 5);
    }
  }
  if (newStateFA == HIGH && oldStateFA == LOW) {                          //APPUI SUR LA TOUCHE FA
    newStateFA = digitalRead(BUTTON_PIN_FA);
    if (newStateFA == HIGH) {
      myDFPlayer.playFolder(Son, 6);
    }
  }
  if (newStateFAs == HIGH && oldStateFAs == LOW) {                 //APPUI SUR LA TOUCHE FA#
    newStateFAs = digitalRead(BUTTON_PIN_FAs);
    if (newStateFAs == HIGH) {
      myDFPlayer.playFolder(Son, 7);
    }
  }
  if (newStateSOL == HIGH && oldStateSOL == LOW) {                //APPUI SUR LA TOUCHE SOL
    newStateSOL = digitalRead(BUTTON_PIN_SOL);
    if (newStateSOL == HIGH) {
      myDFPlayer.playFolder(Son, 8);
    }
  }
  if (newStateSOLs == HIGH && oldStateSOLs == LOW) {              //APPUI SUR LA TOUCHE SOL#
    newStateSOLs = digitalRead(BUTTON_PIN_SOLs);
    if (newStateSOLs == HIGH) {
      myDFPlayer.playFolder(Son, 9);
    }
  }
  if (newStateLA == HIGH && oldStateLA == LOW) {               //APPUI SUR LA TOUCHE LA
    newStateLA = digitalRead(BUTTON_PIN_LA);
    if (newStateLA == HIGH) {
      myDFPlayer.playFolder(Son, 10);
    }
  }
  if (newStateLAs == HIGH && oldStateLAs == LOW) {               //APPUI SUR LA TOUCHE LAs
    newStateLAs = digitalRead(BUTTON_PIN_LAs);
    if (newStateLAs == HIGH) {
      myDFPlayer.playFolder(Son, 11);
    }
  }
  if (newStateSI == HIGH && oldStateSI == LOW) {               //APPUI SUR LA TOUCHE SI
    newStateSI = digitalRead(BUTTON_PIN_SI);
    if (newStateSI == HIGH) {
      myDFPlayer.playFolder(Son, 12);
    }
  }
  if (newStatedo == HIGH && oldStatedo == LOW) {              //APPUI SUR LA TOUCHE do
    newStatedo = digitalRead(BUTTON_PIN_do);
    if (newStatedo == HIGH) {
      myDFPlayer.playFolder(Son, 13);
    }
  }

  oldStateDO = newStateDO;                             //EGALISATION DES ETATS DES TOUCHES
  oldStateDOs = newStateDOs;
  oldStateRE = newStateRE;
  oldStateREs = newStateREs;
  oldStateMI = newStateMI;
  oldStateFA = newStateFA;
  oldStateFAs = newStateFAs;
  oldStateSOL = newStateSOL;
  oldStateSOLs = newStateSOLs;
  oldStateLA = newStateLA;
  oldStateLAs = newStateLAs;
  oldStateSI = newStateSI;
  oldStatedo = newStatedo;

  if (Pause() == 1){                         //PAUSE PUIS REPRENDRE, CHANGER DE TEMPO, RETOUR AU MENU PRINCIPAL
    delay(200);
    int k;
    while (Ok() != 1) {
      Ok();
      float angle = Angle(pot_sons_chansons);
      if (angle >= 0 && angle < 100) {
        SeeedOled.setTextXY(0, 0);
        SeeedOled.putString("      PAUSE     ");
        SeeedOled.setTextXY(1, 0);
        SeeedOled.putString("                ");
        SeeedOled.setTextXY(2, 0);
        SeeedOled.putString("Reprendre    <<<");
        SeeedOled.setTextXY(3, 0);
        SeeedOled.putString("                ");
        SeeedOled.setTextXY(4, 0);
        SeeedOled.putString("Changer tempo   ");
        SeeedOled.setTextXY(5, 0);
        SeeedOled.putString("                ");
        SeeedOled.setTextXY(6, 0);
        SeeedOled.putString("Revenir au menu ");
        k = 1;
      } 
      if (angle >= 100 && angle < 200) {
        SeeedOled.setTextXY(0, 0);
        SeeedOled.putString("      PAUSE     ");
        SeeedOled.setTextXY(1, 0);
        SeeedOled.putString("                ");
        SeeedOled.setTextXY(2, 0);
        SeeedOled.putString("Reprendre       ");
        SeeedOled.setTextXY(3, 0);
        SeeedOled.putString("                ");
        SeeedOled.setTextXY(4, 0);
        SeeedOled.putString("Changer tempo<<<");
        SeeedOled.setTextXY(5, 0);
        SeeedOled.putString("                ");
        SeeedOled.setTextXY(6, 0);
        SeeedOled.putString("Revenir au menu ");
        k = 2;
      }
      if (angle >= 200 && angle <= 300) {
        SeeedOled.setTextXY(0, 0);
        SeeedOled.putString("      PAUSE     ");
        SeeedOled.setTextXY(1, 0);
        SeeedOled.putString("                ");
        SeeedOled.setTextXY(2, 0);
        SeeedOled.putString("Reprendre       ");
        SeeedOled.setTextXY(3, 0);
        SeeedOled.putString("                ");
        SeeedOled.setTextXY(4, 0);
        SeeedOled.putString("Changer tempo   ");
        SeeedOled.setTextXY(5, 0);
        SeeedOled.putString("                ");
        SeeedOled.setTextXY(6, 0);
        SeeedOled.putString("Revenir au me<<<");
        k = 3;
      }
    }
    if (k == 1) {
      SeeedOled.clearDisplay();
    }
    if (k == 2) {
      delay(1000);
      tempo_max = 140;
      tempo(tempo_max);
      SeeedOled.clearDisplay();   
    }
    if (k == 3) {
      SeeedOled.clearDisplay();
      eteintTOUT();
      Choix_Mode();
    }     
  }
}



 /*--------------------------------------------------------*LEDs CLAVIER----------------------------------------------------------------*/

void eteintTOUT() {                                                      //FONCTION QUI ETEINT TOUTES LES LEDs
  etatLedDO = LOW;
  stripDO.setPixelColor(0, 0, 0, 0);
  stripDO.show();
  etatLedDOs = LOW;
  stripDOs.setPixelColor(0, 0, 0, 0);
  stripDOs.show();
  etatLedRE = LOW;
  stripRE.setPixelColor(0, 0, 0, 0);
  stripRE.show();
  etatLedREs = LOW;
  stripREs.setPixelColor(0, 0, 0, 0);
  stripREs.show();
  etatLedMI = LOW;
  stripMI.setPixelColor(0, 0, 0, 0);
  stripMI.show();
  etatLedFA = LOW;
  stripFA.setPixelColor(0, 0, 0, 0);
  stripFA.show();
  etatLedFAs = LOW;
  stripFAs.setPixelColor(0, 0, 0, 0);
  stripFAs.show();
  etatLedSOL = LOW;
  stripSOL.setPixelColor(0, 0, 0, 0);
  stripSOL.show();
  etatLedSOLs = LOW;
  stripSOLs.setPixelColor(0, 0, 0, 0);
  stripSOLs.show();
  etatLedLA = LOW;
  stripLA.setPixelColor(0, 0, 0, 0);
  stripLA.show();
  etatLedLAs = LOW;
  stripLAs.setPixelColor(0, 0, 0, 0);
  stripLAs.show();
  etatLedSI = LOW;
  stripSI.setPixelColor(0, 0, 0, 0);
  stripSI.show();
  etatLeddo = LOW;
  stripdo.setPixelColor(0, 0, 0, 0);
  stripdo.show();
}

void silence(unsigned long S, unsigned long TempsAttente, int Son) {                 //FONCTION D'ATTENTE ENTRE L'ALLUMAGE DE DEUX NOTES :
  unsigned long dateCourante = millis();                                                         //Ex : silence(S, noire, Son);
  eteintTOUT();                                                                                  //--> Toutes les leds s'éteignent pendant la duree d'une noire.
  dateCourante = millis();
  S = dateCourante;
  while ((dateCourante - S) < TempsAttente) {
    dateCourante = millis();
    clavier(Son);
  }
}



void allumeTOUT() {                                                      //FONCTION QUI ETEINT TOUTES LES LEDs
  etatLedDO = HIGH;
  stripDO.setPixelColor(0, Wheel(color_pos));
  stripDO.show();
  etatLedDOs = HIGH;
  stripDOs.setPixelColor(0, Wheel(color_pos));
  stripDOs.show();
  etatLedRE = HIGH;
  stripRE.setPixelColor(0, Wheel(color_pos));
  stripRE.show();
  etatLedREs = HIGH;
  stripREs.setPixelColor(0, Wheel(color_pos));
  stripREs.show();
  etatLedMI = HIGH;
  stripMI.setPixelColor(0, Wheel(color_pos));
  stripMI.show();
  etatLedFA = HIGH;
  stripFA.setPixelColor(0, Wheel(color_pos));
  stripFA.show();
  etatLedFAs = HIGH;
  stripFAs.setPixelColor(0, Wheel(color_pos));
  stripFAs.show();
  etatLedSOL = HIGH;
  stripSOL.setPixelColor(0, Wheel(color_pos));
  stripSOL.show();
  etatLedSOLs = HIGH;
  stripSOLs.setPixelColor(0, Wheel(color_pos));
  stripSOLs.show();
  etatLedLA = HIGH;
  stripLA.setPixelColor(0, Wheel(color_pos));
  stripLA.show();
  etatLedLAs = HIGH;
  stripLAs.setPixelColor(0, Wheel(color_pos));
  stripLAs.show();
  etatLedSI = HIGH;
  stripSI.setPixelColor(0, Wheel(color_pos));
  stripSI.show();
  etatLeddo = HIGH;
  stripdo.setPixelColor(0, Wheel(color_pos));
  stripdo.show();
}

void allumeDO(int dureeDO, int Son) {                                                 //FONCTION D'ALLUMAGE DE LA LED DO PENDANT dureeDO
  unsigned long dateCourante = millis();                                                      //Ex : allumeDO(noire);
  unsigned long intervalleDOs = dateCourante - dateDernierChangementDOs;                      //--> Allumera la led DO pendant la durée d'une noire
  if (etatLedDO == LOW) {                                                                     
    etatLedDO = HIGH;
    stripDO.setPixelColor(0, Wheel(color_pos));
    stripDO.show();
    dateCourante = millis();
    dateDernierChangementDO = dateCourante;
  }
  while ((dateCourante - dateDernierChangementDO) < dureeDO) {
    dateCourante = millis();
    clavier(Son);
  }
  silence(S, 10, Son);
}

void allumeDOs(int dureeDOs, int Son) {
  unsigned long dateCourante = millis();
  unsigned long intervalleDOs = dateCourante - dateDernierChangementDOs;
  if (etatLedDOs == LOW) {
    etatLedDOs = HIGH;
    stripDOs.setPixelColor(0, Wheel(color_pos));
    stripDOs.show();
    dateCourante = millis();
    dateDernierChangementDOs = dateCourante;
  }
  while ((dateCourante - dateDernierChangementDOs) < dureeDOs) {
    dateCourante = millis();
    clavier(Son);
  }
  silence(S, 10, Son);
}

void allumeRE(int dureeRE, int Son) {
  unsigned long dateCourante = millis();
  unsigned long intervalleRE = dateCourante - dateDernierChangementRE;
  if (etatLedRE == LOW) {
    etatLedRE = HIGH;
    stripRE.setPixelColor(0, Wheel(color_pos));
    stripRE.show();
    dateCourante = millis();
    dateDernierChangementRE = dateCourante;
  }
  while ((dateCourante - dateDernierChangementRE) < dureeRE) {
    dateCourante = millis();
    clavier(Son);
  }
  silence(S, 10, Son);
}

void allumeREs(int dureeREs, int Son) {
  unsigned long dateCourante = millis();
  unsigned long intervalleREs = dateCourante - dateDernierChangementREs;
  if (etatLedREs == LOW) {
    etatLedREs = HIGH;
    stripREs.setPixelColor(0, Wheel(color_pos));
    stripREs.show();
    dateCourante = millis();
    dateDernierChangementREs = dateCourante;
  }
  while ((dateCourante - dateDernierChangementREs) < dureeREs) {
    dateCourante = millis();
    clavier(Son);
  }
  silence(S, 10, Son);
}

void allumeMI(int dureeMI, int Son) {
  unsigned long dateCourante = millis();
  unsigned long intervalleMI = dateCourante - dateDernierChangementMI;
  if (etatLedMI == LOW) {
    etatLedMI = HIGH;
    stripMI.setPixelColor(0, Wheel(color_pos));
    stripMI.show();
    dateCourante = millis();
    dateDernierChangementMI = dateCourante;
  }
  while ((dateCourante - dateDernierChangementMI) < dureeMI) {
    dateCourante = millis();
    clavier(Son);
  }
  silence(S, 10, Son);
}

void allumeFA(int dureeFA, int Son) {
  unsigned long dateCourante = millis();
  unsigned long intervalleFA = dateCourante - dateDernierChangementFA;
  if (etatLedFA == LOW) {
    etatLedFA = HIGH;
    stripFA.setPixelColor(0, Wheel(color_pos));
    stripFA.show();
    dateCourante = millis();
    dateDernierChangementFA = dateCourante;
  }
  while ((dateCourante - dateDernierChangementFA) < dureeFA) {
    dateCourante = millis();
    clavier(Son);
  }
  silence(S, 10, Son);
}

void allumeFAs(int dureeFAs, int Son) {
  unsigned long dateCourante = millis();
  unsigned long intervalleFAs = dateCourante - dateDernierChangementFAs;
  if (etatLedFAs == LOW) {
    etatLedFAs = HIGH;
    stripFAs.setPixelColor(0, Wheel(color_pos));
    stripFAs.show();
    dateCourante = millis();
    dateDernierChangementFAs = dateCourante;
  }
  while ((dateCourante - dateDernierChangementFAs) < dureeFAs) {
    dateCourante = millis();
    clavier(Son);
  }
  silence(S, 10, Son);
}

void allumeSOL(int dureeSOL, int Son) {
  unsigned long dateCourante = millis();
  unsigned long intervalleSOL = dateCourante - dateDernierChangementSOL;
  if (etatLedSOL == LOW) {
    etatLedSOL = HIGH;
    stripSOL.setPixelColor(0, Wheel(color_pos));
    stripSOL.show();
    dateCourante = millis();
    dateDernierChangementSOL = dateCourante;
  }
  while ((dateCourante - dateDernierChangementSOL) < dureeSOL) {
    dateCourante = millis();
    clavier(Son);
  }
  silence(S, 10, Son);
}

void allumeSOLs(int dureeSOLs, int Son) {
  unsigned long dateCourante = millis();
  unsigned long intervalleSOLs = dateCourante - dateDernierChangementSOLs;
  if (etatLedSOLs == LOW) {
    etatLedSOLs = HIGH;
    stripSOLs.setPixelColor(0, Wheel(color_pos));
    stripSOLs.show();
    dateCourante = millis();
    dateDernierChangementSOLs = dateCourante;
  }
  while ((dateCourante - dateDernierChangementSOLs) < dureeSOLs) {
    dateCourante = millis();
    clavier(Son);
  }
  silence(S, 10, Son);
}

void allumeLA(int dureeLA, int Son) {
  unsigned long dateCourante = millis();
  unsigned long intervalleLA = dateCourante - dateDernierChangementLA;
  if (etatLedLA == LOW ) {
    etatLedLA = HIGH;
    stripLA.setPixelColor(0, Wheel(color_pos));
    stripLA.show();
    dateCourante = millis();
    dateDernierChangementLA = dateCourante;
  }
  while ((dateCourante - dateDernierChangementLA) < dureeLA) {
    dateCourante = millis();
    clavier(Son);
  }
  silence(S, 10, Son);
}

void allumeLAs(int dureeLAs, int Son) {
  unsigned long dateCourante = millis();
  unsigned long intervalleLAs = dateCourante - dateDernierChangementLAs;
  if (etatLedLAs == LOW) {
    etatLedLAs = HIGH;
    stripLAs.setPixelColor(0, Wheel(color_pos));
    stripLAs.show();
    dateCourante = millis();
    dateDernierChangementLAs = dateCourante;
  }
  while ((dateCourante - dateDernierChangementLAs) < dureeLAs) {
    dateCourante = millis();
    clavier(Son);
  }
  silence(S, 10, Son);
}

void allumeSI(int dureeSI, int Son) {
  unsigned long dateCourante = millis();
  unsigned long intervalleSI = dateCourante - dateDernierChangementSI;
  if (etatLedSI == LOW) {
    etatLedSI = HIGH;
    stripSI.setPixelColor(0, Wheel(color_pos));
    stripSI.show();
    dateCourante = millis();
    dateDernierChangementSI = dateCourante;
  }
  while ((dateCourante - dateDernierChangementSI) < dureeSI) {
    dateCourante = millis();
    clavier(Son);
  }
  silence(S, 10, Son);
}

void allumedo(int dureedo, int Son) {
  unsigned long dateCourante = millis();
  unsigned long intervalledo = dateCourante - dateDernierChangementdo;
  if (etatLeddo == LOW) {
    etatLeddo = HIGH;
    stripdo.setPixelColor(0, Wheel(color_pos));
    stripdo.show();
    dateCourante = millis();
    dateDernierChangementdo = dateCourante;
  }
  while ((dateCourante - dateDernierChangementdo) < dureedo) {
    dateCourante = millis();
    clavier(Son);
  }
  silence(S, 10, Son);
}


/*--------------------------------------------------------CHANSON 1 ; J'AI DU BON TABAC----------------------------------------------------------------*/

void chanson1(int Son) {                                       //CODE DE LA CHANSON n°1 QUE LE JOUEUR A SELECTIONNE : "J'ai du bon tabac".

  tempo_max = 120.0;
  tempo(tempo_max);

  color_pos = random(0, 200);

  myDFPlayer.playFolder(2, 3);
  decompte1();
  silence(S, noire, Son);
  myDFPlayer.playFolder(2, 3);
  decompte2();
  silence(S, noire, Son);
  myDFPlayer.playFolder(2, 3);
  decompte3();
  silence(S, noire, Son);
  myDFPlayer.playFolder(2, 3);
  decompte4();
  silence(S, noire, Son);
  SeeedOled.clearDisplay();



  allumeDO(noire, Son);
  allumeRE(noire, Son);
  allumeMI(noire, Son);
  allumeDO(noire, Son);

  allumeRE(blanche, Son);
  allumeRE(noire, Son);
  allumeMI(noire, Son);

  allumeFA(blanche, Son);
  allumeFA(blanche, Son);


  allumeMI(blanche, Son);
  allumeMI(blanche, Son);


  allumeDO(noire, Son);
  allumeRE(noire, Son);
  allumeMI(noire, Son);
  allumeDO(noire, Son);

  allumeRE(blanche, Son);
  allumeRE(noire, Son);
  allumeMI(noire, Son);

  allumeFA(blanche, Son);
  allumeSOL(blanche, Son);

  allumeDO(ronde, Son);
  allumeSOL(blanche, Son);
  allumeSOL(noire, Son);
  allumeFA(noire, Son);
  allumeMI(blanche, Son);
  allumeRE(noire, Son);
  allumeMI(noire, Son);
  allumeFA(blanche, Son);
  allumeSOL(blanche, Son);
  allumeRE(ronde, Son);
  allumeSOL(blanche, Son);
  allumeSOL(noire, Son);
  allumeFA(noire, Son);
  allumeMI(blanche, Son);
  allumeRE(noire, Son);
  allumeMI(noire, Son);
  Serial.println(noire);
  allumeFA(blanche, Son);
  
  allumeSOL(blanche, Son);
  allumeDO(ronde, Son);


  silence(S, blanche, Son);
  
  chanson1(Son);
}


/*--------------------------------------------------------CHANSON 2 ; EVERYTHING NOW - ARCADE FIRE----------------------------------------------------------------*/

void chanson2(int Son) {                                    //CODE DE LA CHANSON n°2 QUE LE JOUEUR A SELECTIONNE : Everything Now - Arcade Fire

  tempo_max = 120.0;
  tempo(tempo_max);

  color_pos = random(0, 200);

  myDFPlayer.playFolder(2, 3);
  decompte1();
  silence(S, noire, Son);
  myDFPlayer.playFolder(2, 3);
  decompte2();
  silence(S, noire, Son);
  myDFPlayer.playFolder(2, 3);
  decompte3();
  silence(S, noire, Son);
  myDFPlayer.playFolder(2, 3);
  decompte4();
  silence(S, noire, Son);
  SeeedOled.clearDisplay();


  allumeDO(noire, Son);
  allumeRE(noire, Son);
  allumeMI(noire, Son);
  allumeFA(noire, Son);

  allumeSOL(blanche, Son);
  allumeFA(croche, Son);
  allumeMI(croche, Son);
  allumeFA(blanche, Son);
  allumeFA(crochepointee, Son);
  allumeFA(crochepointee, Son);
  allumeFA(crochepointee, Son);


  allumeFA(blanche, Son);
  allumeMI(croche, Son);
  allumeRE(croche, Son);
  allumeMI(blanche, Son);

  allumeFA(crochepointee, Son);
  allumeMI(crochepointee, Son);
  allumeRE(crochepointee, Son);
  allumeRE(crochepointee, Son);
  allumeDO(blanche, Son);

  allumeDO(ronde, Son);

  allumeDO(noire, Son);
  allumeRE(noire, Son);
  allumeMI(noire, Son);
  allumeFA(noire, Son);
  allumeSOL(blanche, Son);
  allumeFA(croche, Son);
  allumeMI(croche, Son);
  allumeFA(blanche, Son);
  allumeFA(crochepointee, Son);
  allumeFA(crochepointee, Son);
  allumeFA(crochepointee, Son);

  allumeFA(blanche, Son);
  allumeMI(croche, Son);
  allumeRE(croche, Son);
  allumeMI(blanche, Son);
  allumeFA(crochepointee, Son);
  allumeMI(crochepointee, Son);
  allumeRE(crochepointee, Son);
  allumeRE(crochepointee, Son);
  allumeDO(blanche, Son);
  allumeDO(ronde, Son);

  silence(S, blanche, Son);


  chanson2(Son);
}


/*--------------------------------------------------------CHANSON 1 ; BEAT----------------------------------------------------------------*/

void chanson3(int Son) {                                                             //CODE DE LA CHANSON n°2 QUE LE JOUEUR A SELECTIONNE

  tempo_max = 120.0;
  tempo(tempo_max);

  color_pos = random(0, 200);
  
  myDFPlayer.playFolder(2, 3);
  decompte1();
  silence(S, noire, Son);
  myDFPlayer.playFolder(2, 3);
  decompte2();
  silence(S, noire, Son);
  myDFPlayer.playFolder(2, 3);
  decompte3();
  silence(S, noire, Son);
  myDFPlayer.playFolder(2, 3);
  decompte4();
  silence(S, noire, Son);
  SeeedOled.clearDisplay();
  



  allumeDO(noire, Son);
  allumeRE(noire, Son);
  allumeMI(noire, Son);
  allumeDO(noire, Son);

  allumeRE(blanche, Son);
  allumeRE(noire, Son);
  allumeMI(noire, Son);
  allumeFA(blanche, Son);
  allumeFA(blanche, Son);
  allumeMI(blanche, Son);
  allumeMI(blanche, Son);


  allumeDO(noire, Son);
  allumeRE(noire, Son);
  allumeMI(noire, Son);
  allumeDO(noire, Son);
  allumeRE(blanche, Son);
  allumeRE(noire, Son);
  allumeMI(noire, Son);


  allumeFA(blanche, Son);
  allumeSOL(blanche, Son);
  allumeDO(ronde, Son);


  allumeSOL(blanche, Son);
  allumeSOL(noire, Son);
  allumeFA(noire, Son);

  allumeMI(blanche, Son);
  allumeRE(noire, Son);
  allumeMI(noire, Son);
  allumeFA(blanche, Son);
  allumeSOL(blanche, Son);


  allumeRE(ronde, Son);

  
  
  allumeSOL(blanche, Son);
  allumeSOL(noire, Son);
  allumeFA(noire, Son);
  allumeMI(blanche, Son);
  allumeRE(noire, Son);
  allumeMI(noire, Son);


  allumeFA(blanche, Son);
  allumeSOL(blanche, Son);
  allumeDO(ronde, Son);


  

  silence(S, blanche, Son);
  
  chanson3(Son);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// |\/| [¯ |\ | |  | : Choix Mode  > Libre         > Choix Son > Clavier(Son)                                      ///////////////////////////////////////////
// |  | [_ | \| |__|               > Apprentissage > Choix Son > Choix Chanson > Allumenote(Son) > Clavier(Son)    ///////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void Choix_Mode() {
  int mode;
  Ok();
  while (Ok() != 1) {
    Ok();
    if (digitalRead(selecteur_mode) == 1) {
      SeeedOled.setTextXY(0, 0);
      SeeedOled.putString("   CHOIX MODE   ");
      SeeedOled.setTextXY(1, 0);
      SeeedOled.putString("                ");
      SeeedOled.setTextXY(2, 0);
      SeeedOled.putString("Apprentissage<<<");
      SeeedOled.setTextXY(3, 0);
      SeeedOled.putString("                ");
      SeeedOled.setTextXY(4, 0);
      SeeedOled.putString("Libre           ");
      SeeedOled.setTextXY(5, 0);
      SeeedOled.putString("                ");
      mode = 1;
    }
    if (digitalRead(selecteur_mode) == 0) {
      SeeedOled.setTextXY(0, 0);
      SeeedOled.putString("   CHOIX MODE   ");
      SeeedOled.setTextXY(1, 0);
      SeeedOled.putString("                ");
      SeeedOled.setTextXY(2, 0);
      SeeedOled.putString("Apprentissage   ");
      SeeedOled.setTextXY(3, 0);
      SeeedOled.putString("                ");
      SeeedOled.setTextXY(4, 0);
      SeeedOled.putString("Libre        <<<");
      SeeedOled.setTextXY(5, 0);
      SeeedOled.putString("                ");
      mode = 0;
    }
  }
  if (mode == 1) {
    delay(200);
    Mode_Apprentissage();
  }
  if (mode == 0) {
    Mode_Libre();
  }
}


/////////////////////////////////////////*-------------------------------------MODE LIBRE--------------------------------------*/////////////////////////////////////////////////

void Mode_Libre(){                                          //MODE LIBRE : LE JOUEUR SELECTIONNE SON SON ET JOUE LIBREMENT
  float angle;
  angle = Angle(pot_sons_chansons);             //On tourne le potentiometre pour changer son son
  int u = true;
  while (u = true){
    u = true;
    if (angle <= 150) {
      SeeedOled.setTextXY(0, 0);
      SeeedOled.putString("   CHOIX  SON   ");
      SeeedOled.setTextXY(1, 0);
      SeeedOled.putString("                ");
      SeeedOled.setTextXY(2, 0);
      SeeedOled.putString("Piano        <<<");
      SeeedOled.setTextXY(3, 0);
      SeeedOled.putString("                ");
      SeeedOled.setTextXY(4, 0);
      SeeedOled.putString("Batterie        ");
      SeeedOled.setTextXY(5, 0);
      SeeedOled.putString("                ");
      while (angle <= 150){
        angle = Angle(pot_sons_chansons);
        Son = 1;
        clavier(Son);           ///!\ : On n'a pas besoin de valider
      }
    }
    if (angle > 150) {
      SeeedOled.setTextXY(0, 0);
      SeeedOled.putString("   CHOIX  SON   ");
      SeeedOled.setTextXY(1, 0);
      SeeedOled.putString("                ");
      SeeedOled.setTextXY(2, 0);
      SeeedOled.putString("Piano           ");
      SeeedOled.setTextXY(3, 0);
      SeeedOled.putString("                ");
      SeeedOled.setTextXY(4, 0);
      SeeedOled.putString("Batterie     <<<");
      SeeedOled.setTextXY(5, 0);
      SeeedOled.putString("                ");
      while (angle > 150){
        angle = Angle(pot_sons_chansons);
        Son = 2;
        clavier(Son);
      }
    }
  }
}

/////////////////////////////////////////*-----------------------------------------MODE APPRENTISSAGE----------------------------------*/////////////////////////////////////////////////

void Mode_Apprentissage(){
  float angle;
  while (Ok() != 1) {
    Ok();
    angle = Angle(pot_sons_chansons);
    if (angle >= 0 && angle < 150) {
      SeeedOled.setTextXY(0, 0);
      SeeedOled.putString("   CHOIX  SON   ");
      SeeedOled.setTextXY(1, 0);
      SeeedOled.putString("                ");
      SeeedOled.setTextXY(2, 0);
      SeeedOled.putString("Piano        <<<");
      SeeedOled.setTextXY(3, 0);
      SeeedOled.putString("                ");
      SeeedOled.setTextXY(4, 0);
      SeeedOled.putString("Batterie        ");
      SeeedOled.setTextXY(5, 0);
      SeeedOled.putString("                ");
      Son = 1;
    }
    Ok();
    if (angle >= 150 && angle <= 300) {
      SeeedOled.setTextXY(0, 0);
      SeeedOled.putString("   CHOIX  SON   ");
      SeeedOled.setTextXY(1, 0);
      SeeedOled.putString("                ");
      SeeedOled.setTextXY(2, 0);
      SeeedOled.putString("Piano           ");
      SeeedOled.setTextXY(3, 0);
      SeeedOled.putString("                ");
      SeeedOled.setTextXY(4, 0);
      SeeedOled.putString("Batterie     <<<");
      SeeedOled.setTextXY(5, 0);
      SeeedOled.putString("                ");
      Son = 2;
    }
  }
  delay(200);
  Ok();

  int c;
  while (Ok() != 1) {
    Ok();
    angle = Angle(pot_sons_chansons);
    if (angle >= 0 && angle < 100) {
      SeeedOled.setTextXY(0, 0);
      SeeedOled.putString(" CHOIX  CHANSON ");
      SeeedOled.setTextXY(1, 0);
      SeeedOled.putString("                ");
      SeeedOled.setTextXY(2, 0);
      SeeedOled.putString("Jai du bon ta<<<");
      SeeedOled.setTextXY(3, 0);
      SeeedOled.putString("                ");
      SeeedOled.setTextXY(4, 0);
      SeeedOled.putString("Everything Now  ");
      SeeedOled.setTextXY(5, 0);
      SeeedOled.putString("                ");
      SeeedOled.setTextXY(6, 0);
      SeeedOled.putString("Beat            ");
      SeeedOled.setTextXY(7, 0);
      SeeedOled.putString("                ");
      c = 1;
    }
    if (angle >= 100 && angle < 200) {
            SeeedOled.setTextXY(0, 0);
      SeeedOled.putString(" CHOIX  CHANSON ");
      SeeedOled.setTextXY(1, 0);
      SeeedOled.putString("                ");
      SeeedOled.setTextXY(2, 0);
      SeeedOled.putString("Jai du bon tabac");
      SeeedOled.setTextXY(3, 0);
      SeeedOled.putString("                ");
      SeeedOled.setTextXY(4, 0);
      SeeedOled.putString("Everything No<<<");
      SeeedOled.setTextXY(5, 0);
      SeeedOled.putString("                ");
      SeeedOled.setTextXY(6, 0);
      SeeedOled.putString("Beat            ");
      SeeedOled.setTextXY(7, 0);
      SeeedOled.putString("                ");
      c = 2;
    }
    if (angle >= 200 && angle <= 300) {
      SeeedOled.setTextXY(0, 0);
      SeeedOled.putString(" CHOIX  CHANSON ");
      SeeedOled.setTextXY(1, 0);
      SeeedOled.putString("                ");
      SeeedOled.setTextXY(2, 0);
      SeeedOled.putString("Jai du bon tabac");
      SeeedOled.setTextXY(3, 0);
      SeeedOled.putString("                ");
      SeeedOled.setTextXY(4, 0);
      SeeedOled.putString("Everything Now  ");
      SeeedOled.setTextXY(5, 0);
      SeeedOled.putString("                ");
      SeeedOled.setTextXY(6, 0);
      SeeedOled.putString("Beat         <<<");
      SeeedOled.setTextXY(7, 0);
      SeeedOled.putString("                ");
      c = 3;
    }
  }
  delay(200);
  if (c == 1) {
    chanson1(Son);
  }
  if (c == 2) {
    chanson2(Son);
  }
  if (c == 3) {
    chanson3(Son);
  }
}
