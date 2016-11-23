// Need G4P library
import g4p_controls.*;
int g;
Table stats;
public void setup(){
  size(480, 320, JAVA2D);
  createGUI();
  customGUI();
  stats = new Table();
  stats.addColumn("maxSpeed");
  stats.addColumn("avgSpeed");
  stats.addColumn("maxHR");
  stats.addColumn("avgHR");
  stats.addColumn("maxCadence");
  stats.addColumn("avgCadence");
  stats.addColumn("date");
  // Place your setup code here
  
}

public void draw(){
  background(230);
  if(g==3){background(0);}
  
}

// Use this method to add additional statements
// to customise the GUI controls
public void customGUI(){

}