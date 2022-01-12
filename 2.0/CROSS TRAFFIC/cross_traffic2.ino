// C++ code
//

#define redLedPin 8
#define yellowLedPin 9
#define greenLedPin 10
#define pedRedLedPin 6
#define pedGreenLedPin 7
#define buttonPin 2

#define redState 44
#define yellowState 200
#define greenState 100
#define pedRedState 101
#define pedGreenState 301
#define timeState 120

//enum stateValue{redState,yellowState,greenState,pedRedState,pedGreenState,timeState};
//stateValue sValue;
volatile int stateFlow = 0;
volatile int t10 = 10000;
volatile int t20 = 20000;
volatile int state = greenState;
volatile int pedLedState = pedGreenState;
volatile int buttonState = LOW;
volatile int sValue = 0;
int incomingByte = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(buttonPin,INPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(yellowLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(pedRedLedPin, OUTPUT);
  pinMode(pedGreenLedPin, OUTPUT);
  //attachInterrupt(digitalPinToInterrupt(2), interuptEvent, RISING);
//  attachInterrupt(buttonPin, interuptEvent, RISING);


}

void loop(){
  //Serial.print(buttonState);
  if (Serial.available()>0){
    incomingByte = Serial.read();
    sValue = incomingByte;
  }
  setState();
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
    	//delay(t20);
    	digitalWrite(redLedPin,LOW);
    	//digitalWrite(pedGreenLed,LOW);
    	//state = yellowState;
    	pedLedState = pedRedState;
    	break;
    case yellowState:
    	digitalWrite(yellowLedPin,HIGH);
    	pedState();
    	//digitalWrite(pedRedLed,HIGH);
    	delay(t10/5);
    	digitalWrite(yellowLedPin,LOW);
    	/*if (stateFlow == 0){
      		state = greenState;
      		stateFlow = 1;
    	}
    	else{
      		state = redState;
      		stateFlow = 0;
        }*/
    	break;
    case greenState:
    	digitalWrite(greenLedPin,HIGH);
    	pedLedState = pedRedState;
    	pedState();
    	//delay(t20);
    	digitalWrite(greenLedPin,LOW);
    	//state = yellowState;
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
/*void interuptEvent(){
  if(state == redState && buttonState == HIGH){
    buttonState = LOW;
    timeReset();
    switchState();
  }
  else if (state != redState){
    buttonState = HIGH;
    //buttonStatePrint();
    changeTime();
  }
}*/
void timeReset(){
  t10 = 10000;
  t20 = 20000;
}
void changeTime(){
  t10 = 2000;
  t20 = 4000;  
}
void buttonStatePrint(){
  Serial.print(buttonState);
}
void setState(){
  if (sValue == 120){
    //changeTime();
    //interuptEvent();
  }
  /*else if(sValue == 200){
    delay(t10);
    state = sValue;
  }*/
  else {
    state = sValue;
  }
}