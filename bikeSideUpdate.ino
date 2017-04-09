int Wheel_pin = 2;
int Crank_pin = 3;
#define RADIUS 67.8 //cm 700cx28 trainer
#define MAGNETS 1 //number of magnets
#define CIRCUMFERENCE 2*RADIUS*3.1415926 //cm
#define MAGNETDISTANCE CIRCUM/MAGNETS //cm
#define RESETSPEED 2500 //ms, reset speed to zero
#define DEBOUNCETIMEINTERVAL (MAGNETDISTANCE*36)/80
#define MAGNETREVOLUTION //you say you want a revolution, well.. ya know the song

volatile boolean needRefresh = true;


void setup() {
  //---------interrupt routine setup
  interruptSetup();
pinMode(Wheel_pin, INPUT_PULLUP);
pinMode(Crank_pin, INPUT_PULLUP);
Serial.begin(9600);
  // put your setup code here, to run once:
}
void loop() {
  if(needRefresh){
    needRefresh=false;
    serSend();
  }
  delay(100);
}
