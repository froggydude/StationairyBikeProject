
import processing.serial.*;


Serial Arduino;
void setup(){
  size(800,600);
  background(127);
  String portName = Serial.list()[0];
  Arduino = new Serial(this, portName, 9600);
  Arduino.bufferUntil('\n');



}
char curVal, preVal;
void draw(){
  
}

void serialEvent(Serial Ard){
  curVal = Ard.readChar();    //get Fresh Data
  
  //if(Arduino.available()>0){
    
  //    val = Arduino.readChar();
     // println(val);
      if(curVal!=preVal)
      {
                                              /*if(val == 'A')
                                              {  
                                                background(0);
                                              }*/
      
      if(curVal == 'B')
      {
       println(curVal);
       background(255,0,0);
       
      }
      if(curVal == 'N'){background(0,255,0);}
  }
  
  preVal=curVal;
  }
  
  