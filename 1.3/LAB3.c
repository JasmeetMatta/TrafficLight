// C++ code
//

#define redState 0
#define yellowState 1
#define greenState 2


volatile int t10=2000;
volatile int t20=4000;



const int redLed = 8;
const int yellowLed = 9;
const int greenLed = 10;
const int pedRedLed = 12;
const int pedGreenLed = 11;

const int button = 2;



volatile int buttonState =0;


volatile int state = redState;
volatile int stateFlow = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(redLed, OUTPUT);
  pinMode(yellowLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(pedRedLed, OUTPUT);
  pinMode(pedGreenLed, OUTPUT);
  
  //attachInterrupt(digitalPinToInterrupt(2), interuptEvent, RISING);

}

void loop()
{  
  switchState();
  buttonState=digitalRead(button);
  Serial.println(buttonState);
  if(buttonState == 1){
    interuptEvent();
    buttonState = 0;
  }


}

void switchState(){
  switch(state){
    case redState:
    digitalWrite(redLed,HIGH);
    digitalWrite(pedRedLed,LOW);
    digitalWrite(pedGreenLed,HIGH);
    delay(t20);
    digitalWrite(redLed,LOW);
    digitalWrite(pedGreenLed,LOW);
    state = yellowState;
    break;
    case yellowState:
    digitalWrite(yellowLed,HIGH);
    digitalWrite(pedRedLed,HIGH);
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

void interuptEvent(){
  if (state == redState){
    t10 =2000;
    t20 =4000; 
    pedLight();
  }
  else{
    buttonState = 1;
    t10 = 1000;
    t20 = 2000;
  }
}
void pedLight(){
  digitalWrite(redLed,HIGH);
  digitalWrite(pedRedLed,LOW);
  digitalWrite(pedGreenLed,HIGH);
  delay(6000);
  digitalWrite(redLed,LOW);
  digitalWrite(pedGreenLed,LOW);
  Serial.println("Bye");
  state= yellowState;

}

