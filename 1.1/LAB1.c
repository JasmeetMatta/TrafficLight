// C++ code
//

#define redState 0
#define yellowState 1
#define greenState 2
#define t10 1000
#define t20 2000



const int redLed = 8;
const int yellowLed = 9;
const int greenLed = 10;

volatile int state = redState;
volatile int stateFlow = 0;

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(redLed, OUTPUT);
  pinMode(yellowLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
}

void loop()
{  
  switchState();
}

void switchState(){
  switch(state){
    case redState:
    digitalWrite(redLed,HIGH);
    delay(t20);
    digitalWrite(redLed,LOW);
    state = yellowState;
    break;
    case yellowState:
    digitalWrite(yellowLed,HIGH);
    delay(t10);
    digitalWrite(yellowLed,LOW);
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
    digitalWrite(greenLed,HIGH);
    delay(t20);
    digitalWrite(greenLed,LOW);
    state = yellowState;
    break;
         }
    
  }

