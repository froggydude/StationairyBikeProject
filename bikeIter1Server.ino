int Reed1 = 52;
int Reed2 = 53;

void setup() {
pinMode(Reed1, INPUT);
pinMode(Reed2, INPUT);
Serial.begin(9600);
  // put your setup code here, to run once:

}
int r;
void loop() {
  if(digitalRead(Reed1)==0){Serial.println('A');}
  if(digitalRead(Reed2)==0){Serial.println('B');}
  else(Serial.println('N'));
  delay(100);
//r= digitalRead(Reed1);
//Serial.println(r);
  //Serial.println(digitalRead(Reed1));
  //Serial.println("---------");
  //Serial.println(digitalRead(Reed2));
 // Serial.println("++++++++++");
  // put your main code here, to run repeatedly:

}
