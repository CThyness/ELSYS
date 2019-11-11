#include <CapacitiveSensor.h>
CapacitiveSensor cap = CapacitiveSensor(4,2); //kapasative sensoren er koblet til både 2 og 4
//husk at Led-porten på fargesensoren skal kobles til 3.3V
const int s0 = 5; //S0 og S1 forteller frekvens vi skal få info i og kommer fra port 5 og 6
const int s1 = 6;
const int s2 = 9; //S2 og s3 brukes for å fortelle hvilken farge som skal måles og signalet kommer fra port 9 og 10
const int s3 = 10;
const int sens_ut = 11; //Signalet fra sensoren til arduinoen sendes til port 11
const int LedR = 12; //Signalet til rød LED kommer fra port 12
const int LedG = 13; //Signalet til grønn LED kommer fra port 13
const int grense_kapasativ = 100; //Setter grensen for vår kapasative sensor, kan trenge kalibrering!
int maalinger_gront;
int tid;
int dtid;

int sens_verdi = 0;
int sens_verdi_rodt = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);
  pinMode(sens_ut, INPUT);

  pinMode(LedR, OUTPUT);
  pinMode(LedG, OUTPUT);

  
  
  //Setter sensoren til å sende signalet tilbake i 1/5 av den reele verdien
  digitalWrite(s0, HIGH);
  digitalWrite(s1, LOW);

  digitalWrite(s2, LOW); //Setter den til å måle blå
  digitalWrite(s3, HIGH);
  
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  sens_verdi = pulseIn(sens_ut, LOW);  //Sjekker signalet fra sensoren
  Serial.println(sens_verdi);

  long kapasativ_verdi = cap.capacitiveSensor(30); //Finner verdien til vår kapasative sensor
  //Serial.println(kapasativ_verdi);
  
  if (sens_verdi < 9) { //Denne verdien kommer til å trenge kalibrering!!! (Lavere verdi for at den grønne streken skal ha ABSORBERT mer lys)
    delay(2); //Hvis bakken ikke er blå måles det på nytt om 10 millisekunder
  }
  else if (maalinger_gront == 0){  //Første gang det måles grønt så noteres det ned og så tar arduinoen en pause i 2 sek
    Serial.println("merkelig");
    maalinger_gront = 1;
    delay(2000);
  }
  else if (kapasativ_verdi > grense_kapasativ){  //Hvis du holder for lenge på håndtaket
    Serial.println("fuck");
    digitalWrite(LedR, HIGH); //Setter på vår røde LED for 1min
    delay(10000);
    digitalWrite(LedR, LOW);
    maalinger_gront = 0;
  }
  else {  //Hvis du slipper kulen i tide
    maalinger_gront = 0;
    dtid = 0;
    Serial.println("Ja, innteresant");
    tid = millis();
    digitalWrite(LedG, HIGH);
      while (dtid < 10000){
        dtid = millis() - tid;
        long kapasativ_verdi = cap.capacitiveSensor(30); //Finner verdien til vår kapasative sensor
        if (kapasativ_verdi > grense_kapasativ){
          Serial.println("fuck2");
          digitalWrite(LedG, LOW);
          digitalWrite(LedR, HIGH); //Setter på vår røde LED for det som er igjen av 1min
          delay(10000 - dtid);
          digitalWrite(LedR, LOW);
          break;
        }
        delay(10); //tar pause i 10ms
      }
    digitalWrite(LedG, LOW);
    
  }
}
