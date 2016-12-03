//                    Here's the Arduino code
//
///                    Thanks to:
//                 Augusto Carmo (carmolim) 2012 - https://github.com/carmolim/cycloduino
//
//
//                   There's a lot of bicycle projects out there, This one has a timer interrupt instead of a hardware interrupt
//-----------------------------------------
//CYCLES
////////

const int oneSecCycle      = 1000;            // 1 second
unsigned long before1Sec   = 0;

const int buttonDebounce   = 200;             // 500 milisegundos
unsigned long beforeButton = 0;
// SENSORS
//////////

const int speedReed        = 2;              // speed reed switch
const int cadenceReed      = 3;              // cadence reed switch

// TOTAL MEASURES
/////////////////

const int maxReedCounter  = 80;               // min time (in ms) of one rotation (for debouncing)
float odometer            = 0;                // total distante
int loopCounter           = 0;                // how many times the loop run before the ride started


// PER RIDE
///////////

long rideTime             = 0;                // total time of the ride
long movingTime           = 0;                // only the moving time
long millisCount          = 0;                // stores the number of cicles runned in the interrupt
float rideDistance        = 0.00;             // total distance of the ride in Km
boolean rideStarted       = false;            // if the bike is moving = true
boolean moving            = false;            // if the bike is moving = true

// SPEED VARIBALES
//////////////////

long speedTimer           = 0;                 // time between one full rotation (in ms)
long speedNumberSamples   = 0;                 // total of revolutions made by the front wheel
float speedSamplesSum     = 0.00;              // sum of all the speeds collected
float circumference       = 678;             // lenght of the wheel
float kph                 = 0.00;              // speed in kph
float mph                 = 0.00;              // speed in mph
float topSpeed            = 0.00;              // top speed of the ride
float avgSpeed            = 0.00;              // average speed of the ride
int speedReedVal          = 0;                 // ?? stores if the switch is open or closed // change to boolean?
int speedReedCounter      = 0;                 // ??


// CADENCE VARIABLES
////////////////////

long cadenceTimer         = 0;                 // time between one full rotation (in ms)
long cadenceNumberSamples = 0;                 // total of revolutions made by the front wheel
float cadenceSamplesSum   = 0.00;              // sum of all the speeds collected
float cadence             = 0.00;              // actual cadence
float avgCadence          = 0.00;              // average cadence of the ride
float topCadence          = 0.00;              // top cadence fo the ride
int cadenceReedVal        = 0;                 // stores if the switch is open or closed // change to boolean?
int cadenceReedCounter = 0; // ??


void setup() {
  speedReedCounter = maxReedCounter;      // ?
  cadenceReedCounter = maxReedCounter; // ?
  // speed input
  pinMode(speedReed, INPUT);

  // cadence input
  pinMode(cadenceReed, INPUT);

  // TIMER SETUP - the timer interrupt allows precise timed measurements of the reed switch
  // for more info about configuration of arduino timers see http://arduino.cc/playground/Code/Timer1

  cli(); // stop interrupts

  // set timer1 interrupt at 1kHz
  TCCR1A = 0; // set entire TCCR1A register to 0
  TCCR1B = 0; // same for TCCR1B
  TCNT1  = 0;

  // set timer count for 1khz increments
  OCR1A = 1999; // = (1/1000) / ((1/(16*10^6))*8) - 1

  // turn on CTC mode
  TCCR1B |= (1 << WGM12);

  // Set CS11 bit for 8 prescaler
  TCCR1B |= (1 << CS11);

  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);

  sei(); //allow interrupts

  //END TIMER SETUP

  Serial.begin(9600);
}
// Interrupt at freq of 1000 Hz to measure reed switch
ISR(TIMER1_COMPA_vect)
{

  //SPEED
  ///////

  // get val of A0
  speedReedVal = digitalRead(speedReed);

  // if reed switch is closed
  if (speedReedVal)
  {
    //min time between pulses has passed
    if (speedReedCounter == 0)
    {
      // min time between pulses has passed
      kph = (36*float(circumference))/float(speedTimer); // calculate kilometers per hour
  
      // reset speedTimer      
      speedTimer = 0;

      // reset speedReedCounter
      speedReedCounter = maxReedCounter;

      // increase number of samples by 1 - number of wheel rotations ajust the debouncer??
      speedNumberSamples++;  

      // starts the chronometer
      rideStarted = true;

      // the wheel is spinning
      moving = true;
    }

    else
    {
      if (speedReedCounter > 0)
      {
        // don't let speedReedCounter go negative
        speedReedCounter -= 1; // decrement speedReedCounter
      }
    }
  }

  else
  {
    // if reed switch is open
    if (speedReedCounter > 0)
    {
      // don't let speedReedCounter go negative
      speedReedCounter -= 1; // decrement speedReedCounter
    }
  }

  if (speedTimer > 2000)
  {
    // if no new pulses from reed switch- tire is still, set kmh to 0
    kph = 0; 

    // the bike is not moving
    moving = false;
  }

  else
  {
    speedTimer += 1; // increment speedTimer
  } 


  // CADENCE
  //////////

  // get val of A1
  cadenceReedVal = digitalRead(cadenceReed);

  // if reed switch is if closed
  if(cadenceReedVal)
  {
    //min time between pulses has passed
    if (cadenceReedCounter == 0)
    {
      // calculate rotations per minute 
      cadence = float(60000)/float(cadenceTimer);

      // reset timer
      cadenceTimer = 0;

      // reset reedCounter
      cadenceReedCounter = maxReedCounter;

      // increase number of samples by 1      
      cadenceNumberSamples++;  

      // starts the chronometer
      rideStarted = true;

      // the wheel is spinning
      moving = true;                                 
    }

    else
    {
      if(cadenceReedCounter > 0)
      {// don't let cadenceReedCounter go negative

        // decrement cadenceReedCounter
        cadenceReedCounter -= 1;    
      }
    }
  }

  // if reed switch is open
  else
  {
    // don't let cadenceReedCounter go negative
    if (cadenceReedCounter > 0)
    {   
      // decrement cadenceReedCounter
      cadenceReedCounter -= 1;
    }
  }

  if (cadenceTimer > 2000)
  {
    cadence = 0; 
  }

  else
  {
    cadenceTimer += 1; // increment timer
  } 


  // TIME
  ///////

  // if the timeCount is == to 1000 (1s)
  if(millisCount == 1000)
  {
    // if the ride started...
    if(rideStarted)
    {
      // increments 1 once a second
      rideTime += 1;  
    }
    
    // if the bike is moving...
    if(moving)
    {
      // increments 1 once a second
      movingTime += 1;
    }
    
    // reset the counter
    millisCount = 0; 
  }

  // increments 1 every cicle
  millisCount += 1;

} // end of timer

void loop() {
    // DISTANCE
    ///////////
    
    // Ride distance
    rideDistance = circumference * (float) speedNumberSamples / 100000;     // calculate distance in Km (1000 m)  


    // SPEED
    ////////

    // verifies if this speed is the top speed of the ride
    if(kph > topSpeed)
    {
      topSpeed = kph;
    }      

    // average speed
    speedSamplesSum += kph;                                // add the new calculate kph                                     
    avgSpeed = speedSamplesSum / (float) movingTime;       // calculate average speed

    // print kph once a second
    //displayKMH();


    // CADENCE
    //////////

    // verifies if this cadence is the top cadence of the ride
    if(cadence > topCadence)
    {
      topCadence = cadence;
    }

    // average cadence 
    cadenceSamplesSum += cadence;                          // add the new calculate cadence
    avgCadence = cadenceSamplesSum / (float) movingTime;   // calculate average cadence

    // print cadence once a second
//displayCadence();

sendToUART();

}
String TX = "";
void sendToUART(){
  TX = "<";
  TX+= "12.3"; 
 TX += ",";
 TX+= "12.3";
  TX += ",";
 TX += "12.5";
TX += ",";
TX+= "12.5";
TX += ">";
 Serial.println(TX);
  
}
