#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <chrono.h>
#include <thread.h>
#define NUM_THREADS 2

#define t10=3000;
#define t20=6000;

pthread_mutex_t mutext_time = PTHREAD_MUTEX_INITALIZER;// Genral mutex in case there are many function accessing/changing  the time at same time 
// At that moment need more SYNC or genreal roundabout way for the traffic to continue

#define redState 0
#define yellowState 1
#define greenState 2

int state = redState;
int stateFlow = 0;
int buttonState = 0;


int main(){

    while(true){
        laneLight();
    }
}

void controller(){
    // Some function to get button state can use input as example 
    if buttonState == 1 {
        if state == 0{
            pthread_mutex_lock(&mutext_time);  
             
            pedLightButtonPressed();
            buttonState = 0;
            pthread_mutex_unlock(&mutext_time);  
            
        }
        else{
            pthread_mutex_lock(&mutext_time);  
             
            t10=2000;
            t20=4000;
            laneLight();
            pthread_mutex_unlock(&mutext_time);  
            

        }
    else{
        laneLight();
    }
    }
}

void laneLight(){
    switch(state):
        case 0:
            printf("REDPedLight OFF")
            printf("RED LIGHT ON");
            printf("GreenPedLight ON");
            sleep_until(system_clock::now() + t20ns)
            printf("RED LIGHT Off");
            printf("GreenPedLight Off");
            state = 1;
            break;
        case 1:
            if (stateFlow == 0){
                printf("Yellow LIGHT ON");
                printf("RedPedLight ON");
                sleep_until(system_clock::now() + t10ns)
                printf("Yellow LIGHT Off");
                //printf("GreenPedLightON");
                stateFlow = 1;
                state = 2;
            }
            if (stateFlow == 1){
                printf("Yellow LIGHT ON");
                printf("RedPedLight ON");
                sleep_until(system_clock::now() + t10ns)
                printf("Yellow LIGHT Off");
                //printf("GreenPedLightON");
                stateFlow = 0;
                state = 0;
            }
            break;
        case 2:
            printf("Greem LIGHT ON");
            //printf("GreenPedLightON");
            sleep_until(system_clock::now() + t20ns)
            printf("Greem LIGHT Off");
            state = 1;
            break;

}

void pedLightButtonPressed(){
//    long tid;
//    tid = (long)threadid;
    print("redLed,HIGH");
    print("pedRedLed,LOW");
    print("pedGreenLed,HIGH");
    sleep_until(system_clock::now() + t20ns);
    print("redLed,LOW");
    print("pedGreenLed,LOW");
    state= yellowState;

}