int ldr;
const int ledpin = 13;
int varhitung;
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
pinMode(ledpin,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
ldr=analogRead(A7); // A7 = pin data ldr
Serial.println(varhitung);
delay(500);
if (ldr>450){
  varhitung=varhitung+1;
  //digitalWrite(ledpin,HIGH);
}
}
