#include <EEPROM.h>
#include <Wire.h>
#include <DHT.h>
#include <LiquidCrystal.h>
#define DHTTYPE DHT22
#define DHTPIN 9
DHT dht(DHTPIN, DHTTYPE);
const int buttonPin = 31;
int contrast = 100;
const int pinpompa = 29;
LiquidCrystal lcd(12,11,5,4,3,2);
const int DS1307 = 0x68; // Address of DS1307 see data sheets
const char* namahari[] =
{"Minggu", "Senin", "Selasa", "Rabu", "Kamis", "Jum'at", "Sabtu"};
const char* namabulan[] =
{"Januari", "Februari", "Maret", "April", "Mei", "Juni", "Juli", "Augustus","September", "Oktober", "November", "Desember"};

bool man= false;
bool vled;
byte detik = 0;
byte menit = 0;
byte jam = 0;
byte hari = 0;
byte tanggal = 0;
byte bulan = 0;
byte tahun = 0;
byte input;

int addressjamawaleeprom = 0;
int addressmenitawaleeprom = 1;
int addressdetikawaleeprom = 2;
int addressjamakhireeprom = 3;
int addressmenitakhireeprom = 4;
int addressdetikakhireeprom = 5;

byte jamawal;
byte menitawal;
byte detikawal;
byte jamakhir;
byte menitakhir;
byte detikakhir;

///// stopwatch
///
unsigned long dtk;
unsigned long mnt;
unsigned long jm;
unsigned long sekarang;
unsigned long previousMillis=0;
byte hstart;
byte hfinish;
byte mstart;
byte mfinish;
byte sstart;
byte sfinish;

  unsigned long totalmenit;
  unsigned long totaldetik;
  unsigned long convertpembandingmenitnyala;
  unsigned long convertpembandingdetiknyala;
  unsigned long convertpembandingmenitmati;
  unsigned long convertpembandingdetikmati;
int buttonstate=0;
unsigned long start;

 
void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  dht.begin();
  lcd.begin(16,2);
  analogWrite(6, contrast);
  pinMode(buttonPin, INPUT);
  pinMode(pinpompa, OUTPUT);
  pinMode(13,OUTPUT);
  Serial.begin(9600);
  
  

  Serial.println(man);
  Serial.print("EEPROM Length: ");
  Serial.print(EEPROM.length());
  Serial.println("");
  hstart = EEPROM.read(addressjamawaleeprom);
  mstart = EEPROM.read(addressmenitawaleeprom);
  sstart = EEPROM.read(addressdetikawaleeprom);
  hfinish = EEPROM.read(addressjamakhireeprom);
  mfinish = EEPROM.read(addressmenitakhireeprom);
  sfinish = EEPROM.read(addressdetikakhireeprom);
  Serial.println("waktu nyala dari jam : ");
  Serial.print(hstart);
  Serial.print(":");
  Serial.print(mstart);
  Serial.print(":");
  Serial.print(sstart);
  Serial.print(" sampai ");
  Serial.print(hfinish);
  Serial.print(":");
  Serial.print(mfinish);
  Serial.print(":");
  Serial.print(sfinish);
  Serial.println("");

  Serial.println("");
  Serial.println("1 = otomatis");
  Serial.println("2 = manual");
}

void loop() {
  // put your main code here, to run repeatedly:
  dhtlcd();
  bacartc();
    jamlcd();
  tombol();
  cekmode();
  waterpump();
}

void dhtlcd(){
unsigned long currentMillis;
float h = dht.readHumidity();
float t = dht.readTemperature();
currentMillis = millis();
  if (currentMillis-previousMillis >= 1000){
  previousMillis = currentMillis;
  lcd.setCursor(0,0);
  lcd.print(t);
  start = start+1;
}
}

byte deckebcd(byte val) {
  return ((val/10*16) + (val%10));
}
byte bcdkedec(byte val) {
  return ((val/16*10) + (val%16));
}

void digit(int angka){
  if (angka>=0 && angka<10){
    lcd.print('0');
  } 
  lcd.print(angka);
}

void bacartc(){
  Wire.beginTransmission(DS1307);
  Wire.write(byte(0));
  Wire.endTransmission();
  Wire.requestFrom(DS1307, 7);
  detik = bcdkedec(Wire.read());
  menit = bcdkedec(Wire.read());
  jam = bcdkedec(Wire.read());
  hari = bcdkedec(Wire.read());
  tanggal = bcdkedec(Wire.read());
  bulan = bcdkedec(Wire.read());
  tahun = bcdkedec(Wire.read());
}

void jamlcd(){
  bacartc();
  lcd.setCursor(13,0);
  lcd.print("jam");
  lcd.setCursor(11,1);
  digit(jam);
  lcd.setCursor(13,1);
  lcd.print(":");
  lcd.setCursor(14,1);
  digit(menit);
}

void tombol(){

  buttonstate = digitalRead(buttonPin);
  if(buttonstate == HIGH){
    stopwatch();
  } else 
  if (buttonstate == LOW){
    start=0;
    /*
    lcd.setCursor(0,0);
    lcd.print("hidupkan!");
    */
    lcd.setCursor(0,1);
    lcd.print("Hidupkan");

  }
}


void stopwatch(){
  lcd.setCursor(0,1);
  dtk=start%60;
  mnt=(start/60)%60;
  jm=(start/3600)%24;
  digit(jm);
  lcd.print(":");
  digit(mnt);
  lcd.print(":");
  digit(dtk);
  
}


void cekmode(){
  if (man==false){
    otomts();
  } else
  if (man==true){
    manual();
  }
}

void otomts(){
  bacartc();

  totalmenit = ((jam*60)+menit);
  totaldetik = ((totalmenit*60)+detik);
  convertpembandingmenitnyala = ((hstart*60)+mstart);
  convertpembandingdetiknyala = ((convertpembandingmenitnyala*60)+sstart);
  convertpembandingmenitmati = ((hfinish*60)+mfinish);
  convertpembandingdetikmati = ((convertpembandingmenitmati*60)+sfinish);

  if((totaldetik>=convertpembandingdetiknyala) && (totaldetik<=convertpembandingdetikmati)){
    digitalWrite(13,HIGH);
  } else
  digitalWrite(13,LOW);
/* ////untuk cek detik, jangan dihapus
  Serial.println("waktu saat ini: ");
  Serial.println(totaldetik);
  Serial.println("waktu nyala: ");
  Serial.println(convertpembandingdetiknyala);
  Serial.println("waktu mati: ");
  Serial.println(convertpembandingdetikmati);
  delay(2000);
  */
}

void manual(){
  Serial.println("1 = on lampu");
  Serial.println("2 = off lampu");
  Serial.println("3 = atur waktu nyala / mati");
  Serial.println("4 = atur tanggal");
  Serial.println("5 = atur waktu");
  Serial.println("6 = waktu sekarang");
  Serial.println("7 = cek dht");
  Serial.println("8 = kembali ke otomatis");
  Serial.println("");
  pilihan();
}


void pilihan(){
  input = bacabyte();
  if(input==1){
    digitalWrite(13,HIGH);
    Serial.println("lampu nyala");
  } else
  if(input==2){
    Serial.println("lampu mati");
    digitalWrite(13,LOW);
  } else
  if(input==3){
    p3();
  } else
  if(input==4){
    p4();
  }else
  if(input==5){
    p5();
  } else
  if (input==6){
    p6();
  }else
  if(input==7){
    p7();
  }else
  if(input==8){
    p8();
  }else
  Serial.println("tidak ada");
}

void p3(){
  Serial.println("atur jam nyala lampu");
  Serial.println("jam awal 0-23");
  jamawal = bacabyte();
  Serial.println(jamawal);
  Serial.println("menit awal, 0-59");
  menitawal = bacabyte();
  Serial.println(menitawal);
  Serial.println("detik awal, 0-59");
  detikawal = bacabyte();
  Serial.println(detikawal);
  Serial.println("jam akhir, 0-23");
  jamakhir = bacabyte();
  Serial.println(jamakhir);
  Serial.println("menit akhir, 0-59");
  menitakhir=bacabyte();
  Serial.println(menitakhir);
  Serial.println("detik akhir, 0-59");
  detikakhir=bacabyte();
  Serial.println(detikakhir);
  EEPROM.update(addressjamawaleeprom,jamawal);
  EEPROM.update(addressmenitawaleeprom, menitawal);
  EEPROM.update(addressdetikawaleeprom,detikawal);
  EEPROM.update(addressjamakhireeprom, jamakhir);
  EEPROM.update(addressmenitakhireeprom, menitakhir);
  EEPROM.update(addressdetikakhireeprom, detikakhir);
  
}
void p4(){
  Serial.println("pengaturan tanggal");
  Serial.println("tahun saat ini, 00-99");
  tahun = bacabyte();
  Serial.println(tahun);
  Serial.println("bulan saat ini, 1-12");
  bulan = bacabyte();
  Serial.println(namabulan[bulan-1]);
  Serial.println("tanggal saat ini, 1-31");
  tanggal = bacabyte();
  Serial.println(tanggal);
  Serial.println("hari saat ini, 1- 7, 1 Min | 2 Sen | 3 Sel | 4 Rab | 5 Kam | 6 Jum | 7 Sab ");
  hari = bacabyte();
  Serial.println(namahari[hari-1]);
  Wire.beginTransmission(DS1307);
  Wire.write(byte(0));
  Wire.write(deckebcd(detik));
  Wire.write(deckebcd(menit));
  Wire.write(deckebcd(jam));
  Wire.write(deckebcd(hari));
  Wire.write(deckebcd(tanggal));
  Wire.write(deckebcd(bulan));
  Wire.write(deckebcd(tahun));
  Wire.write(byte(0));
  Wire.endTransmission();
  tampilkanrtc();
}
void p5(){
  Serial.println("pengaturan waktu");
  Serial.println("jam saat ini, 0-23");
  jam = bacabyte();
  Serial.println(jam);
  Serial.println("menit saat ini, 0-59");
  menit = bacabyte();
  Serial.println(menit);
  Wire.beginTransmission(DS1307);
  Wire.write(byte(0));
  Wire.write(deckebcd(detik));
  Wire.write(deckebcd(menit));
  Wire.write(deckebcd(jam));
  Wire.write(deckebcd(hari));
  Wire.write(deckebcd(tanggal));
  Wire.write(deckebcd(bulan));
  Wire.write(deckebcd(tahun));
  Wire.write(byte(0));
  Wire.endTransmission();
  tampilkanrtc();
}

void tampilkanrtc() {
  char buffer[3];
  const char* AMPM = 0;
  bacartc();
  Serial.print(namahari[hari-1]);
  Serial.print(" ");
  Serial.print(tanggal);
  Serial.print(" ");
  Serial.print(namabulan[bulan-1]);
  Serial.print(", 20");
  Serial.print(tahun);
  Serial.print(" ");
  if (jam > 12) {
    jam -= 12;
    AMPM = " Sore";
  }
  else AMPM = " Pagi";
  Serial.print(jam);
  Serial.print(":");
  sprintf(buffer, "%02d", menit);
  Serial.print(buffer);
  Serial.print(":");
  Serial.print(detik);
  Serial.println(AMPM);
}

void p6(){
  tampilkanrtc();
}
void p7(){
  
float h=dht.readHumidity();
float t=dht.readTemperature();
Serial.println ();
Serial.print(h);
Serial.print(" %   ");
Serial.print(t);
Serial.println(" °C");
}
void p8(){
  man=false;
  Serial.println(man);
  Serial.println("otomatis berjalan");
  Serial.println("2 = manual");
}

void waterpump(){
bacartc();

if (jam>=6 && jam <=20){
if(menit > 0 && menit <= 1){
  digitalWrite(pinpompa,HIGH);
} else if (menit >10 && menit <=11){
  digitalWrite(pinpompa,HIGH);
} else if( menit>20 && menit <=21){
  digitalWrite(pinpompa,HIGH);
} else if( menit >30 && menit <=31){
  digitalWrite(pinpompa,HIGH);
} else if(menit >40 && menit <=41){
  digitalWrite(pinpompa,HIGH);
} else if(menit > 50 && menit <=51){
  digitalWrite(pinpompa,HIGH);
} else digitalWrite(pinpompa,LOW);
} else digitalWrite(pinpompa,LOW);


}

void serialEvent(){
    input = bacabyte();
    if (input==1){
      man=false;
      Serial.println("otomatis dipilih");
      Serial.println("2 = manual");
    } else
    if (input==2){
      man=true;
      Serial.println("manual dipilih");
      
    } else
    Serial.println("tidak ada");
    //====================
}

byte bacabyte() {
  while (!Serial.available()) delay(10);
  byte reading = 0;
  byte incomingByte = Serial.read();
  while (incomingByte != '\n') {
    if (incomingByte >= '0' && incomingByte <= '9')
      reading = reading * 10 + (incomingByte - '0');
    else;
    incomingByte = Serial.read();
    
  }
  Serial.flush();
  return reading;
}
