
volatile float rpm = 0;  //rotations of crank
volatile float kph = 0;
volatile float dailyKm;
int cadenceReedVal;
boolean rideStarted;
volatile boolean moving;
volatile int cadence;
volatile int speedTimer;
volatile int maxReedCounter;
volatile int speedReedCounter;
volatile int cadenceTimer;
volatile int cadenceNumberSamples;
volatile unsigned long speedNumberSamples;
volatile unsigned long speedReedVal;
volatile unsigned long cadenceReedCounter;
volatile unsigned long millisCount;
volatile unsigned long rideTime;
volatile unsigned long movingTime;
volatile byte stoppedState = 0;


void interruptSetup(){
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
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

// TIMER

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////


// Interrupt at freq of 1000 Hz to measure reed switch
ISR(TIMER1_COMPA_vect)
{

  //SPEED
  ///////

  // get val of A0
  speedReedVal = digitalRead(Wheel_pin);

  // if reed switch is closed
  if (speedReedVal)
  {
    //min time between pulses has passed
    if (speedReedCounter == 0)
    {
      // min time between pulses has passed
      kph = (36*float(CIRCUMFERENCE))/float(speedTimer); // calculate kilometers per hour
  
      // reset speedTimer      
      speedTimer = 0;

      // reset Counter
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
  cadenceReedVal = digitalRead(Crank_pin);

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

