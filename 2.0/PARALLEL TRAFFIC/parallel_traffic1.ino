// C++ code
//

#define redLedPin 8
#define yellowLedPin 9
#define greenLedPin 10
#define pedRedLedPin 6
#define pedGreenLedPin 7
#define buttonPin 2

#define redState 100
#define yellowState 200
#define greenState 44
#define pedRedState 101
#define pedGreenState 301

//enum stateValue{100,200,300,101,301};

volatile int stateFlow = 0;
volatile int t10 = 10000;
volatile int t20 = 20000;
volatile int state = redState;
volatile int pedLedState = pedGreenState;
volatile int buttonState = LOW;


void setup()
{
  Serial.begin(9600);
  pinMode(buttonPin,INPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(yellowLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(pedRedLedPin, OUTPUT);
  pinMode(pedGreenLedPin, OUTPUT);
  //pinMode(11,OUTPUT);
  attachInterrupt(digitalPinToInterrupt(2), interuptEvent, RISING);
//  attachInterrupt(buttonPin, interuptEvent, RISING);


}

void loop(){
  //Serial.print(buttonState);
  Serial.write(state);
  switchState();
}

void switchState(){
  switch(state){
    case redState:
    	timeReset();
    	digitalWrite(redLedPin,HIGH);
    	pedLedState = pedGreenState;
    	pedState();
    	//digitalWrite(pedRedLed,LOW);
    	//digitalWrite(pedGreenLed,HIGH);
    	delay(t20);
    	digitalWrite(redLedPin,LOW);
    	//digitalWrite(pedGreenLed,LOW);
    	state = yellowState;
    	pedLedState = pedRedState;
    	break;
    case yellowState:
    	digitalWrite(yellowLedPin,HIGH);
    	pedState();
    	//digitalWrite(pedRedLed,HIGH);
    	delay(t10);
    	digitalWrite(yellowLedPin,LOW);
    	if (stateFlow == 0){
      		state = greenState;
      		stateFlow = 1;
    	}
    	else{
      		state = redState;
      		stateFlow = 0;
        }
    	break;
    case greenState:
    	digitalWrite(greenLedPin,HIGH);
    	delay(t20);
    	digitalWrite(greenLedPin,LOW);
    	state = yellowState;
    	break;
         }
}
void pedState(){
  if (pedLedState == pedRedState){
    digitalWrite(pedGreenLedPin,LOW);
    digitalWrite(pedRedLedPin,HIGH);
  }
  else if (pedLedState == pedGreenState){
    //digitalWrite(redLedPin,HIGH);
    digitalWrite(pedGreenLedPin,HIGH);
    digitalWrite(pedRedLedPin,LOW);
  }
}
void interuptEvent(){
  //digitalWrite(11,HIGH);
  //digitalWrite(11,LOW);
  if(state == redState && buttonState == HIGH){
    buttonState = LOW;
    timeReset();
    switchState();
  }
  else if (state != redState){
    buttonState = HIGH;
    Serial.write(120);
    t10 = 2000;
    t20 = 4000;
  }
}
void timeReset(){
  t10 = 10000;
  t20 = 20000;
}
void buttonStatePrint(){
  Serial.print(buttonState);
}