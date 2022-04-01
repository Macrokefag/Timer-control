// rely pin 51 49 47 45 43 41
// üzemmod pin 9
//tigger pin 10
// manual start pin 8
// encoder pin 7 6 5 az  5 pin a button
// forgo kapcsolo pin 52 50 48 46 44 42 
// menu pin elso34, also38, ball36, jobb40
// piros 53, hot 39 , citrom.S 37,  narancs.S 35, karosFEL 33, karosLE 31

// lcd.setCursor (0,2); oszlop 0-19 sor 0-3
//-----------------------------------------------------------------------------------
//Bill kombináciok
// narancs+citrom+piros=reset
// vágott kapcsolo a doboz jobbsarkán  üzemmod kapcsolo.. elfele timer lefele delay üzemmod
// piros többfunkcios nyomogombb = seve , load engedélyezés 
// LOAD= pirosnyomva+ narnancs lépked lefele .... pirosnyomva+citrom lépked  felfele memoria helyek között... ha a piros elenged akkor are you sure ... bármely kombb cancel a piros YES
// SAVE = karos nyomogombb kapcsolo (baloldat) fel irány +narancs ... memoria hely vátás a karossal... majd citrom ...ARE YOU SURE ... bármely cancel ..piros YES




#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);

#define ENCODER_DO_NOT_USE_INTERRUPTS
#include <Encoder.h>

//EEPROM cuccok
#include <EEPROM.h>
unsigned long address=0;



Encoder myEnc(7, 6);

int f,ff; //általános for

// delay segédvátozoi
unsigned int Tm1,Tm2,Tm3,Tm4,Tm5,Tm6;
unsigned int Tc1,Tc2,Tc3,Tc4,Tc5,Tc6;

unsigned long CH[7]; //time
unsigned long CHL[7]; // Long
unsigned long CHX[7]; // delay
unsigned long START[7],END[7];

unsigned long ti;
unsigned long maxi=0;//maximu time
unsigned long maxiS=0;

int varakozas=100; // dlay idö a ha button nyomás;
byte MODE=1;
byte k=0; // kapcsolo ha változás történt
byte FORGO=1; // a forgo kapcsolo pozicio
byte x=3; //x culsor pozicio
byte y=2; // culsor y pozicio 
byte db=6; //ennyi db relé
byte mentve=0;

int pin[]={0,51,49,47,45,43,41}; //pin mapp
int culsor[]={0,7,12,13,14,19}; // culsor pozicio  map


int d=2000; // delay segled ennyit várakozij 2 akcio között

int segedM, segedT, segedSZ,segedE,segedTE; // segédek a print hez a szám felbontásához számjegyekre


//unsigned long hatra;
unsigned long h=4294967295;
unsigned long hh=1;

int rotX, rotXX ; //rotary segéd válozo a változás figyeléshez
unsigned long szorzo[6]={0,1000000,100000,10000,1000,100}; //a rotari segéd változoi

unsigned long MemD=0;
unsigned long MemT=0;
unsigned long OldMemD=0;
unsigned long OldMemT=0;
int ment=0;

//---------------------------------------------START SETUP---------------------
void(* resetFunc) (void) = 0; //declare reset function @ address 0
//--------------------------------------------------------------------------------------


void setup() {
//Serial.begin(9600);

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//eeprom nullázás csak 1x kell!!
//for (int f = 0 ; f < EEPROM.length() ; f++) {EEPROM.write(f, 0);} //eeprom nullázás csak 1x kell!!

//eeprom elsö feltöltés csak 1x kell...

  
 // rely pin 51 49 47 45 43 41 Nullára huzbe a relé
 pinMode(51, OUTPUT); digitalWrite(51, 1);
 pinMode(49, OUTPUT); digitalWrite(49, 1);
 pinMode(47, OUTPUT); digitalWrite(47, 1);
 pinMode(45, OUTPUT); digitalWrite(45, 1);
 pinMode(43, OUTPUT); digitalWrite(43, 1);
 pinMode(41, OUTPUT); digitalWrite(41, 1);
 digitalWrite(41, 0);delay(200);digitalWrite(41, 1);// relé teszt
 

 // üzemmod pin 9
pinMode(9, INPUT_PULLUP);

//tigger pin 10
pinMode(10, INPUT_PULLUP);

// manual start pin 8
pinMode(8, INPUT_PULLUP);

// encoder pin 7 6 5 az  5 pin a button CSAK AZ 5 KELL FELHUZNI A TÖBBIT A LIBRY CSINÁLJA
pinMode(5, INPUT_PULLUP);

// forgo kapcsolo pin 52 50 48 46 44 42 
pinMode(52, INPUT_PULLUP);
pinMode(50, INPUT_PULLUP);
pinMode(48, INPUT_PULLUP);
pinMode(46, INPUT_PULLUP);
pinMode(44, INPUT_PULLUP);
pinMode(42, INPUT_PULLUP);

// menu pin elso 32 also 38 ball 34 jobb 38 
pinMode(36, INPUT_PULLUP);
pinMode(38, INPUT_PULLUP);
pinMode(34, INPUT_PULLUP);
pinMode(40, INPUT_PULLUP);

// piros53, hot 39 , citrom.s 37, narancs.s 35, karosfel 33, karosle 31
pinMode(53, INPUT_PULLUP);
pinMode(39, INPUT_PULLUP);
pinMode(37, INPUT_PULLUP);
pinMode(35, INPUT_PULLUP);
pinMode(33, INPUT_PULLUP);
pinMode(31, INPUT_PULLUP); 

  lcd.init();
  lcd.setBacklight(1);

  //...........................................................................................................
lcd.clear();
lcd.print("    Mano Timer");

lcd.setCursor (3,2);lcd.print("2022.03.31."); //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX  VERZIO XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

delay(500);
//..........................................................................................................

lcd.clear();
for(f=1;f<30;f++){
  //lcd.setCursor (0,0);
  switch (random(4)){
  case 0:lcd.print("0");break;
  case 1:lcd.print("1");break;
  case 2:lcd.print(" ");break;
  case 3:lcd.print("  ");break;
  }
  lcd.scrollDisplayRight();
  delay(10);}

lcd.clear();
lcd.print("     Macrokefag");
lcd.setCursor (0,2);lcd.print("Hardware & Software");

lcd.setCursor (0,3);lcd.print("       v0.92");//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

delay(1500);
 lcd.clear();
PRINT();

CH[1]=1000000; CHL[1]=100000;
CH[2]=2000000; CHL[2]=100000;
CH[3]=3000000; CHL[3]=100000;
CH[4]=4000000; CHL[4]=100000;
CH[5]=5000000; CHL[5]=100000;
CH[6]=6000000; CHL[6]=100000;

CHX[1]=0;
CHX[2]=10000;
CHX[3]=10000;
CHX[4]=10000;
CHX[5]=10000;
CHX[6]=10000;

maxi=0;
ti=micros();

eepromOLVAS();
PRINT();
}
//----------------------------END SETUP----------------------------------- 





//---------------------------------------START LOOP---------------------------------------
void loop() {

if(digitalRead(53)==0 && digitalRead(37)==0 && digitalRead(35)==0) {resetFunc();} //reset
if(digitalRead(35)==0 && digitalRead(33)==0 ){save();} //mentés
READ();
rotary();
vanHatra();
FelLe();
Hot();



 //Serial.println(digitalRead(8));

}
//---------------------------------------END LOOP----------------------------------------



void PRINT(){
lcd.clear();
  
 // 0. sor
 lcd.setCursor (0,0);lcd.print("Mem:");
 if(MODE==0){lcd.print(MemT);if(mentve==0){lcd.print("s");} else {lcd.print("*");}
 } else {y=2;lcd.print(MemD);
 if(mentve==0){lcd.print("s");} else {lcd.print("*");}
 lcd.setCursor (0,3);lcd.print("        ");}
 lcd.setCursor (7,0);lcd.print("CH:");lcd.print(FORGO);
 lcd.setCursor (15,0);lcd.print("L");
 
if(MODE==0){ //Time mode
//1. sor
 //lcd.setCursor (0,1);lcd.print("       m  t  s  e  x");

 //2. sor 
 
lcd.setCursor (0,2);lcd.print("        ");
if(MODE==0){lcd.setCursor (0,2);lcd.print("Time:");} else {lcd.setCursor (0,2);lcd.print("Delay:");}   

// sec
segedM=int(CH[FORGO]/1000000);
if(segedM<=9){lcd.setCursor (7,2);lcd.print(segedM);} else {lcd.setCursor (6,2);lcd.print(segedM);}

//tized
segedT=int((CH[FORGO]-(segedM*1000000))/100000);
lcd.setCursor (12,2);lcd.print(segedT);

//szátad
segedSZ=int(((CH[FORGO]-(segedM*1000000))-(segedT*100000))/10000);
lcd.setCursor (13,2);lcd.print(segedSZ);

//ezred
segedE=(((CH[FORGO]-(segedM*1000000))-(segedT*100000))-(segedSZ*10000));
segedE=segedE/1000;
lcd.setCursor (14,2);lcd.print(segedE);

//tizezred
segedTE=(((CH[FORGO]-(segedM*1000000))-(segedT*100000))-(segedSZ*10000))-(segedE*1000);
segedTE=segedTE/100;
lcd.setCursor (19,2);lcd.print(segedTE);


//3. sor ha van long
if(MODE==0){
  lcd.setCursor (0,3);lcd.print("        ");
  lcd.setCursor (0,3);lcd.print("Long:");
  
// sec
segedM=int(CHL[FORGO]/1000000);
if(segedM<=9){lcd.setCursor (7,3);lcd.print(segedM);} else {lcd.setCursor (6,3);lcd.print(segedM);}

//tized
segedT=int((CHL[FORGO]-(segedM*1000000))/100000);
lcd.setCursor (12,3);lcd.print(segedT);

//szátad
segedSZ=int(((CHL[FORGO]-(segedM*1000000))-(segedT*100000))/10000);
lcd.setCursor (13,3);lcd.print(segedSZ);

//ezred
segedE=(((CHL[FORGO]-(segedM*1000000))-(segedT*100000))-(segedSZ*10000));
segedE=segedE/1000;
lcd.setCursor (14,3);lcd.print(segedE);

//tizezred
segedTE=(((CHL[FORGO]-(segedM*1000000))-(segedT*100000))-(segedSZ*10000))-(segedE*1000);
segedTE=segedTE/100;
lcd.setCursor (19,3);lcd.print(segedTE);
} 
 
 else
  {lcd.setCursor (0,3);lcd.print("                    ");}
}


// Delay MODE !!!!!!!!!! ----------------------------------------------
 //2. sor 
 
if(MODE==1){lcd.setCursor (0,2);lcd.print("        ");
if(MODE==0){lcd.setCursor (0,2);lcd.print("Time:");} else {lcd.setCursor (0,2);lcd.print("Delay:");}   

// sec
segedM=int(CHX[FORGO]/1000000);
if(segedM<=9){lcd.setCursor (7,2);lcd.print(segedM);} else {lcd.setCursor (6,2);lcd.print(segedM);}

//tized
segedT=int((CHX[FORGO]-(segedM*1000000))/100000);
lcd.setCursor (12,2);lcd.print(segedT);

//szátad
segedSZ=int(((CHX[FORGO]-(segedM*1000000))-(segedT*100000))/10000);
lcd.setCursor (13,2);lcd.print(segedSZ);

//ezred
segedE=(((CHX[FORGO]-(segedM*1000000))-(segedT*100000))-(segedSZ*10000));
segedE=segedE/1000;
lcd.setCursor (14,2);lcd.print(segedE);

//tizezred
segedTE=(((CHX[FORGO]-(segedM*1000000))-(segedT*100000))-(segedSZ*10000))-(segedE*1000);
segedTE=segedTE/100;
lcd.setCursor (19,2);lcd.print(segedTE);
}




  lcd.setCursor (culsor[x],y);lcd.blink();// itt állitodik a culsor

  
  }

//-------------------------------------------------------------------------------------------------

void READ(){

//mode
if(digitalRead(9) != MODE){MODE=digitalRead(9);PRINT();}

//forgo kapcsolo
if(digitalRead(52)==0 &&  FORGO != 6){FORGO=6;PRINT();}
if(digitalRead(50)==0 &&  FORGO != 5){FORGO=5;PRINT();}
if(digitalRead(48)==0 &&  FORGO != 4){FORGO=4;PRINT();}
if(digitalRead(46)==0 &&  FORGO != 3){FORGO=3;PRINT();}
if(digitalRead(44)==0 &&  FORGO != 2){FORGO=2;PRINT();}
if(digitalRead(42)==0 &&  FORGO != 1){FORGO=1;PRINT();}

//colsor x
//balra
// menu pin elso34, also38, ball36, jobb40
if(digitalRead(36)==0){
delay(varakozas);
if(digitalRead(36)==0){
  x--;
  if (x==0){x=1;}
  lcd.setCursor (culsor[x],y);lcd.blink();
  }
  }

//jobbra
if(digitalRead(40)==0){
  delay(varakozas);
  if(digitalRead(40)==0){
  x++;
  if (x==6){x=5;}
  lcd.setCursor (culsor[x],y);lcd.blink();
  }}

  //colsor y
//fel
// menu pin elso34, also38, ball36, jobb40
if(digitalRead(33)==0){
delay(varakozas);
if(digitalRead(33)==0){
  y--;
  if (y==1){y=2;}
  if(MODE==1){y=2;} // ha nem kell a 3.sor akkor nem megy le a kulzor
  lcd.setCursor (culsor[x],y);lcd.blink();
  }}

//le
if(digitalRead(31)==0){
  delay(varakozas);
  if(digitalRead(31)==0){
  y++;
  if (y==4){y=3;}
  if(MODE==1){y=2;} // ha nem kell a 3.sor akkor nem megy le a kulzor
  lcd.setCursor (culsor[x],y);lcd.blink();
  }}
//akcio manuális 
//if(digitalRead(8)==0){akcio();}

// üzemmod pin 9
//tigger pin 10
// manual start pin 8
// encoder pin 7 6 5 az  5 pin a button
// forgo kapcsolo pin 52 50 48 46 44 42 


//memoria vátás
OldMemD=MemD;
OldMemT=MemT;
  
while (digitalRead(53)==0){
//lefele irányba  MODE=0
if(digitalRead(53)==0 && digitalRead(35)==0 && MODE==0){
  MemT--; if(MemT>400000000){MemT=0;} // ha MemT -1 akkor MemT=0
  PRINT();
  delay(varakozas);k=1;}

  //lefele irányba  MODE=1
if(digitalRead(53)==0 && digitalRead(35)==0 && MODE==1){
  MemD--; if(MemD>400000000){MemD=0;} // ha MemD -1 akkor MemT=0
  PRINT();
  delay(varakozas);k=1;}

  //fel irányba  MODE=0
if(digitalRead(53)==0 && digitalRead(37)==0 && MODE==0){
  MemT++; if(MemT>=10){MemT=9;} // max 9
  PRINT();
  delay(varakozas);k=1;}

    //fel irányba  MODE=1
if(digitalRead(53)==0 && digitalRead(37)==0 && MODE==1){
  MemD++; if(MemD>=10){MemD=9;} // max 9
  PRINT();
  delay(varakozas);k=1;}
}

//--------------------------------------------------- 

if(k==1){
lcd.setCursor (0,1);lcd.print("                   ");
lcd.setCursor (0,1);lcd.print("Are You Sure? LOAD:");
if(MODE==1){lcd.print(MemD);lcd.setCursor (19,1);}
if(MODE==0){lcd.print(MemT);lcd.setCursor (19,1);}
delay(1500);

int kkk=0;
//cancel vagy load ....piros a load
 while (kkk==0){
    if(digitalRead(37)==0){kkk=1;}
    if(digitalRead(35)==0){kkk=1;}
    if(digitalRead(34)==0){kkk=1;}
    if(digitalRead(38)==0){kkk=1;}
    if(digitalRead(36)==0){kkk=1;}
    if(digitalRead(40)==0){kkk=1;}
    if(digitalRead(53)==0){kkk=2;} // ha pros vagyis k=2 akkor save külömben cancel
  }
if(kkk==2){
if(MODE==1){lcd.setCursor (0,1);lcd.print("  Memory ");lcd.print(MemD);lcd.print(" Loaded!");kkk=0;mentve=0;}
if(MODE==0){lcd.setCursor (0,1);lcd.print("  Memory ");lcd.print(MemT);lcd.print(" Loaded!");kkk=0;mentve=0;}
delay(1500);
eepromOLVAS();//majd ide kell a memoria olvaso void
PRINT();k=0;kkk=0;}
if(kkk==1){
MemD=OldMemD;
MemT=OldMemT;
lcd.setCursor (0,1);lcd.print("                   ");
lcd.setCursor (0,1);lcd.print("** Cancel LOAD **");
k=0;kkk=0;
delay(1500);
PRINT();
}
}
}


//-------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------

  
void rotary(){

myEnc.write(0);

if(MODE==0){
//time állitás
if(y==2){
  while (digitalRead(5)==0){
    lcd.noBlink();
    lcd.cursor();
    
    rotX=myEnc.read(); //rotari olvasás

    //növelés
    if(rotX > 2){
    CH[FORGO]=CH[FORGO]+szorzo[x];
    if(CH[FORGO]>65000000){CH[FORGO]=65000000;} //nem engedi 65sec fölé!
    myEnc.write(0);mentve=1;
    delay(100);
    
    PRINT();
    }  

  //csökkentés
    if(rotX < -2){
    CH[FORGO]=CH[FORGO]-szorzo[x];
    if(CH[FORGO]>4000000000){CH[FORGO]=0;} //nem engedi nulla alá!
    myEnc.write(0);mentve=1;;
    delay(100);
    PRINT();
    }     
}    // while vége
}    // if vége ,ha y==2


//long állitás
if(y==3){
  while (digitalRead(5)==0){
    lcd.noBlink();
    lcd.cursor();
    
    rotX=myEnc.read(); //rotari olvasás

    //növelés
    if(rotX > 2){
    CHL[FORGO]=CHL[FORGO]+szorzo[x];
    if(CHL[FORGO]>65000000){CHL[FORGO]=65000000;} //nem engedi 65sec fölé!
    myEnc.write(0);mentve=1;
    delay(100);
    PRINT();
    }  

  //csökkentés
    if(rotX < -2){
    CHL[FORGO]=CHL[FORGO]-szorzo[x];
    if(CHL[FORGO]>4000000000){CHL[FORGO]=0;}  //nem engedi nulla alá!
    myEnc.write(0);mentve=1;
    delay(100);
    PRINT();
    }     
}    // while vége
}    // if vége ,ha y==3
} //ha MODE==0. *vége*

if(MODE==1){
//time állitás
if(y==2){
  while (digitalRead(5)==0){
    lcd.noBlink();
    lcd.cursor();
    
    rotX=myEnc.read(); //rotari olvasás

    //növelés
    if(rotX > 2){
    CHX[FORGO]=CHX[FORGO]+szorzo[x];
    if(CHX[FORGO]>65000000){CHX[FORGO]=65000000;} //nem engedi 65sec fölé!
    myEnc.write(0);mentve=1;
    delay(100);
    PRINT();
    }  

  //csökkentés
    if(rotX < -2){
    CHX[FORGO]=CHX[FORGO]-szorzo[x];
    if(CHX[FORGO]>4000000000){CHX[FORGO]=0;} //nem engedi nulla alá!
    myEnc.write(0);mentve=1;
    delay(100);
    PRINT();
    }     
}    // while vége
}    // if vége ,ha y==2 és MODE==1
}
lcd.noCursor();
lcd.blink();
}    //rotari end

//---------------------------------------------------------------------------------------------------


void vanHatra(){

h=(4294967295-micros())/1000000;
if(h != hh){
  hh=h;
  //Serial.println(h);Serial.println(hh);
  lcd.setCursor (16,0);lcd.print("    ");
  lcd.setCursor (16,0);lcd.print(hh);
  lcd.setCursor (culsor[x],y);}
}

//------------------------------------------------------------------------------------------------------

void FelLe(){
 if(MODE==0){
 if(y==2){  //time állitás
  //növelés
if(digitalRead(34)==0){
delay(varakozas);
if(digitalRead(34)==0){CH[FORGO]=CH[FORGO]+szorzo[x];
if(CH[FORGO]>65000000){CH[FORGO]=65000000;} //nem engedi 65sec fölé!
delay(varakozas);mentve=1;
PRINT();}}

//csökkentés
if(digitalRead(38)==0){
delay(varakozas);
if(digitalRead(38)==0){CH[FORGO]=CH[FORGO]-szorzo[x];
if(CH[FORGO]>4000000000){CH[FORGO]=0;} //nem engedi nulla alá!
delay(varakozas);mentve=1;
PRINT();}}}

 if(y==3){  //long állitás
  //növelés
if(digitalRead(34)==0){
delay(varakozas);
if(digitalRead(34)==0){CHL[FORGO]=CHL[FORGO]+szorzo[x];
if(CHL[FORGO]>65000000){CHL[FORGO]=65000000;} //nem engedi 65sec fölé!
delay(varakozas);mentve=1;
PRINT();}}

//csökkentés
if(digitalRead(38)==0){
delay(varakozas);
if(digitalRead(38)==0){CHL[FORGO]=CHL[FORGO]-szorzo[x];
if(CHL[FORGO]>4000000000){CHL[FORGO]=0;} //nem engedi nulla alá!
delay(varakozas);mentve=1;
PRINT();}}}}

 if(MODE==1){ //dealy állitás
 if(y==2){  
  //növelés
if(digitalRead(34)==0){
delay(varakozas);
if(digitalRead(34)==0){CHX[FORGO]=CHX[FORGO]+szorzo[x];
if(CHX[FORGO]>65000000){CHX[FORGO]=65000000;} //nem engedi 65sec fölé!
delay(varakozas);mentve=1;
PRINT();}}

//csökkentés
if(digitalRead(38)==0){
delay(varakozas);
if(digitalRead(38)==0){CHX[FORGO]=CHX[FORGO]-szorzo[x];
if(CHX[FORGO]>4000000000){CHX[FORGO]=0;} //nem engedi nulla alá!
delay(varakozas);mentve=1;
PRINT();}}}}
} 

void Hot(){
if(digitalRead(39)==0){
    lcd.noCursor();
    lcd.noBlink();
    lcd.clear();
    lcd.setCursor (0,1);lcd.print("**Device is Hot!!!**");

if(MODE==1){   //Serial.println(MODE); 
Tm1=CHX[1]/1000; Tc1=CHX[1]-(Tm1*1000);
Tm2=CHX[2]/1000; Tc2=CHX[2]-(Tm2*1000);
Tm3=CHX[3]/1000; Tc3=CHX[3]-(Tm3*1000);
Tm4=CHX[4]/1000; Tc4=CHX[4]-(Tm4*1000);
Tm5=CHX[5]/1000; Tc5=CHX[5]-(Tm5*1000);
Tm6=CHX[6]/1000; Tc6=CHX[6]-(Tm6*1000);}

if(MODE==0){  //Serial.println(MODE); 
  for(f=1;f<=6;f++){
    maxiS=CH[f]+CHL[f];
    if(maxiS>maxi){maxi=maxiS;}}}
 
    while (digitalRead(39)==0){ 
      if(digitalRead(8)==0 || digitalRead(10)==0){ //itt fire pinrket olvassa
      akcio();
      lcd.print("***Switch Off Hot***");
    }
    }
    
    PRINT();
    digitalWrite(51,1);
    digitalWrite(49,1);
    digitalWrite(47,1);
    digitalWrite(45,1);
    digitalWrite(43,1);
    digitalWrite(41,1);
    }
    }

//----------------------------------------------------------------------------------------------------



void akcio(){   //Serial.println("MODE: ");Serial.println(MODE); delay(2000);

  if(MODE==1){   //Serial.println("Delay Mode");delay(2000);
    delay(Tm1);delay(Tc1);digitalWrite(51,0);
    delay(Tm2);delay(Tc2);digitalWrite(49,0);
    delay(Tm3);delay(Tc3);digitalWrite(47,0);
    delay(Tm4);delay(Tc4);digitalWrite(45,0);
    delay(Tm5);delay(Tc5);digitalWrite(43,0);
    delay(Tm6);delay(Tc6);digitalWrite(41,0);}

if(MODE==0){//Serial.println("Time Mode");//delay(2000);
  ti=micros();
  maxi=maxi+1000000+ti; // 1sec pluszba
  
  for(f=1;f<=db;f++){ END[f]=CHL[f]+ti+CH[f];START[f]=CH[f]+ti;} 
  
  while(micros() < maxi){
  for(f=1;f<=db;f++){
  ti=micros();
  /*Serial.print("f ");Serial.println(f);
  Serial.print("START ");Serial.println(START[f]);
  Serial.print("END ");Serial.println(END[f]);
  Serial.print("ti ");Serial.println(ti);
  Serial.print("maxi ");Serial.println(maxi);
  Serial.println("................");
  */
  if((ti > START[f]) && (ti<END[f]))
  {digitalWrite(pin[f],0); }
  else
  { digitalWrite(pin[f],1); }}}}
    
lcd.clear();
lcd.print("-----AKCIO END------");
delay(2000);
maxi=0;
}

//............................................................................................................

//This function will write a 4 byte (32bit) long to the eeprom at
//the specified address to adress + 3.
void EEPROMWritelong(int address, unsigned long value)
      {
      //Decomposition from a long to 4 bytes by using bitshift.
      //One = Most significant -> Four = Least significant byte
      byte four = (value & 0xFF);
      byte three = ((value >> 8) & 0xFF);
      byte two = ((value >> 16) & 0xFF);
      byte one = ((value >> 24) & 0xFF);

      //Write the 4 bytes into the eeprom memory.
      EEPROM.update(address, four);
      EEPROM.update(address + 1, three);
      EEPROM.update(address + 2, two);
      EEPROM.update(address + 3, one);
      }

//........................................................................................

//This function will return a 4 byte (32bit) long from the eeprom
//at the specified address to adress + 3.
unsigned long EEPROMReadlong(unsigned long address2)
      {
      //Read the 4 bytes from the eeprom memory.
      unsigned long four = EEPROM.read(address);
      unsigned long three = EEPROM.read(address + 1);
      unsigned long two = EEPROM.read(address + 2);
      unsigned long one = EEPROM.read(address + 3);

      //Return the recomposed long by using bitshift.
      return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
      }

//...................................................................................

void eepromIRAS(){
if(MODE==1){
  address=1000+(MemD*100);
  
  for(f=1;f<=6;f++){
    EEPROMWritelong(address, CHX[f]);
  //Serial.print("address "); Serial.println(address); Serial.println(CHX[f]);
    address+=4;
    }}

 if(MODE==0){
    address=2000+(MemT*100);
    //Serial.println(address);
    for(f=1;f<=6;f++){
    EEPROMWritelong(address, CH[f]);
    address+=4;
    EEPROMWritelong(address, CHL[f]);
    address+=4;
 }}}
     
    

  //.....................................................................................................

  void eepromOLVAS(){

    if(MODE==1){
  address=1000+(MemD*100);
  for(f=1;f<=6;f++){
    CHX[f]=EEPROMReadlong(address);
    address+=4;
    }}

    if(MODE==0){
    address=2000+(MemT*100);
    for(f=1;f<=6;f++){
    CH[f]=EEPROMReadlong(address);
    address+=4;
    CHL[f]=EEPROMReadlong(address);
    address+=4; 
    }}
 }

 //.............................................................................................


 void save(){
 lcd.setCursor (0,1);lcd.print("Save to Memory: ");lcd.print(ment);lcd.setCursor (16,1);
 delay(1000);


 while (digitalRead(37)==1){

  //ment++
  if(digitalRead(33)==0)
  {delay(varakozas);
  if(digitalRead(33)==0)
  {ment++;}
  if(ment>=10){ment=9;}
  lcd.setCursor (16,1);lcd.print(ment);lcd.setCursor (16,1);delay(varakozas);}

    //ment--
  if(digitalRead(31)==0)
  {delay(varakozas);
  if(digitalRead(31)==0)
  {ment--;}
  if(ment <= -1){ment=0;}
  lcd.setCursor (16,1);lcd.print(ment);lcd.setCursor (16,1);delay(varakozas);}
  }
  lcd.setCursor (0,1);lcd.print("                   ");lcd.setCursor (0,1);lcd.print("Are You Sure? Save:");lcd.print(ment);lcd.setCursor (19,1);delay(500);
k=0;

//cancel vagy save ....piros a save
 while (k==0){
    if(digitalRead(37)==0){k=1;}
    if(digitalRead(35)==0){k=1;}
    if(digitalRead(34)==0){k=1;}
    if(digitalRead(38)==0){k=1;}
    if(digitalRead(36)==0){k=1;}
    if(digitalRead(40)==0){k=1;}
    if(digitalRead(53)==0){k=2;} // ha pros vagyis k=2 akkor save külömben cancel
  }
  if(k==1){
  lcd.setCursor (0,1);
  lcd.print("                   ");
  lcd.setCursor (0,1);
  lcd.print("***Save Cancel!***");
  lcd.setCursor (19,1);
  delay(1500);
  lcd.setCursor (0,1);
  lcd.print("                    ");
  k=0;
} 

//Serial.println(k);

if(k==2){
  MemD=ment;
  MemT=ment;
  eepromIRAS();
  lcd.setCursor (0,1);
  lcd.print("                   ");
  lcd.setCursor (0,1);
  lcd.print("****** Saved ******");mentve=0;PRINT();
  lcd.setCursor (19,1);
  delay(1500);
  lcd.setCursor (0,1);
  lcd.print("                    ");
  k=0;
  
  }
  }
