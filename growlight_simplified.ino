/*=============================================
 * rtc == arduino
 * gnd == gnd
 * vcc == 5v
 * sda == sda
 * scl == scl
 * 
 * relay lampu == arduino
 * gnd == pinout
 * 
 * dht == arduino
 * + == 5v
 * - == gnd
 * data == pinout
 * 
 * 
 * 
 * 
 */






#include <Wire.h>
#include <DHT.h>
#define DHTTYPE DHT22
#define DHTPIN 9
DHT dht(DHTPIN, DHTTYPE);

const int DS1307 = 0x68; // Address of DS1307 see data sheets
const char* namahari[] =
{"Minggu", "Senin", "Selasa", "Rabu", "Kamis", "Jum'at", "Sabtu"};
const char* namabulan[] =
{"Januari", "Februari", "Maret", "April", "Mei", "Juni", "Juli", "Augustus","September", "Oktober", "November", "Desember"};
 
bool man= false;

byte detik = 0;
byte menit = 0;
byte jam = 0;
byte hari = 0;
byte tanggal = 0;
byte bulan = 0;
byte tahun = 0;
byte input;
void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  dht.begin();
  Serial.begin(9600);
  pinMode(13,OUTPUT); //pin relay lampu
  tampilkanrtc();
  cekmode();
  Serial.println(man);
  Serial.println("");
  Serial.println("1 = otomatis");
  Serial.println("2 = manual");
}

void loop() {
  // put your main code here, to run repeatedly:
  cekmode();
}

////////////////////////////////////////////////////////////////////////////////////

void cekmode(){
  if (man==false){
    otomts();
  } else
  if (man==true){
    manual();
  }
}
void manual(){
  //on
  //off
  //lama penyinaran
  //atur tangggal
  //atur waktu
  //baca waktu
  //cek sensor
  //kembali ke otomatis
  Serial.println("1 = on lampu");
  Serial.println("2 = off lampu");
  Serial.println("3 = atur lama penyinaran");
  Serial.println("4 = atur tanggal");
  Serial.println("5 = atur waktu");
  Serial.println("6 = waktu sekarang");
  Serial.println("7 = cek dht");
  Serial.println("8 = kembali ke otomatis");
  Serial.println("");
  pilihan();
}
void otomts(){
  bacartc();
  if ((jam>=6) *(menit>=0) && (jam<=17)*(menit<=59)){
    digitalWrite(13,HIGH);
  } else
  digitalWrite(13,LOW);
  
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

////////////////////////////////////////////////////////////////////////////////////
void bacartc() {
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

////////////////////////////////////////////////////////////////////////////////////
void p3(){
  
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
////////////////////////////////////////////////////////////////////////////////////
byte deckebcd(byte val) {
  return ((val/10*16) + (val%10));
}
byte bcdkedec(byte val) {
  return ((val/16*10) + (val%16));
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
