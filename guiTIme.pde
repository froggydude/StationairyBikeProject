import processing.serial.*;

// Need G4P library
import g4p_controls.*;
int g;
double spd, cad, hr, oilTemp;
Table stats;
Serial Arduino;
public void setup(){
  size(480, 320, JAVA2D);
  createGUI();
  customGUI();
  
  
 // String port = Serial.list()[0];
  //Arduino = new Serial(this, port, 115200);
  
  /*stats = new Table();
  stats.addColumn("maxSpeed");
  stats.addColumn("avgSpeed");
  stats.addColumn("maxHR");
  stats.addColumn("avgHR");
  stats.addColumn("maxCadence");
  stats.addColumn("avgCadence");
  stats.addColumn("date");*/
  // Place your setup code here
  
}
String gt;
public void draw(){

  background(127);
  /*serInput();
 curSpd.setText(str(spd));
 CurrentCad.setText(str(cad));
 curHR.setText(str(hr));*/
}

// Use this method to add additional statements
// to customise the GUI controls
public void customGUI(){
}
//------------------it feels good reusing your own code, this could turn into something, if it already hasnt.
void serInput() {
  String serRX;
  serRX = Arduino.readStringUntil('>');      
  String temp=",,,,";                            
  int i = 0;                              
  char q;                                  
  int w = 1;                            
  float g = 0;                               
  //Serial.println("--------------" + serRX);     //debuggary
  while (serRX.length() >= i)                    //iterate though bluetooth string
  { //open braket ;)

    q = serRX.charAt(i);                              
    //------------------case a comma is read, convert to int and set variables accordingly (spd, cad, hr) temp of the oil soon
    if (q == ',') {               //if there is a comma, do some processing
      g = Float.parseFloat(temp);                    //convert temp built string to int
      //Serial.println(g);
      switch (w) {                //switch based on what iteraton of string i.e: "1,2,3,4"
        case (1):
          //first case is PIBT
          spd = g;
          println(spd);
          w++;
          break;
        case (2):
          //second case is PI
          cad = g;
          println(cad);
          w++;
          break;
        //third case is BT
        case (3):
          hr = g;
          println(hr);
          w++;
          break;
        //forth case is temperature setpoint
        case (4):
          println(oilTemp);
          oilTemp = g - 10;
          break;
        default:
          break;
      }
      temp = "";   //if the string has been processed, start over
    }
    //----------end comma accorence
    //---otherwise add to temp string
    else {
      temp = temp + q;      //build string char by char
    }

    i++;     //iterate thorugh temp string charbychar
  }
}
//-------------------------
