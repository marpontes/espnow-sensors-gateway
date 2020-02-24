#include <Bounce2.h>

#define STARTPORT 6
#define ENDPORT   13
#define DELAY     80
#define BPIN      4
#define MAXMODE   2

int currentPort = STARTPORT;
int mode = 0;                // 0=there and back; 1=oneway;
boolean goingUp = true;      // true=up;false=down
Bounce debouncer = Bounce(); // Instantiate a Bounce object

void setup() {
  debouncer.attach(BPIN,INPUT_PULLUP); // Attach the debouncer to a pin with INPUT_PULLUP mode
  debouncer.interval(25); // Use a debounce interval of 25 milliseconds
  
  for (int x = STARTPORT ; x <= ENDPORT ; x++) {
    pinMode(x, OUTPUT);
  }
}

void loop() {
  debouncer.update();
   if ( debouncer.fell() ) { 
     toggleMode();
   }
  digitalWrite(currentPort, LOW);
  
  switch(mode) {
    case 0: thereAndBack(); break;
    case 1: up(); break;
    case 2: down(); break;
  }
  
  digitalWrite(currentPort, HIGH);
  delay(DELAY);
}

void up(){
  currentPort = currentPort + 1 > ENDPORT
                ? STARTPORT
                : currentPort + 1;
}

void down(){
  currentPort = currentPort - 1 < STARTPORT
                  ? ENDPORT
                  : currentPort - 1;
}

void thereAndBack(){
  if(goingUp && currentPort + 1 <= ENDPORT){
    up();
  }else if(goingUp && currentPort + 1 > ENDPORT){
    down();
    goingUp = !goingUp;
  }else if(!goingUp && currentPort - 1 >= STARTPORT){
    down();
  }else if(!goingUp && currentPort - 1 < STARTPORT){
    up();
    goingUp = !goingUp;
  }
}

void toggleMode(){
  mode = mode + 1 > MAXMODE
                ? 0
                : mode + 1;
}
